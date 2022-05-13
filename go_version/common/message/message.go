package message

import (
	"commsys/server/model"
)

const (
	LoginMsgType            = "LoginMsg"
	ResponseMsgType         = "ResponseMsg"
	RegisterMsgType         = "RegisterMsg"
	OnlineUsersMsgType      = "OnlineUsersMsg"
	NotifyUserStatusMsgType = "NotifyUserStatusMsg"
	SmsMsgType              = "SmsMsg"
)

const (
	UserOnline  = "UserOnline"
	UserOffline = "UserOffline"
	UserBusy    = "UserBusy"
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

// 状态码 500 表示该用户未注册 200表示登录成功 400表示改用户已经占用, 登录成功后可以显示当先用户列表
type ResponseMsg struct {
	Code  int    `json:code`
	Error string `json:error` //返回的错误信息，如果没有则nil
	// Users []string `users`
}

type OnlineUsersMsg struct {
	Users []string `json:users`
}

type NotifyUserStatusMsg struct {
	UserId string `json:userId`
	Status string `json:status`
}

// 如果dstUserId = 11111111 则为广播
type SmsMsg struct {
	SrcUserId string `json:srcUserId`
	DstUserId string `json:dstuserId`
	Content   string `json:content`
}
