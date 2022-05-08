#include "../../include/registerWindow.h"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <thread>


#define MAXSIZE 1024
#define IP_ADDR "127.0.0.1"
#define IP_PORT 8888

std::string RegisterDataSerialize(const std::string& userid, const std::string& passwd, const std::string& username){
	std::string registerdata;
	message::RegisterMsg registermsg;
	registermsg.set_userid(userid);
	registermsg.set_userpwd(passwd);
	registermsg.set_username(username);
	registermsg.SerializeToString(&registerdata);
	return registerdata;
};



void RegisterWindow::ShowMenu() {
	std::cout << "请输入用户的id" << std::endl;
    std::cin >> this->userid;
	std::cout <<"请输入用户的密码" << std::endl;
    std::cin >> this->passwd;
	std::cout << "请输入用户的用户名" << std::endl;
	std::cin >> this->username;

	bool ret = this->RegisterCheck(this->userid, this->passwd, this->username);
	if (!ret) {
        std::cout << "注册失败" << std::endl;
	} else {
		std::cout << "注册成功" << std::endl;
	}
}

RegisterWindow::~RegisterWindow(){
    if (this->tf != nullptr){
        delete this->tf;
    }
}

bool RegisterWindow::RegisterCheck(const std::string& userid, const std::string& passwd, const std::string& username){
    bool flag;
	struct sockaddr_in st_clnsock;
	int nrecvSize = 0;


	if((i_sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)	//建立套接字
	{
		printf("socket Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&st_clnsock, 0, sizeof(st_clnsock));
	st_clnsock.sin_family = AF_INET;  //IPv4协议
	//IP地址转换(直接可以从物理字节序的点分十进制 转换成网络字节序)
	if(inet_pton(AF_INET, IP_ADDR, &st_clnsock.sin_addr) <= 0)
	{
		printf("inet_pton Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	st_clnsock.sin_port = htons(IP_PORT);	//端口转换(物理字节序到网络字节序)

	if(connect(i_sockfd, (struct sockaddr*)&st_clnsock, sizeof(st_clnsock)) < 0)	//主动向设置的IP和端口号的服务端发出连接
	{
		printf("connect Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	printf("======connect to server, sent data======\n");

	// 传输登录信息
	tf = new Transfer(i_sockfd); 
	tf->sendMessage(message::MSGTYPE::RegisterMsgType, RegisterDataSerialize(userid, passwd, username), RegisterDataSerialize(userid, passwd, username).length());
    std::cout<< "[提示信息]" << " 已发送："<<RegisterDataSerialize(userid, passwd, username).length() << " 字节的消息"<< std::endl;

	// 等待客户端回复
	message::Message msg = tf->readMessage();
	message::ResponseMsg resMsg;
	bool ret2 = resMsg.ParseFromString(msg.data());
	if (ret2 && resMsg.code() == "200"){
		// std::cout << "登录成功" << std::endl;
		flag = true;
	}else{
		flag = false;
	}
	// printf("======recv finished======\n");
	
	google::protobuf::ShutdownProtobufLibrary();
    close(i_sockfd);
	return flag;
}

