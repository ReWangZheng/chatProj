#ifndef SERVICE_H
#define SERVICE_H
#include <unordered_map>
#include <functional>
#include "json.hpp"
#include <muduo/net/TcpConnection.h>
#include "public.hpp"
#include "usermodel.h"
#include <mutex>
using json = nlohmann::json;
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

using MsgHandler  = std::function<void(const TcpConnectionPtr&,json &js,Timestamp)>; 
class ChatService{
public:
    ChatService();
public:
    static ChatService* instance();
    void  login(const TcpConnectionPtr& conn,json &js,Timestamp time);
    void  reg(const TcpConnectionPtr& conn,json &js,Timestamp time);
    void  chat(const TcpConnectionPtr& conn,json &js,Timestamp time);
    void  subscribeHandler(json &js);
    void clientCloseException(const TcpConnectionPtr& conn);
    MsgHandler getHandler(int msgid);
    void closeServer();
private:  
    unordered_map<int,MsgHandler> m_handlertab;
    // 该成员一定要注意线程安全,STL 容器没有线程安全
    unordered_map<string,TcpConnectionPtr> m_userconnection;
    // 数据库操作类
    UserModel m_usermodel;
    mutex m_connMutex;
};
#endif