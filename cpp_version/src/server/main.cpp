#include "../../include/server.h"
int main(){
    Server* s = new Server();
    s->init();
    s->waitforconnect();
    delete s;
}