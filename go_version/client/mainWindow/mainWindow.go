package mainWindow

import (
	"commsys/client/loginWindow"
	"commsys/client/registerWindow"
	"fmt"
	"os"
)

type MainWindow struct {
	Key  int
	Loop bool
	Lw   *loginWindow.LoginWindow
	Rw   *registerWindow.RegisterWindow
}

func (mw *MainWindow) MainShow() {
	// 接收用户的选择
	// 判断是否还继续显示菜单
	// fmt.Println("here")
	for mw.Loop {
		fmt.Println("--------------------欢迎登陆多人聊天系统--------------------")
		fmt.Println("                    1. 登录聊天室")
		fmt.Println("                    2. 注册用户")
		fmt.Println("                    3. 退出系统")
		fmt.Println("                    请选择（1-3）")

		fmt.Scanf("%d\n", &mw.Key)
		switch mw.Key {
		case 1:
			fmt.Println("登录聊天室")
			mw.Lw = &loginWindow.LoginWindow{}
			mw.Loop = false
		case 2:
			fmt.Println("注册用户")
			mw.Rw = &registerWindow.RegisterWindow{}
			mw.Loop = false
		case 3:
			fmt.Println("退出系统")
			mw.Loop = false
			os.Exit(0)
		default:
			fmt.Println("你的输入有误，请重新输入")
		}
	}

	if mw.Key == 1 {
		// 用户登录
		mw.Lw.LoginShow()
	} else if mw.Key == 2 {
		// 用户注册
		mw.Rw.ShowMenu()
		// fmt.Println("进行注册判断的逻辑")
	}
}
