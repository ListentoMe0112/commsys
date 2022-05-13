#include "../../include/loginProcess.h"

std::unordered_map<std::string, std::shared_ptr<message::User>> LoginProcess::SrvUsersOnline = {};

void LoginProcess::processLoginMsg(message::Message msg){
	bool ret2 = loginmsg.ParseFromString(msg.data());
	if (ret2){
		std::cout << loginmsg.userid() << "\t" << loginmsg.userpwd() << std::endl;
	}

	// 检测登录信息
	bool ret3 = checkLogin(loginmsg.userid(), loginmsg.userpwd());
	// 构建transfer实体
	Transfer* tf = new Transfer(connfd);

	if (ret3){ // 账号密码正常
		std::string temp = LoginOkCons();

		message::User user;
		user.set_userid(loginmsg.userid());
		user.set_userpwd(loginmsg.userpwd());
		user.set_username(loginmsg.username());
		user.set_userstatus(message::USERSTATUS::Online);
		user.set_userconnfd(connfd);

		SrvUsersOnline[loginmsg.userid()] = std::make_shared<message::User>(user);
		// std::cout << temp.length();
		tf->sendMessage(message::ResponseMsgType, temp, temp.length());
		temp = AllOnlineUsersCons();
		tf->sendMessage(message::AllOnlineUsersType, temp, temp.length());
		NotifyMeOnline();
	}else { // 整好密码有误
		std::string temp = LoginFailCons();
		tf->sendMessage(message::ResponseMsgType, temp, temp.length());
	}

	delete tf;
}

bool LoginProcess::checkLogin(std::string userid, std::string passwd){
	auto val = sptr_redis->hget("users", userid);
	
	message::RegisterMsg registerMsg;
	if (val){
		bool ret = registerMsg.ParseFromString(val.value());
		if (ret && registerMsg.userpwd() == passwd){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
	return false;
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

std::string LoginProcess::AllOnlineUsersCons(){
	message::AllOnlineUsers resMsg;
	for (auto p : SrvUsersOnline){
		resMsg.add_userid(p.first);
	}

	std::string data;
	resMsg.SerializeToString(&data);
	return data;
}

void LoginProcess::NotifyMeOnline(){
	Transfer* tf = new Transfer(connfd);
	std::string temp = NotifyMeOnlineCons();
	for (auto p : SrvUsersOnline){
		if (p.second->userconnfd() != this->connfd){
			tf->connfd = p.second->userconnfd();
			tf->sendMessage(message::MSGTYPE::NotifyMeOnlineType, temp, temp.length());
		}
	}
	delete tf;
}

std::string LoginProcess::NotifyMeOnlineCons(){
	message::NotifyMeOnline resMsg;
	resMsg.set_userid(loginmsg.userid());

	std::string data;
	resMsg.SerializeToString(&data);
	return data;
}
