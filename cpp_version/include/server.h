#include "message.pb.h"
#include "dispatcher.h"


#include <sw/redis++/redis++.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#include <ctype.h>
#include <sys/epoll.h>	//epoll头文件
#include <set>
#include <iostream>
#include <memory>

using namespace sw::redis;

#define MAXSIZE 1024
#define IP_ADDR "127.0.0.1"
#define IP_PORT 8888


class Server {
private:
    Dispatcher* dispatcher;
    int i_listenfd, i_connfd;
	struct sockaddr_in st_sersock;
	char msg[MAXSIZE];
	int nrecvSize = 0;
    struct epoll_event ev, events[MAXSIZE];
	int epfd, nCounts;	//epfd:epoll实例句柄, nCounts:epoll_wait返回值
    std::shared_ptr<Redis> redis;
    
    // Redis* redis;
    
public:
    void init();

    void waitforconnect();

    ~Server();
};