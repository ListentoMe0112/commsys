package response

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
