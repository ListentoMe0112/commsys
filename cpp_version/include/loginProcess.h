#include "message.pb.h"
#include "transfer.h"
class LoginProcess{
private:
    message::LoginMsg loginmsg;
    int connfd;
public:
    LoginProcess(int fd) : connfd(fd){};
    void processLoginMsg(message::Message msg);
    bool checkLogin();
    std::string LoginOkCons();
    std::string LoginFailCons();
};