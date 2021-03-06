#include "../include/server.h"
void Server::init(){
	redis = std::make_shared<Redis>("tcp://127.0.0.1:6379");
	dispatcher = new Dispatcher(redis);
	
	if((i_listenfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)	//建立socket套接字
	{
		printf("socket Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&st_sersock, 0, sizeof(st_sersock));
	st_sersock.sin_family = AF_INET;  //IPv4协议
	st_sersock.sin_addr.s_addr = htonl(INADDR_ANY);	//INADDR_ANY转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP，因为有些机子不止一块网卡，多网卡的情况下，这个就表示所有网卡ip地址的意思。
	st_sersock.sin_port = htons(IP_PORT);

	if(bind(i_listenfd,(struct sockaddr*)&st_sersock, sizeof(st_sersock)) < 0) //将套接字绑定IP和端口用于监听
	{
		printf("bind Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if(listen(i_listenfd, 20) < 0)	//设定可同时排队的客户端最大连接个数
	{
		printf("listen Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if((epfd = epoll_create(MAXSIZE)) < 0)	//创建epoll实例
	{
		printf("epoll_create Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(-1);
	}
	
	ev.events = EPOLLIN;
	ev.data.fd = i_listenfd;
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, i_listenfd, &ev) < 0)
	{
		printf("epoll_ctl Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(-1);
	}
	printf("======waiting for client's request======\n");
}

void Server::waitforconnect(){
	while(1)
	{
		if((nCounts = epoll_wait(epfd, events, MAXSIZE, -1)) < 0)
		{
			printf("epoll_ctl Error: %s (errno: %d)\n", strerror(errno), errno);
			exit(-1);
		}
		else if(nCounts == 0)
		{
			printf("time out, No data!\n");
		}
		else
		{
			for(int i = 0; i < nCounts; i++)
			{
				int tmp_epoll_recv_fd = events[i].data.fd;
				if(tmp_epoll_recv_fd == i_listenfd)	//有客户端连接请求
				{
					if((i_connfd = accept(i_listenfd, (struct sockaddr*)NULL, NULL)) < 0)	//阻塞等待客户端连接
					{
						printf("accept Error: %s (errno: %d)\n", strerror(errno), errno);
					//	continue;
					}	
					else
					{
						printf("Client[%d], welcome!\n", i_connfd);
					}
	
					ev.events = EPOLLIN;
					ev.data.fd = i_connfd;
					if(epoll_ctl(epfd, EPOLL_CTL_ADD, i_connfd, &ev) < 0)
					{
						printf("epoll_ctl Error: %s (errno: %d)\n", strerror(errno), errno);
						exit(-1);
					}
				}
				else	//若是已连接的客户端发来数据请求
				{
					// 处理客户端请求
					memset(msg, 0 ,sizeof(msg));
					if((nrecvSize = recv(tmp_epoll_recv_fd, msg, MAXSIZE, 0)) < 0)
					{
						if (errno != EAGAIN){
							printf("read Error: %s (errno: %d)\n", strerror(errno), errno);
						}
						continue;
					}
					else if( nrecvSize == 0)	//read返回0代表对方已close断开连接。
					{
						printf("client has disconnected!\n");
						epoll_ctl(epfd, EPOLL_CTL_DEL, tmp_epoll_recv_fd, NULL);
						close(tmp_epoll_recv_fd);  //
						continue;
					}
					else
					{
						dispatcher->dispatch(nrecvSize, msg, tmp_epoll_recv_fd);
					}
				}
			}
		}
	}//while
}

Server::~Server(){
	close(i_listenfd);
	close(epfd);
	delete(dispatcher);
}
