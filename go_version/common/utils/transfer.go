package utils

import (
	"commsys/common/message"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net"
)

type Transfer struct {
	// 分析它应该有哪些字段
	Conn net.Conn // 管理连接
	Buf  []byte   // 缓冲
}

func (trans *Transfer) ReadPkg() (mes message.Message, err error) {
	trans.Buf = make([]byte, 4096)
	n, err := trans.Conn.Read(trans.Buf)
	if err != nil && err != io.EOF {
		fmt.Println("conn.Read err = ", err)
		err = errors.New("read pkg header error")
		return
	} else if err == io.EOF {
		return
	}

	fmt.Printf("读到了数据%d字节\n", n)

	var msg message.Message

	err = json.Unmarshal(trans.Buf[:n], &msg)

	if err != nil {
		fmt.Println("json unmarhsal err = ", err)
		err = errors.New("json unmarhsal err")
		return
	}

	// 接收准确无误（长度校验）
	if msg.Length != len(msg.Data) {
		fmt.Println("接收数据长度错误")
		err = errors.New("接收数据长度错误")
		return
	}

	return msg, nil
}

func (trans *Transfer) WritePkg(mes message.Message) (err error) {
	sendData, err := json.Marshal(mes)
	if err != nil {
		fmt.Println("marshal err = ", err)
	}
	trans.Conn.Write(sendData)
	return
}
