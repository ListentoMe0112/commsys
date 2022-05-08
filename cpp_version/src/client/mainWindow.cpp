
#include "../include/mainWindow.h"
#include <iostream>

void MainWindow::Show(){
    while (this->Loop) {
        std::cout <<"--------------------欢迎登陆多人聊天系统--------------------" << std::endl;
	    std::cout <<"                    1. 登录聊天室"<< std::endl;
	    std::cout <<"                    2. 注册用户"<< std::endl;
	    std::cout <<"                    3. 退出系统"<< std::endl;
	    std::cout <<"                    请选择（1-3）" << std::endl;
        std::cin >> this->Key;
        switch (this->Key) {
            case 1:
                std::cout << "登录聊天室" << std::endl;
                this->lw = new LoginWindow();
                this->Loop = false;
                break;
            case 2:
                std::cout << "注册用户" << std::endl;
                this->rw = new RegisterWindow();
                this->Loop = false;
                break;
            case 3:
                std::cout << "退出系统" << std::endl;
                this->Loop = false;
                exit(0);
            default:
                std::cout << "你的输入有误，请重新输入" << std::endl;
        }
    }

    if (this->Key == 1) {
    // 用户登录
	    this->lw->LoginShow();
    } else if (this->Key == 2) {
	    std::cout << "进行注册判断的逻辑" << std::endl;
        this->rw->ShowMenu();
	}
}