#include "service.h"
#include "public.hpp"
#include <muduo/base/Logging.h>
using namespace muduo;
ChatService::ChatService()
{
    // 初始化ChatService，将回调函数加入到事件处理表中
    this->m_handlertab.insert({LOGIN_MSG,std::bind(&ChatService::login,this,_1,_2,_3)});
    this->m_handlertab.insert({REG_MSG,std::bind(&ChatService::reg,this,_1,_2,_3)});
}

ChatService* ChatService::instance() {
    static ChatService service;
    return &service;
} 

void ChatService::login(const TcpConnectionPtr& conn, json &js, Timestamp time) {
    LOG_INFO<<"login.....";
}

void ChatService::reg(const TcpConnectionPtr& conn, json &js, Timestamp time) {
    LOG_INFO<<"login......";
}
 
MsgHandler ChatService::getHandler(int msgid) {
    auto it = m_handlertab.find(msgid);
    if(it == m_handlertab.end()){
        // 如果没有找到msg_id，就返回一个空处理器 
        return [=](const TcpConnectionPtr& conn, json &js, Timestamp time){
            LOG_ERROR<<"can't find handler!";
        };
    }else{
        return m_handlertab[msgid];
    }
}
