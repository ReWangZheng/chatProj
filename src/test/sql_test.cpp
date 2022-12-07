// #include <iostream>
// #include "db/db.h"
// #include "db/usermodel.h"
// #include <hiredis/hiredis.h>
// #include "db/credis.h"
// #include <yaml-cpp/yaml.h>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include "usermodel.h"
#include "json.hpp"
using namespace nlohmann;
using namespace std;
int main(){
    UserModel model;
    spdlog::set_level(spdlog::level::debug);
    model.deleteMessage("admin");
    auto v = model.fetchMessage("admin");
    SPDLOG_DEBUG(v.size());
    for(json j:v){
        SPDLOG_DEBUG(j.dump());
    }
    return 0; 
}
// void redis_test();
// void redis_test2();
// void redis_test3();
// void redis_test4();
// void redis_test5();
// void redis_test6(){
//     auto r = Redis::instance();
//     redisAppendCommand(r->m_subscribe_context,"subscribe foo");
//     redisReply *rd;
//     redisGetReply(r->m_subscribe_context,(void**)&rd);
//     redisGetReply(r->m_subscribe_context,(void**)&rd);
//     redisGetReply(r->m_subscribe_context,(void**)&rd);
//     redisGetReply(r->m_subscribe_context,(void**)&rd);


// }
// int main(){
//     // auto node = YAML::LoadFile("/home/regan/code/chatProj/src/config/server.yaml");
//     // cout<<node["server"]["name"].as<string>()<<endl;
//     // cout<<node["server"]["host"].as<string>()<<endl;
//     // cout<<node["server"]["port"].as<string>()<<endl;
//     auto redis = Redis::instance();
//     redisReply * reply  =(redisReply *) redisCommand(redis->m_subscribe_context,"SUBSCRIBE server1");
//     freeReplyObject(reply);
//     cout<<"waiting message \n";
//     while(redisGetReply(redis->m_subscribe_context,(void **)&reply) == REDIS_OK) {
//         // consume message
//         cout<<reply->element[2]->str<<endl;
//         freeReplyObject(reply);
        
//     }

//     return 0;
// }
// typedef struct student{
//     int id;
//     char *name;
//     int score;
// }Student;
// void printStudent(Student &s){
//         cout<<s.id<<" "<<" "<<s.score<<endl;
// }
// void redis_test5(){
//     Redis * redis = Redis::instance();
//     int t=redis->uniqueset("u","b");
//     cout<<t<<endl;
// }


// void redis_test4(){
//     Student s;
//     s.id = 100;
//     s.score = 89;
//     Redis *redis = Redis::instance();
//     redis->set<Student>("s2",&s);
//     Student*sss = redis->get<Student>("s2");
//     printStudent(*sss);
// }

// int model_test(){
//     UserModel model;
//     User user;
//     // t1
//     // user.setName("admin");
//     // user.setPassword("admin");
//     // user.setState("offline");
//     // user.setId(13);

//     // bool isok=model.update(&user);
//     // assert(isok);

//     // t2
//     user.setName("admin");

//     User *u=model.querry(&user);
//     assert(u->getPassword()=="admin");

//     return 0;

// }
// void redis_test(){
//     redisContext * context = redisConnect("127.0.0.1",6379);
//     if(context==nullptr || context->err){
//         if(context){
//             cout<<"Error:"<<context->errstr<<"\n";
//         }else{
//             cout<<"无法分配redis context\n";
//         }
//     }else{
//         cout<<"连接分配成功!"<<endl;
//     }
//     void *reply = redisCommand(context, "SET %s %s", "v1", "c++");
//     cout <<(const char *)reply<<"???\n";
// }