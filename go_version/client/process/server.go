package process

import (
	"commsys/common/utils"
	"fmt"
	"net"
)

type LoginSuccWindow struct {
	key  int
	Loop bool
}

func (lsw *LoginSuccWindow) ShowMenu() {
	for lsw.Loop {
		fmt.Println("--------------------恭喜登录成功--------------------")
		fmt.Println("             1. 显示在线用户列表")
		fmt.Println("             2. 发送消息")
		fmt.Println("             3. 信息列表")
		fmt.Println("             4. 退出系统")
		fmt.Println("             请选择（1-4）： ")
		fmt.Scanf("%d\n", &lsw.key)

		switch lsw.key {
		case 1:
			fmt.Println("显示在线用户列表")

		case 2:
			fmt.Println("发送消息")

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
		fmt.Println("msg = %v", msg)
	}
}
