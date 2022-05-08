package main

import (
	_ "commsys/common/utils"
	"commsys/server/dispatcher"
	"commsys/server/model"
	_ "encoding/json"
	_ "errors"
	"fmt"
	_ "io"
	"net"
	"os"
)

func RedisInit() {
	model.MyUserDao = model.NewUserDao()
}

func Listen(protocal string, ipport string) net.Listener {
	listen, err := net.Listen(protocal, ipport)
	if err != nil {
		fmt.Println("Listen err = ", err)
		os.Exit(0)
	}
	return listen
}

func Accept(listener net.Listener) net.Conn {
	conn, err := listener.Accept()
	if err != nil {
		fmt.Println("Accept err = ", err)
		return nil
	}
	return conn
}

func Process(conn net.Conn) {
	// 延时关闭
	defer conn.Close()
	// 循环读客户端发的信息
	// 创建一个分发器
	dispatch := &dispatcher.Dispatcher{
		Conn: conn,
	}
	err := dispatch.Dispatch()
	if err != nil {
		fmt.Println("客户端与服务器端通讯携程错误err = ", err)
	}
}

func main() {
	RedisInit()
	fmt.Println("服务器在8889端口监听。。。")
	listener := Listen("tcp", "0.0.0.0:8889")
	defer listener.Close()

	for {
		fmt.Println("等待客户端链接服务器。。。")
		conn := Accept(listener)
		if conn != nil {
			go Process(conn)
		}
	}
}
