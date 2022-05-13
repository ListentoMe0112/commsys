#include <string>
#include "message.pb.h"
#pragma once

class Transfer {
private:
    
    char buf[4096];
    static const int MAXSIZE = 4096;

private:
    bool checkLength(message::Message* msg);

public:
    int connfd;
public:
    Transfer(int fd) : connfd(fd) {};
    bool sendMessage(const message::MSGTYPE& type,const std::string& data, const int len);
    message::Message readMessage();
    
};