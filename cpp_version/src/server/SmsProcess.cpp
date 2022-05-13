#include "../../include/SmsProcess.h"

void SmsProcess::processSmsMsg(message::Message msg){
  bool ret = smsMsg.ParseFromString(msg.data());
  if (ret){
		for (auto p : LoginProcess::SrvUsersOnline){
      if (p.first != smsMsg.srcid()){
        tf->connfd = p.second->userconnfd();
        tf->sendMessage(message::MSGTYPE::SmsMsgType, msg.data(), msg.data().length());
      }
    }
	}
}