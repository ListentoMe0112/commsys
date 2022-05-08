#include "../include/transfer.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

bool Transfer::sendMessage(const message::MSGTYPE& type,const std::string& data, const int len){
    std::string serializedata;
	message::Message sendmsg;
	sendmsg.set_data(data);
	sendmsg.set_type(type);
	sendmsg.set_len(len);
	sendmsg.SerializeToString(&serializedata);
    write(connfd, serializedata.c_str(), serializedata.length());
	return true;
}

message::Message Transfer::readMessage(){
	int nrecvSize;
	memset(buf, 0, MAXSIZE);
	while (true){
		if((nrecvSize = recv(connfd, buf, MAXSIZE, 0)) < 0)	//接受数据
		{
			if (errno != EAGAIN){
				printf("read Error: %s (errno: %d)\n", strerror(errno), errno);
			}else{
				continue;
			}
		} else if(nrecvSize == 0) {
				printf("Service Close!\n");
		}else{
			// printf("Server return: %s\n", buf);	
			std::string temp;
			// std::cout << nrecvSize << std::endl;
			temp.assign(buf, nrecvSize);
			message::Message msg;
			bool ret = msg.ParseFromString(temp);
			std::cout << ret << std::endl;
			if (ret && checkLength(&msg)){
				return msg;
			}else{
				return message::Message{};
			}
		}
	}
}


bool Transfer::checkLength(message::Message* msg){
    if (msg->len() == msg->data().length()){
        return true;
    }else{
        return false;
    }
}
