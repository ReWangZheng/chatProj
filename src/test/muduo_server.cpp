#include  "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include <iostream>
#include <functional>
using namespace muduo;
using namespace muduo::net;
using std::cout;
using std::endl;

class ChatServer
{
private:
    TcpServer m_server;
public:
    ChatServer(EventLoop *loop,const InetAddress addr,const string &name);
    ~ChatServer();
    void connect_handle(const TcpConnectionPtr&);
    void message_handle(const TcpConnectionPtr&,Buffer*,Timestamp t);
    void start();
};

ChatServer::ChatServer(EventLoop *loop,const InetAddress addr,const string &name):m_server(loop,addr,name)
{
    m_server.setConnectionCallback(std::bind(&ChatServer::connect_handle,this,_1));
    m_server.setMessageCallback(std::bind(&ChatServer::message_handle,this,_1,_2,_3));
    m_server.setThreadNum(10);
}

ChatServer::~ChatServer()
{
}

void ChatServer::connect_handle(const TcpConnectionPtr& conn) {
    if(conn->connected()){
        cout<< conn->peerAddress().toIpPort()<<"->"<<conn->localAddress().toIpPort();
    }
}

void ChatServer::message_handle(const TcpConnectionPtr& conn,Buffer* buffer,Timestamp t) {
    string buf = buffer->retrieveAllAsString();
    cout<<"recv data:"<<buf<<" time" <<t.toString()<<endl;
    conn->send(buf);
}

void ChatServer::start() {
    m_server.start();
}

int main(){
    EventLoop loop;
    int sum=10;
    for(int i=0;i<100;i++){
        sum+=i;
    }
    InetAddress addr("127.0.0.1",8088);
    ChatServer server(&loop,addr,"chatservere");
    server.start();
    loop.loop();
    return 0;
}