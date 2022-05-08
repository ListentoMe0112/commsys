#include "../../include/dispatcher.h"


void Dispatcher::dispatch(int nrecvSize, char* msg, int tmp_epoll_recv_fd){
	buf.assign(msg, nrecvSize);
	bool ret = data.ParseFromString(buf);
	if (ret){
		std::cout << "Parse Success" << std::endl;
		std::cout << data.type() << std::endl;
		// 分发消息
        if (checkLength(&data)){
            if (data.type() == message::MSGTYPE::LoginMsgType){
                LoginProcess* lp = new LoginProcess(tmp_epoll_recv_fd);
                lp->processLoginMsg(data);
            }else if (data.type() == message::MSGTYPE::RegisterMsgType){
                message::RegisterMsg registerMsg;
                // buf.assign(data.data(), data.len());
                bool ret = registerMsg.ParseFromString(data.data());
                std::cout << registerMsg.userid() << "\t" << registerMsg.userpwd() << "\t" << registerMsg.username() << std::endl;
            }
        }else{
            std::cout << "Length Wrong" << std::endl;
        }
	}else{
		std::cout << "Parse Fail" << std::endl;
	}
}

bool Dispatcher::checkLength(message::Message* msg){
    if (msg->len() == msg->data().length()){
        return true;
    }else{
        return false;
    }
}