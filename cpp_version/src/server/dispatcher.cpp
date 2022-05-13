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
                LoginProcess* lp = new LoginProcess(tmp_epoll_recv_fd, sptr_redis);
                lp->processLoginMsg(data);
            }else if (data.type() == message::MSGTYPE::RegisterMsgType){
                RegisterProcess* rp = new RegisterProcess(tmp_epoll_recv_fd, sptr_redis);
                rp->processRegisterData(data);
            }else if (data.type() == message::MSGTYPE::SmsMsgType){
                SmsProcess* sm = new SmsProcess(tmp_epoll_recv_fd);
                sm->processSmsMsg(data);
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