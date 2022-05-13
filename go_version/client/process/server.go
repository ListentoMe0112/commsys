package process

import (
	"commsys/common/message"
	"commsys/common/utils"
	"encoding/json"
	"fmt"
	"net"
)

type LoginSuccWindow struct {
	key       int
	Loop      bool
	Tf        *utils.Transfer
	SrcUserId string
}

func (lsw *LoginSuccWindow) Broadcast(content string) (err error) {
	var smsmsg message.SmsMsg
	smsmsg.Content = content
	smsmsg.SrcUserId = lsw.SrcUserId
	smsmsg.DstUserId = "11111111"

	data, err := json.Marshal(smsmsg)

	if err != nil {
		fmt.Println("json.Marshal err = ", err)
		return err
	}
	var msg message.Message
	// 5. 把data赋给msg.Data字段，把长度赋给msg.Length字段
	msg.Data = string(data)
	msg.Length = len(msg.Data)
	msg.Type = message.SmsMsgType
	lsw.Tf.WritePkg(msg)
	return
}

func (lsw *LoginSuccWindow) ShowMenu() {
	for lsw.Loop {
		fmt.Printf("--------------------恭喜%s登录成功--------------------\n", lsw.SrcUserId)
		fmt.Println("             1. 显示在线用户列表")
		fmt.Println("             2. 发送消息")
		fmt.Println("             3. 信息列表")
		fmt.Println("             4. 退出系统")
		fmt.Println("             请选择（1-4）： ")
		fmt.Scanf("%d\n", &lsw.key)

		switch lsw.key {
		case 1:
			fmt.Println("显示在线用户列表")
			for userid, user := range MyOnlineUsers {
				fmt.Printf("[Debug Info] %s : %s\n", userid, user.UserStatus)
			}
		case 2:
			fmt.Println("发送消息")
			var temp string
			fmt.Scanln(&temp)
			lsw.Broadcast(temp)
		case 3:
			fmt.Println("信息列表")

		case 4:
			fmt.Println("退出系统")
			lsw.Loop = false
		default:
			fmt.Println("输入错误，请选择（1-4）")
		}
	}
}

// 和服务器端保持通讯
func (lsw *LoginSuccWindow) RecvMsgFromServer(Conn net.Conn) {
	// 创建一个transfe实例，不断读取服务器发送的消息
	tf := &utils.Transfer{
		Conn: Conn,
	}
	for {
		fmt.Println("客户端正在等待读取服务器发送的消息")
		msg, err := tf.ReadPkg()
		if err != nil {
			fmt.Println("Read Err = ", err)
			return
		}
		// 如果读取到消息，下一步逻辑
		fmt.Printf("[Debug Info] msg = %v\n", msg)
		switch msg.Type {
		case message.NotifyUserStatusMsgType:
			var onlineUsersMsg message.NotifyUserStatusMsg
			json.Unmarshal([]byte(msg.Data), &onlineUsersMsg)

			fmt.Println("[Debug Info] ", onlineUsersMsg.UserId, " is OnLine")
			user, ok := MyOnlineUsers[onlineUsersMsg.UserId]
			if !ok {
				MyOnlineUsers[onlineUsersMsg.UserId] = &message.User{
					UserId:     onlineUsersMsg.UserId,
					UserStatus: message.UserOnline,
				}
			} else {
				user.UserStatus = message.UserOnline
			}
		case message.OnlineUsersMsgType:
			var onlineUsersMsg message.OnlineUsersMsg
			json.Unmarshal([]byte(msg.Data), &onlineUsersMsg)

			for idx, userid := range onlineUsersMsg.Users {
				fmt.Printf("[Debug Info] %d\t%s is online\n", idx, userid)
				user, ok := MyOnlineUsers[userid]
				if !ok {
					MyOnlineUsers[userid] = &message.User{
						UserId:     userid,
						UserStatus: message.UserOnline,
					}
				} else {
					user.UserStatus = message.UserOnline
				}
			}
		case message.SmsMsgType:
			var smsmsg message.SmsMsg
			err = json.Unmarshal([]byte(msg.Data), &smsmsg)
			if err != nil {
				fmt.Println("json unmarhsal err = ", err)
				return
			}

			fmt.Printf("Recv From %s: %s\n", smsmsg.SrcUserId, smsmsg.Content)
		}

	}
}
