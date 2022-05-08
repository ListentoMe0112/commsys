#include "../include/loginWindow.h"
#include "../build/message.pb.h"

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



std::string LoginDataSerialize(const std::string& userid, const std::string& passwd){
	std::string logindata;
	message::LoginMsg loginmsg;
	loginmsg.set_userid(userid);
	loginmsg.set_userpwd(passwd);
	loginmsg.set_username("xieyibo0112");
	loginmsg.SerializeToString(&logindata);
	return logindata;
};


bool LoginWindow::LoginCheck(const std::string& userid, const std::string& passwd){
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
	tf->sendMessage(message::MSGTYPE::LoginMsgType, LoginDataSerialize(userid, passwd), LoginDataSerialize(userid, passwd).length());

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
	return flag;
}

void LoginWindow::LoginShow(){
    std::cout << "请输入用户的id" << std::endl;
	std::cin >> this->userid;
	std::cout <<"请输入用户的密码"<< std::endl;
	std::cin >> this->passwd;
	bool ret = this->LoginCheck(this->userid, this->passwd);
	
	if (!ret) {
		std::cout <<"登录失败" << std::endl;
		close(i_sockfd);
	} else {
		std::cout <<"登录成功" << std::endl;
		lsw = new LoginSuccWindow();
		std::thread t1 = std::thread(&LoginSuccWindow::RecvMsgFromSrv, lsw, i_sockfd);
		lsw->ShowMenu();
		t1.join();	
	}
	
}