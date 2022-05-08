#include <string>
#include "loginSuccWindow.h"


class LoginWindow{
    std::string userid;
    std::string passwd;
    Transfer* tf;
    LoginSuccWindow* lsw;
    int i_sockfd;
public:
    LoginWindow() : userid(""), passwd("") {};
    bool LoginCheck(const std::string& userid, const std::string& passwd);
    void LoginShow();
};

