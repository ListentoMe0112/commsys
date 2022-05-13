package process

import (
	"fmt"
)

// 全局唯一，多处使用
var (
	MyUserMgr *UserMgr
)

type UserMgr struct {
	OnlineUsers map[string]*UserProcess
}

// 完成对userMgr初始化工作
func init() {
	MyUserMgr = &UserMgr{
		OnlineUsers: make(map[string]*UserProcess, 1024),
	}
}

func (um *UserMgr) AddOnlineUser(up *UserProcess) {
	um.OnlineUsers[up.UserId] = up
}

func (um *UserMgr) DelOnlineUser(userid string) {
	delete(um.OnlineUsers, userid)
}

func (um *UserMgr) GetAllOnlineUsers() map[string]*UserProcess {
	return um.OnlineUsers
}

func (um *UserMgr) GetOnlineUserById(userid string) (up *UserProcess, err error) {
	up, ok := um.OnlineUsers[userid]
	if !ok {
		err = fmt.Errorf("用户%s 不存在", userid)
		return
	} else {
		return
	}
}
