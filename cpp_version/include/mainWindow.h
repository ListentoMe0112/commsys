// type MainWindow struct {
// 	Key  int
// 	Loop bool
// 	Lw   *loginWindow.LoginWindow
// }
#include "loginWindow.h"
#include "registerWindow.h"

class MainWindow {
public:
    int Key;
    bool Loop;
    LoginWindow* lw;
    RegisterWindow* rw;
public:
    MainWindow(int k, bool l) : Key(k), Loop(l){}
    void Show();
};