#pragma once
#include "message.pb.h"
#include "transfer.h"
#include <sw/redis++/redis++.h>
#include <memory>
#include <unordered_map>
class LoginProcess{
private:
    message::LoginMsg loginmsg;
    std::shared_ptr<sw::redis::Redis> sptr_redis;
    int connfd;
    
public:
    static std::unordered_map<std::string, std::shared_ptr<message::User>> SrvUsersOnline;
    LoginProcess(int fd, std::shared_ptr<sw::redis::Redis> redis) : connfd(fd), sptr_redis(redis){};
    void processLoginMsg(message::Message msg);
    bool checkLogin(std::string userid, std::string passwd);
    void NotifyMeOnline();
    std::string LoginOkCons();
    std::string LoginFailCons();
    std::string AllOnlineUsersCons();
    std::string NotifyMeOnlineCons();
};



