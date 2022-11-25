#include <iostream>
#include "server.h"
int main(){
    EventLoop loop;
    InetAddress addr("127.0.0.1",8080);
    ChatServer server(&loop,addr,"chatserver");
    server.start();
    loop.loop();
    return 0;
}