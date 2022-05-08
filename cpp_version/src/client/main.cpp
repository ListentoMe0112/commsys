#include <mainWindow.h>
#include <iostream>
int main(){
    std::cout << "Client Start" << std::endl;
    MainWindow* mw = new MainWindow(0, true);
    mw->Show();
    return 0;
}