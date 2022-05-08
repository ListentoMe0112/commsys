package registerWindow

import (
	"commsys/common/message"
	"commsys/common/utils"
	"encoding/json"
	"errors"
	"fmt"
	"net"
)

type RegisterWindow struct {
	tf       *utils.Transfer
	userid   string
	passwd   string
	username string
}

func (rw *RegisterWindow) RegisterCheck(userid string, passwd string, username string) (err error) {
	// 开始制定协议。。。
	// fmt.Printf("userId = %s userpwd = %s\n", userid, passwd)
	// 1. 连接到服务器
	conn, err := net.Dial("tcp", "localhost:8889")
	if err != nil {
		fmt.Println("Dial err = ", err)
	}

	// 延时关闭
	defer conn.Close()

	// 2. 准备通过conn发送消息给服务
	var msg message.Message

	// 3. 创建一个LoginMsg结构体
	var registerMsg message.RegisterMsg
	registerMsg.UserIns.UserId = userid
	registerMsg.UserIns.UserPwd = passwd
	registerMsg.UserIns.UserName = username

	// 4. 将loginMsg序列化
	data, err := json.Marshal(registerMsg)
	// fmt.Printf("%c\n", data)
	if err != nil {
		fmt.Println("json.Marshal err = ", err)
		return err
	}

	// 5. 把data赋给msg.Data字段，把长度赋给msg.Length字段
	msg.Data = string(data)
	msg.Length = len(msg.Data)
	msg.Type = message.RegisterMsgType

	// 构建transfer实例
	rw.tf.Conn = conn
	// 6. 将msg发送到服务器
	rw.tf.WritePkg(msg)

	fmt.Println("客户端发送消息成功")

	// 处理服务器返回的消息

	ResMsg, err := rw.tf.ReadPkg()

	// 反序列化
	var loginRes message.ResponseMsg
	err = json.Unmarshal([]byte(ResMsg.Data), &loginRes)
	if err != nil {
		fmt.Println("json unmarshal err = ", err)
		return
	}

	if loginRes.Code == 200 {
		return
	} else {
		return errors.New(loginRes.Error)
	}
	return
}

func (rw *RegisterWindow) ShowMenu() {
	fmt.Println("请输入用户的id")
	fmt.Scanf("%s\n", &rw.userid)
	fmt.Println("请输入用户的密码")
	fmt.Scanf("%s\n", &rw.passwd)
	fmt.Println("请输入用户的用户名")
	fmt.Scanf("%s\n", &rw.username)
	rw.tf = &utils.Transfer{}
	err := rw.RegisterCheck(rw.userid, rw.passwd, rw.username)
	if err != nil {
		fmt.Println(err.Error())
	} else {
		fmt.Println("注册成功")
	}
}
