#include "loginProcess.h"
#include "message.pb.h"
#include "transfer.h"
class SmsProcess {
private:
    message::SmsMsg smsMsg;
    Transfer* tf;
public:
    int connfd;
    SmsProcess(int fd) : connfd(fd) {
        tf = new Transfer(connfd);
    };
    void processSmsMsg(message::Message msg);
    ~SmsProcess(){
        if (tf != nullptr){
            delete tf;
        }
    }
};