#include "message.pb.h"
#include "loginProcess.h"
#include "registerProcess.h"
#include "SmsProcess.h"
#include <sw/redis++/redis++.h>
#include <string>



class Dispatcher {
private:
    std::string buf;
    message::Message data;
    std::shared_ptr<sw::redis::Redis> sptr_redis;
public:
    Dispatcher(std::shared_ptr<sw::redis::Redis> redis):sptr_redis(redis){};
    void dispatch(int nrecvSize, char* msg, int tmp_epoll_recv_fd);
    bool checkLength(message::Message* msg);
};