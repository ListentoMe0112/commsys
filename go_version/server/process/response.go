package process

import (
	"commsys/common/message"
	"encoding/json"
	"fmt"
)

func LoginOkResCons() message.Message {
	var resMsg message.Message
	resMsg.Type = message.ResponseMsgType
	var data message.ResponseMsg
	data.Code = 200 // 合法 登录成功
	data.Error = "Login Success"

	resMsgData, err := json.Marshal(data)
	if err != nil {
		fmt.Println("marshal err = ", err)
	}
	resMsg.Data = string(resMsgData)
	resMsg.Length = len(resMsg.Data)
	return resMsg
	// sendData, err := json.Marshal(resMsg)
	// if err != nil {
	// 	fmt.Println("marshal err = ", err)
	// }
	// return sendData
}

func LoginFailResCons(info string) message.Message {
	var resMsg message.Message
	resMsg.Type = message.ResponseMsgType
	var data message.ResponseMsg
	data.Code = 500 // 不合法 登录失败
	data.Error = info

	resMsgData, err := json.Marshal(data)
	if err != nil {
		fmt.Println("marshal err = ", err)
	}
	resMsg.Data = string(resMsgData)
	resMsg.Length = len(resMsg.Data)

	// sendData, err := json.Marshal(resMsg)
	// if err != nil {
	// 	fmt.Println("marshal err = ", err)
	// }
	// return sendData
	return resMsg
}

func OnlineUsersMsgCons() message.Message {
	var onlineUsersMsg message.OnlineUsersMsg
	var resMsg message.Message
	resMsg.Type = message.OnlineUsersMsgType

	for id := range MyUserMgr.OnlineUsers {
		onlineUsersMsg.Users = append(onlineUsersMsg.Users, id)
	}

	// fmt.Println("[Debug Info]: ", onlineUsersMsg.Users)

	resMsgData, err := json.Marshal(onlineUsersMsg)
	if err != nil {
		fmt.Println("marshal err = ", err)
	}
	resMsg.Data = string(resMsgData)
	resMsg.Length = len(resMsg.Data)

	return resMsg
}

func NotifyUserStatusMsgCons(UserId string, status string) message.Message {
	var notifyUserStatusMsg message.NotifyUserStatusMsg
	var resMsg message.Message
	resMsg.Type = message.NotifyUserStatusMsgType

	notifyUserStatusMsg.UserId = UserId
	notifyUserStatusMsg.Status = status

	resMsgData, err := json.Marshal(notifyUserStatusMsg)
	if err != nil {
		fmt.Println("marshal err = ", err)
	}

	resMsg.Data = string(resMsgData)
	resMsg.Length = len(resMsg.Data)

	return resMsg
}
