#include "db/credis.h"
#include <iostream>
#include <pthread.h>
#include <threads.h>
#include <initializer_list>
#include <pthread.h>
#include <functional>
#include "public.hpp"
using namespace std;
string redis_host = "127.0.0.1";
int redis_port = 6379;
Redis::Redis()
{
    this->connect();
}
Redis::~Redis()
{
    if(m_public_context!=nullptr){
        redisFree(m_public_context);
    }
    if(m_subscribe_context!=nullptr){
        redisFree(m_public_context);
    }
}

bool Redis::connect() {
    // 初始化订阅连接
    m_subscribe_context = redisConnect(redis_host.c_str(),redis_port);
    
    if(m_subscribe_context==nullptr){
        spdlog::critical("Redis订阅连接初始化失败");
        return false;
    }
    
    m_public_context = redisConnect(redis_host.c_str(),redis_port);
    
    if(m_public_context==nullptr){
        spdlog::critical("Redis发布连接初始化失败");
        return false;
    }
    // 创建完连接之后，然后再创建一个线程对通道上是的时间进行监听
    return true;
}

bool Redis::publish(string channel,string msg) {
    SPDLOG_DEBUG("{}:{}",channel,msg);
    redisReply * reply = (redisReply *)redisCommand(m_public_context,"publish %s %s",channel.c_str(),msg.c_str());
    if(nullptr == reply){
        spdlog::critical("发布消息失败!");
        return false;
    }else{
        freeReplyObject(reply);
        return true;
    }
}
void* observer(void * r){
    Redis * redis = (Redis *)r;
    redisReply * reply = nullptr;
    spdlog::info("订阅监听进程启动成功!");
    while(redisGetReply(redis->m_subscribe_context,(void**)&reply)==REDIS_OK){
            if(reply!=nullptr && reply->element[1]!=nullptr && reply->element[2]!=nullptr){
                // 给业务层上报消息
                redis->m_notify_handler(reply->element[2]->str);
            }
        }
    return nullptr;
}
    

bool Redis::subscribe(string channel) {
    SPDLOG_DEBUG("准备订阅:{}",channel);
    if(REDIS_ERR == redisAppendCommand(this->m_subscribe_context,"SUBSCRIBE %s",channel.c_str())){
        spdlog::critical("订阅失败");
        return false;
    }
    redisReply * reply = nullptr;
    if(redisGetReply(redis->m_subscribe_context,(void**)reply)==REDIS_OK){
        pthread_t id;
        if(pthread_create(&id,nullptr,observer,(void*)this)==0){
            pthread_detach(id);
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool Redis::unsubscribe(string channel) {
    return true;
}



void Redis::init_notify_handler(function<void(string)> fn) {
    this->m_notify_handler = fn;
}

int Redis::uniqueset(string key,string v) {
    redisReply * reply = (redisReply *)redisCommand(m_public_context,"SADD %s %s",key.c_str(),v.c_str());
    if(reply==nullptr){
        return -1;
    }
    return reply->integer;
}

bool Redis::insert(string key,string value){
    redisReply * reply = (redisReply * )redisCommand(m_public_context,"SET %s %s",key.c_str(),value.c_str());
    if(!reply){
        return false;
    }else if(strcmp(reply->str,"OK")){
        return true;
    }else{
        return false;
    }

}
string Redis::getKeyValue(string key){
    redisReply * reply = (redisReply * )redisCommand(m_public_context,"GET %s",key.c_str());
    if(!reply){
        return "";
    }else{
        return reply->str;
    }
}

int Redis::del(string key) {
    redisReply * reply = (redisReply * )redisCommand(m_public_context,"DEL %s",key.c_str());
    if(!reply){
        return -1;
    }else{
        return reply->integer;
    }
}

int Redis::exists(string key) {
    redisReply * reply = (redisReply * )redisCommand(m_public_context,"EXISTS %s",key.c_str());
    if(!reply){
        return -1;
    }else{
        return reply->integer;
    }

}

Redis * Redis::redis = nullptr;
Redis * Redis::instance() {
    if(Redis::redis==nullptr){
        Redis::redis = new Redis();
    }

    return Redis::redis;
}

