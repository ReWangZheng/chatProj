#ifndef CHATSERVER_H
#define CHATSERVER_H
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;
class ChatServer{
public:
    ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg);
    void start();

private:
     TcpServer  m_server;
     EventLoop * m_loop;
     void onConnection(const TcpConnectionPtr&);
     
     void onMessageCallback(const TcpConnectionPtr&,Buffer*,Timestamp);

};

#endif