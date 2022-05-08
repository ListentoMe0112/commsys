#include "message.pb.h"
#include "loginProcess.h"
#include <string>



class Dispatcher {
private:
    std::string buf;
    message::Message data;
public:
    void dispatch(int nrecvSize, char* msg, int tmp_epoll_recv_fd);
    bool checkLength(message::Message* msg);
};