#include "../../include/registerProcess.h"


void RegisterProcess::processRegisterData(message::Message msg){
    bool ret = registerMsg.ParseFromString(msg.data());
    std::cout << registerMsg.userid() << "\t" << registerMsg.userpwd() << "\t" << registerMsg.username() << std::endl;
    sptr_redis->hset("users", registerMsg.userid(), msg.data());
    // std::string data = RegisterDataSerialize(userid, passwd, username);
}