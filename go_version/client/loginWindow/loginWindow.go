package loginWindow

import (
	"commsys/client/process"
	"commsys/common/message"
	"commsys/common/utils"
	"encoding/json"
	"errors"
	"fmt"
	"net"
)

type LoginWindow struct {
	userid string
	passwd string
	tf     *utils.Transfer
}

func (lw *LoginWindow) LoginCheck(userid string, passwd string) (err error) {
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
	var loginMsg message.LoginMsg
	loginMsg.UserId = userid
	loginMsg.UserPwd = passwd

	// 4. 将loginMsg序列化
	data, err := json.Marshal(loginMsg)
	// fmt.Printf("%c\n", data)
	if err != nil {
		fmt.Println("json.Marshal err = ", err)
		return err
	}

	// 5. 把data赋给msg.Data字段，把长度赋给msg.Length字段
	msg.Data = string(data)
	msg.Length = len(msg.Data)
	msg.Type = message.LoginMsgType

	// 构建transfer实例
	lw.tf.Conn = conn
	// 6. 将msg发送到服务器
	lw.tf.WritePkg(msg)

	fmt.Println("客户端发送消息成功")

	// 处理服务器返回的消息

	ResMsg, err := lw.tf.ReadPkg()

	// 反序列化
	var loginRes message.ResponseMsg
	err = json.Unmarshal([]byte(ResMsg.Data), &loginRes)
	if err != nil {
		fmt.Println("json unmarshal err = ", err)
		return
	}

	if loginRes.Code == 200 {
		lsw := &process.LoginSuccWindow{
			Loop:      true,
			Tf:        lw.tf,
			SrcUserId: userid,
		}

		// 启动一个协程，保持和服务器端通讯，如果服务器有数据推送给客户端，可以接收并显示在客户端的终端
		go lsw.RecvMsgFromServer(conn)
		// 显示登录成功后的二级菜单
		lsw.ShowMenu()
		return
	} else {
		fmt.Println(loginRes.Error)
		return errors.New("账号密码错误")
	}
}

func (lw *LoginWindow) LoginShow() {
	fmt.Println("请输入用户的id")
	fmt.Scanf("%s\n", &lw.userid)
	fmt.Println("请输入用户的密码")
	fmt.Scanf("%s\n", &lw.passwd)
	lw.tf = &utils.Transfer{}
	err := lw.LoginCheck(lw.userid, lw.passwd)
	if err != nil {
		fmt.Println("登录失败")
	} else {
		fmt.Println("登录成功")
	}
}
