package model

type User struct {
	UserId     string `json:"userId"`
	UserPwd    string `json:"userPwd"`
	UserName   string `json:"userName"`
	UserStatus string `json:userStatus`
}
