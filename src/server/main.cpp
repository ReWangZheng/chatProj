#include <iostream>
#include "public.hpp"
#include <unistd.h>
#include <signal.h>
#include <functional>
#include "service.h"
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

void init_loger(){
    // spdlog::rotating_logger_mt("filelog","runtime_log",1024*1024*3,3);
    spdlog::set_pattern("%+ %s %#");
    spdlog::set_level(spdlog::level::debug);
}
void sigint_handler(int i){
    spdlog::info("正在关闭服务器");
    ChatService::instance()->closeServer();
    exit(0);
}

void init_signal(){
    // sigaction(SIGINT,NULL,NULL);
    struct sigaction sa;
    sa.sa_handler = sigint_handler; // 设置新的信号处理器函数
    // 将sa_mask设置为NULL
    sigemptyset(&sa.sa_mask);// sa_mask specifies a mask of signals which should be blocked during execution of the signal handler.
    if(0 != sigaction(SIGINT,&sa,NULL)){
        spdlog::get("message")->critical("绑定信号处理器失败");
        return;
    }
}
namespace ServerConfig{
    string servername;
    int port;
    string hosts;
};

int main(int argc,char** args){
    using namespace ServerConfig;
    
    servername= "chatserver";
    port= 8080;
    hosts= "127.0.0.1";

    if(argc>1){
        auto config = YAML::LoadFile(args[1]);
        servername= config["server"]["name"].as<string>();
        port= config["server"]["port"].as<int>();
        hosts= config["server"]["host"].as<string>();
    }
    init_loger();
    init_signal();
    EventLoop loop;
    InetAddress addr(hosts,port);
    ChatServer server(&loop,addr,servername);
    spdlog::info("server:{}  port:{}  address:{}",servername,port,hosts);
    server.start();
    loop.loop();
    return 0;
}