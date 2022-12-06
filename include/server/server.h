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
            const string nameArg);
    void start();
    void setName(string n);
private:
    TcpServer  m_server;
    EventLoop * m_loop;
    void onConnection(const TcpConnectionPtr&);
    string m_servername;
    void onMessageCallback(const TcpConnectionPtr&,Buffer*,Timestamp);
    void onChannelCallback(string json);

    
};

#endif