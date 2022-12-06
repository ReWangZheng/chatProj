#ifndef CREDIS_H
#define CREDIS_H
#include <functional>
#include <string>
#include <hiredis/hiredis.h>
#include <memory.h>
#include <iostream>
#include <initializer_list>
// 封装redis操作
using namespace std;
class Redis{
private:
    Redis();
    ~Redis();
    /**
     * @brief 用来初始化一个context的
     * 
     * @return true :如果初始化连接成功
     * @return false : 如果初始化连接失败
     */
    bool connect();

public:
    bool publish(string channel,string msg);
    // 消息队列中间件
    bool subscribe(string channel);
    bool unsubscribe(string channel);
    void init_notify_handler(function<void(string)> fn);

    bool insert(string key,string value);
    string getKeyValue(string key);
    int del(string key);
    int exists(string key);

    /**
     * @brief :可以将任意类型的对象放入redis中
     * 
     * @tparam T :指定存放对象的类型
     * @param obj ：指定要存放的对象
     * @return true  存放成功即返回true
     * @return false 存放失败返回fasle
     */
    template<typename T>
    bool set(string,T * obj);
    /**
     * @brief 取出redis中的对象
     * 
     * @tparam T :需要取出对象的类型
     * @param key 对象对应的键
     * @return T* ：返回对象的内存地址
     */
    template<typename T>
    T * get(string key);

    // bool list(string key,initializer_list<string> &l);
    // Set操作
    /**
     * @brief 集合操作，对某个集合添加值
     * 
     * @param key 集合名字
     * @param v  添加进的集合值
     * @return int 添加成功返回1，否则返回-1
     */
    int uniqueset(string key,string v);
public:
    redisContext * m_public_context;
    redisContext * m_subscribe_context;
    function<void(string)> m_notify_handler;
public:
    static Redis * instance();
private:
    static Redis * redis;
};


template<typename T>
bool Redis::set(string key,T *obj) {
    redisReply * reply = (redisReply * )redisCommand(m_public_context,"SET %s %b",key.c_str(),obj,sizeof(T));
    if(strcmp(reply->str,"OK")){
        return true;
    }else{
        return false;
    }
}
template<typename T>
inline T * Redis::get(string key) {
    redisReply * reply =(redisReply * ) redisCommand(m_public_context,"GET %s",key.c_str());
    if(reply==nullptr){
        return nullptr;
    }else{
        T* ret = (T*)malloc(sizeof(T));
        memcpy(ret,reply->str,sizeof(T));
        freeReplyObject(reply);
        return ret;
    }
}
#endif