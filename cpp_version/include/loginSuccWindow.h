#include <iostream>
#include "transfer.h"


class LoginSuccWindow{
public:
	int key;
	bool loop;
public:
	LoginSuccWindow():loop(true){}
	void ShowMenu();
	void RecvMsgFromSrv(int fd);
};