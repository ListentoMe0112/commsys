package process

import (
	"commsys/common/message"
	"commsys/common/utils"
	"commsys/server/model"
	"commsys/server/response"
	"encoding/json"
	"fmt"
	"net"
)

type UserProcess struct {
	Conn net.Conn
}

// 判断用户名密码是否合法

func (up *UserProcess) ServerProcessLogin(msg *message.Message) (err error) {
	var loginmsg message.LoginMsg
	err = json.Unmarshal([]byte(msg.Data), &loginmsg)
	if err != nil {
		fmt.Println("json unmarhsal err = ", err)
		return
	}

	tf := &utils.Transfer{
		Conn: up.Conn,
	}

	user, err := model.MyUserDao.LoginCheck(loginmsg.UserId, loginmsg.UserPwd)
	if err == nil {
		tf.WritePkg(response.LoginOkResCons())
		// fmt.Println(user)
	} else {
		tf.WritePkg(response.LoginFailResCons(err.Error()))
		fmt.Println(user)
	}
	return
}

func (up *UserProcess) ServerProcessRegister(msg *message.Message) (err error) {
	var registermsg message.RegisterMsg
	err = json.Unmarshal([]byte(msg.Data), &registermsg)
	if err != nil {
		fmt.Println("json unmarhsal err = ", err)
		return
	}

	tf := &utils.Transfer{
		Conn: up.Conn,
	}

	err = model.MyUserDao.RegisterCheck(&registermsg.UserIns)

	if err == nil {
		tf.WritePkg(response.LoginOkResCons())
		// fmt.Println(user)
	} else {
		tf.WritePkg(response.LoginFailResCons(err.Error()))
	}
	return
}
