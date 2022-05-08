#include "../../include/loginSuccWindow.h"

void LoginSuccWindow::ShowMenu(){
    while (this->loop) {
	    std::cout << "--------------------恭喜登录成功--------------------" << std::endl;
		std::cout <<"             1. 显示在线用户列表" << std::endl;
		std::cout <<"             2. 发送消息" << std::endl;
		std::cout <<"             3. 信息列表" << std::endl;
		std::cout <<"             4. 退出系统" << std::endl;
		std::cout <<"             请选择（1-4）： " << std::endl;
        std::cin >> this->key;

		switch (this->key) {
		case 1:
			std::cout <<"显示在线用户列表"<< std::endl;
            break;
		case 2:
			std::cout <<"发送消息"<< std::endl;
            break;
		case 3:
			std::cout << "信息列表"<< std::endl;
            break;
		case 4:
			std::cout <<"退出系统"<< std::endl;
			this->loop = false;
            break;
		default:
			std::cout <<"输入错误，请选择（1-4）"<< std::endl;
		}
	}
}

void LoginSuccWindow::RecvMsgFromSrv(int fd){
    Transfer *tf = new Transfer(fd);
	while (this->loop){
		std::cout << "客户端正在等待读取服务器发送的消息" << std::endl;
        auto msg = tf->readMessage();
        std::cout << msg.data() << std::endl;
	}
	return;
}