#include "message.pb.h"
#include <sw/redis++/redis++.h>
#include <memory>
class RegisterProcess {
    message::RegisterMsg registerMsg;
    std::shared_ptr<sw::redis::Redis> sptr_redis;
    int connfd;
public:
    RegisterProcess(int fd, std::shared_ptr<sw::redis::Redis> redis) : connfd(fd), sptr_redis(redis) {};

    void processRegisterData(message::Message);
};