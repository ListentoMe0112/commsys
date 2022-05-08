package message

import (
	"commsys/server/model"
)

const (
	LoginMsgType    = "LoginMsg"
	ResponseMsgType = "ResponseMsg"
	RegisterMsgType = "RegisterMsg"
)

type Message struct {
	Type   string `json:type`   // 消息类型
	Data   string `json:data`   // 数据
	Length int    `json:length` // 消息长度
}

type LoginMsg struct {
	UserId   string `json:userId`   //用户id
	UserPwd  string `json:userPwd`  //用户密码
	UserName string `json:userName` //用户名
}

type RegisterMsg struct {
	UserIns model.User `json:"user"`
}

// 状态码 500 表示该用户未注册 200表示登录成功 400表示改用户已经占用
type ResponseMsg struct {
	Code  int    `json:code`
	Error string `json:error` //返回的错误信息，如果没有则nil
}
