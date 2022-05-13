package dispatcher

import (
	"commsys/common/message"
	"commsys/common/utils"
	"commsys/server/process"
	"fmt"
	"io"
	"net"
)

type Dispatcher struct {
	Conn net.Conn
}

func (p *Dispatcher) serverProcessMsg(msg *message.Message) (err error) {
	switch msg.Type {
	case message.LoginMsgType:
		// 处理登录信息
		// 创建一个userProcess实例
		up := &process.UserProcess{
			Conn: p.Conn,
		}
		err = up.ServerProcessLogin(msg)
	case message.RegisterMsgType:
		// 处理注册信息
		up := &process.UserProcess{
			Conn: p.Conn,
		}
		err = up.ServerProcessRegister(msg)
	case message.SmsMsgType:
		up := &process.UserProcess{
			Conn: p.Conn,
		}
		err = up.ServerProcessSms(msg)
	default:
		fmt.Println("消息类型不存在，无法处理。。。")
	}
	return
}

func (p *Dispatcher) Dispatch() (err error) {
	for {
		// 创建一个transfer实例完成读包
		tf := &utils.Transfer{
			Conn: p.Conn,
		}
		msg, err := tf.ReadPkg()
		if err != nil && err != io.EOF {
			fmt.Println("readPkg err = ", err)
			break
		} else if err == io.EOF {
			continue
		}
		p.serverProcessMsg(&msg)
	}
	return
}
