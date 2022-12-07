#include "service.h"
#include "public.hpp"
#include "user.h"
#include <muduo/base/Logging.h>
#include "db/credis.h"
using namespace muduo;
namespace ServerConfig{
    extern string servername;
    extern int port;
    extern string hosts;
};

ChatService::ChatService()
{
    // 初始化ChatService，将回调函数加入到事件处理表中
    this->m_handlertab.insert({LOGIN_MSG,std::bind(&ChatService::login,this,_1,_2,_3)});
    this->m_handlertab.insert({REG_MSG,std::bind(&ChatService::reg,this,_1,_2,_3)});
    this->m_handlertab.insert({ONECHAT_MSG,std::bind(&ChatService::chat,this,_1,_2,_3)});
}

ChatService* ChatService::instance() {
    static ChatService service;
    return &service;
} //

void ChatService::login(const TcpConnectionPtr& conn, json &js, Timestamp time) {
    string name = js["name"];
    string password = js["password"];
    User* user = m_usermodel.querry(name,password);
    json response;
    response["msgid"] = MSG_LOG_ACK;

    if(user==nullptr){
        response["error"] = 1;
        response["msg"] = err_tab[1];
    }else{
        if(user->getState()=="online"){
            response["error"] = 2;
            response["msg"] = err_tab[2];
            conn->send(response.dump());
            return;
        }
        user->setState("online");
        bool isok = m_usermodel.update(user);
        auto redis = Redis::instance();
        //将用户信息添加进redis中
        redis->insert(user->getName(),ServerConfig::servername);

        if(isok){
            response["error"] = 0;
            response["msg"] = err_tab[0];
            // 此处要注意线程安全
            m_connMutex.lock();
            m_userconnection.insert({user->getName(),conn});
            m_connMutex.unlock();
            vector<json> vjs = m_usermodel.fetchMessage(user->getName());
            conn->send(response.dump());
            for(auto v:vjs){
                this->subscribeHandler(v);
            }
            m_usermodel.deleteMessage(user->getName());
            return;
        }else{
            response["error"] = 3;
            response["msg"] = err_tab[3];
        }
    }
    conn->send(response.dump());
}//{"msgid":1,"name":"regan","password":"regan"}
// {"msgid":1,"name":"admin","password":"admin"}
void ChatService::reg(const TcpConnectionPtr& conn, json &js, Timestamp time) {
    string name = js["name"];
    string pwd = js["password"];
    User user;
    json response;
    user.setName(name);
    user.setPassword(pwd);
    bool state = m_usermodel.insert(&user);
    if(state){
        response["msgid"] = MSG_REG_ACK;
        response["error"] = 0;
        response["msg"] = err_tab[4];
    }else{
        response["msgid"] = MSG_REG_ACK;
        response["error"] = 5;
        response["msg"] = err_tab[5];
    }
    conn->send(response.dump());
}//{"msgid":2,"name":"regan","password":"regan"} |

void  ChatService::chat(const TcpConnectionPtr& conn,json &js,Timestamp time) {
    string from = js["from"];
    string to = js["to"];
    string content = js["content"];
    // 判断当前用户是否在线
    Redis *redis = Redis::instance();
    User user;
    user.setName(to);
    m_usermodel.querry(&user);
    // 首先判断user是否在线
    if(user.getState()=="online"){
        // 如果在线，在判断是否和用户处于同一个服务器
        string sn = redis->getKeyValue(user.getName());
        if(sn==ServerConfig::servername){
            TcpConnectionPtr to_conn = m_userconnection[to];
            to_conn->send(content);
        }else{
            redis->publish(sn,js.dump());
        }
    }else{
        // 如果没有在线的话，我们就把消息插入到数据库中
        m_usermodel.save_message(js);
    }
    
    
}//{"msgid":3,"from":"regan","to":"admin","content":"haodeb"} | {"msgid":3,"from":"admin","to":"regan","content":"I love you !!"}

void  ChatService::subscribeHandler(json &js) {
    string from = js["from"];
    string to = js["to"];
    string content = js["content"];
    TcpConnectionPtr ptr = m_userconnection[to];
    ptr->send(js.dump());
}

void ChatService::clientCloseException(const TcpConnectionPtr& conn) {
    User user;
    m_connMutex.lock();
    for(auto it = m_userconnection.begin();it!=m_userconnection.end();it++){
        if(it->second == conn){
            user.setName(it->first);
            m_userconnection.erase(it);
            auto redis = Redis::instance();
            redis->del(user.getName());
            break;
        }
    }
    m_connMutex.unlock();
    if(user.getName()==""){
        return;
    }
    m_usermodel.querry(&user);
    user.setState("offline");
    m_usermodel.update(&user);

}

 
MsgHandler ChatService::getHandler(int msgid) {
    auto it = m_handlertab.find(msgid);
    if(it == m_handlertab.end()){
        // 如果没有找到msg_id，就返回一个空处理器 
        return [=](const TcpConnectionPtr& conn, json &js, Timestamp time){
          };
    }else{
        return m_handlertab[msgid];
    }
}

void ChatService::closeServer() {
    for(auto it=m_userconnection.begin();it!=m_userconnection.end();it++){
        string username = it->first;
        User user;
        user.setName(username);
        m_usermodel.querry(&user);
        user.setState("offline");
        m_usermodel.update(&user);
        Redis::instance()->del(username);
    }
    spdlog::info("成功关闭所有连接");
}

// {"msgid":2,"name":"admin","password":"admin"}