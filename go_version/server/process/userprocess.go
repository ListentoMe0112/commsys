package process

import (
	"commsys/common/message"
	"commsys/common/utils"
	"commsys/server/model"
	"encoding/json"
	"fmt"
	"net"
)

type UserProcess struct {
	Conn   net.Conn
	UserId string
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
		// 添加到在线成员列表
		up.UserId = loginmsg.UserId
		fmt.Println("[Debug Info] After Login: ", loginmsg.UserId)
		MyUserMgr.AddOnlineUser(up)

		// 发送登录成功信息, 在线名单
		tf.WritePkg(LoginOkResCons())
		tf.WritePkg(OnlineUsersMsgCons())

		// 通知其他人自己上线
		up.NotifyOthersOnline()
	} else {

		tf.WritePkg(LoginFailResCons(err.Error()))
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
		tf.WritePkg(LoginOkResCons())
		// fmt.Println(user)
	} else {
		tf.WritePkg(LoginFailResCons(err.Error()))
	}
	return
}

func (up *UserProcess) NotifyOthersOnline() {
	// 遍历
	for id, nup := range MyUserMgr.OnlineUsers {
		if id == up.UserId {
			continue
		}
		nup.NotifyMeOnline(up.UserId)
	}
}

func (up *UserProcess) NotifyMeOnline(UserId string) {
	// 遍历
	tf := &utils.Transfer{
		Conn: up.Conn,
	}
	tf.WritePkg(NotifyUserStatusMsgCons(UserId, message.UserOnline))
}

func (up *UserProcess) ServerProcessSms(msg *message.Message) (err error) {
	var smsmsg message.SmsMsg
	err = json.Unmarshal([]byte(msg.Data), &smsmsg)
	if err != nil {
		fmt.Println("json unmarhsal err = ", err)
		return
	}

	tf := &utils.Transfer{}
	// fmt.Println(up.UserId)
	for id, user := range MyUserMgr.OnlineUsers {

		if id != smsmsg.SrcUserId {
			tf.Conn = user.Conn
			tf.WritePkg(*msg)
		}

	}

	return
}
