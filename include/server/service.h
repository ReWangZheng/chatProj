#ifndef SERVICE_H
#define SERVICE_H
#include <unordered_map>
#include <functional>
#include "json.hpp"
#include <muduo/net/TcpConnection.h>
#include "public.hpp" 
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
    MsgHandler getHandler(int msgid);
private:  
    unordered_map<int,MsgHandler> m_handlertab;

 
};
#endif