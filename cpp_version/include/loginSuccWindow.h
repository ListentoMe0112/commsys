#include <iostream>
#include "transfer.h"
#include <unordered_map> 


class LoginSuccWindow{
public:
	int key;
	bool loop;
	std::string myUserid;
	std::unordered_map<std::string, std::shared_ptr<message::User>> MyOnlineUsers;
	Transfer* tf;
public:
	LoginSuccWindow(std::string userid, int fd):loop(true), myUserid(userid){
		tf = new Transfer(fd);
	}
	void ShowMenu();

	~LoginSuccWindow(){
		if (tf != nullptr){
			delete tf;
		}
	}
	void RecvMsgFromSrv(int fd);
	
};