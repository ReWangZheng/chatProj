#include "server.h"
#include "json.hpp"
#include <iostream>
#include "service.h"
#include "db/credis.h"
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string nameArg) : m_server(loop, listenAddr, nameArg)
{
    // 注册连接回调
    m_server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    // 注册消息回调
    m_server.setMessageCallback(std::bind(&ChatServer::onMessageCallback, this, _1, _2, _3));
    // 设置线程名称
    m_server.setThreadNum(4);
    this->setName(nameArg);
    Redis::instance()->init_notify_handler(bind(&ChatServer::onChannelCallback,this,_1));
    Redis::instance()->subscribe(nameArg);

}

void ChatServer::start()
{
    m_server.start();
    
}

void ChatServer::setName(string n) {
    this->m_servername = n;
}

void ChatServer::onConnection(const TcpConnectionPtr &conn)
{

    // 客户端断开连接
    if (!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

void ChatServer::onMessageCallback(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp t)
{

    string buf = buffer->retrieveAllAsString();
    SPDLOG_DEBUG("receive:{}",buf);
    try
    {
        json js = json::parse(buf);
        // 获取到msg处理器
        auto handler_ = ChatService::instance()->getHandler(js["msgid"].get<int>());
        // 然后对时间进行处理
        handler_(conn, js, t);
    }
    catch (exception e)
    {
        LOG_ERROR << e.what();
    }   
}

void ChatServer::onChannelCallback(string buf) {
    cout<<buf<<endl;
    json js = json::parse(buf);
    ChatService::instance()->subscribeHandler(js);
}
