#include "../../include/loginProcess.h"

void LoginProcess::processLoginMsg(message::Message msg){
	bool ret2 = loginmsg.ParseFromString(msg.data());
	if (ret2){
		std::cout << loginmsg.userid() << "\t" << loginmsg.userpwd() << std::endl;
	}

	// 检测登录信息
	bool ret3 = checkLogin();
	// 构建transfer实体
	Transfer* tf = new Transfer(connfd);

	if (ret3){ // 账号密码正常
		std::string temp = LoginOkCons();
		// std::cout << temp.length();
		tf->sendMessage(message::ResponseMsgType, temp, temp.length());
	}else { // 整好密码有误
		std::string temp = LoginFailCons();
		tf->sendMessage(message::ResponseMsgType, temp, temp.length());
	}
}

bool LoginProcess::checkLogin(){
	return true;
}

std::string LoginProcess::LoginOkCons(){
	message::ResponseMsg resMsg;
	resMsg.set_code("200");
	resMsg.set_error("Login Success");
	std::string data;
	resMsg.SerializeToString(&data);
	return data;
}
std::string LoginProcess::LoginFailCons(){
	message::ResponseMsg resMsg;
	resMsg.set_code("400");
	resMsg.set_error("Login Fail");
	std::string data;
	resMsg.SerializeToString(&data);
	return data;
}