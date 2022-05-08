// type RegisterWindow struct {
// 	tf       *utils.Transfer
// 	userid   string
// 	passwd   string
// 	use
#pragma once
#include "transfer.h"
#include <iostream>

class RegisterWindow{
    Transfer *tf;
    std::string userid;
    std::string passwd;
    std::string username;
    int i_sockfd;
public:
    void ShowMenu();
    bool RegisterCheck(const std::string& userid, const std::string& passwd, const std::string& username);
    ~RegisterWindow();
};