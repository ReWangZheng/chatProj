#include "server.h"
#include "json.hpp"
#include <iostream>
#include "service.h"
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,
                    const InetAddress& listenAddr,
                    const string& nameArg):m_server(loop,listenAddr,nameArg)
{
    // 注册连接回调
    m_server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
    // 注册消息回调
    m_server.setMessageCallback(std::bind(&ChatServer::onMessageCallback,this,_1,_2,_3));
    // 设置线程名称
    m_server.setThreadNum(4);
}

void ChatServer::start() {
    m_server.start();
}

void ChatServer::onConnection(const TcpConnectionPtr& conn) {
    // 客户端断开连接
    if(!conn->connected()){
        conn->shutdown();
    }
}

void ChatServer::onMessageCallback(const TcpConnectionPtr& conn, Buffer* buffer, Timestamp t) {
    string buf = buffer->retrieveAllAsString();
    json js = json::parse(buf);
}
