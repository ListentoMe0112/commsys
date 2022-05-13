#include "../../include/loginSuccWindow.h"

void LoginSuccWindow::ShowMenu(){
    while (this->loop) {
	    std::cout << "--------------------恭喜登录成功--------------------" << std::endl;
		std::cout <<"             1. 显示在线用户列表" << std::endl;
		std::cout <<"             2. 发送消息" << std::endl;
		std::cout <<"             3. 信息列表" << std::endl;
		std::cout <<"             4. 退出系统" << std::endl;
		std::cout <<"             请选择（1-4）： " << std::endl;
        std::cin >> this->key;

		switch (this->key) {
			case 1:{
				std::cout <<"显示在线用户列表"<< std::endl;
				for (auto p : MyOnlineUsers){
					std::cout << p.second->userid() << "\t" << p.second->userstatus() << std::endl;
				}
				break;
			}
			case 2:{
				std::cout <<"发送消息"<< std::endl;
				message::SmsMsg smsMsg;
				smsMsg.set_srcid(myUserid);
				smsMsg.set_dstid("11111111");
				std::string content;
				std::cin >> content;
				smsMsg.set_content(content);
				std::string senddata = smsMsg.SerializeAsString();
				tf->sendMessage(message::MSGTYPE::SmsMsgType, senddata, senddata.length());
				break;
			}
			case 3: {
				std::cout << "信息列表"<< std::endl;
				break;
			}
			case 4: {
				std::cout <<"退出系统"<< std::endl;
				this->loop = false;
				break;
			}
			default:{
				std::cout <<"输入错误，请选择（1-4）"<< std::endl;
			}
		}
	}
}

void LoginSuccWindow::RecvMsgFromSrv(int fd){
    Transfer *tf = new Transfer(fd);
	std::cout << "客户端正在等待读取服务器发送的消息" << std::endl;
	while (this->loop){
		
        auto msg = tf->readMessage();
		switch (msg.type()){
			case (message::MSGTYPE::AllOnlineUsersType) : {
				message::AllOnlineUsers msgtemp;
				msgtemp.ParseFromString(msg.data());
				for (int i = 0; i < msgtemp.userid_size(); i++){
					std::cout << i << "\t" << msgtemp.userid(i) << "\t" << "online" << std::endl;
					message::User user;
					user.set_userid(msgtemp.userid(i));
					user.set_userpwd("");
					user.set_username("");
					user.set_userstatus(message::USERSTATUS::Online);
					user.set_userconnfd(0);
					MyOnlineUsers[msgtemp.userid(i)] = std::make_shared<message::User>(user);
				}
				break;
			}
			case (message::MSGTYPE::NotifyMeOnlineType) : {
				message::NotifyMeOnline msgtemp;
				msgtemp.ParseFromString(msg.data());
				std::cout << msgtemp.userid() << "\t" << "online" << std::endl;
				message::User user;
				user.set_userid( msgtemp.userid() );
				user.set_userpwd("");
				user.set_username("");
				user.set_userstatus(message::USERSTATUS::Online);
				user.set_userconnfd(0);
				MyOnlineUsers[msgtemp.userid()] = std::make_shared<message::User>(user);
				break;
			}
			case (message::MSGTYPE::SmsMsgType) : {
				message::SmsMsg msgtemp;
				msgtemp.ParseFromString(msg.data());
				std::cout << "Recv from " << msgtemp.srcid() << " : " << msgtemp.content() << std::endl;
				break;
			}
			default : {
				std::cout << "Sorry, the type of msg is unkonwn but the data is belowed!" << std::endl;
				std::cout << msg.data() << std::endl;
			}
		}
        
	}
	delete tf;
	return;
}