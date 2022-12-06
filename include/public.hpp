#ifndef PUBLIC_H
#define PUBLIC_H
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <unordered_map>
#include <string>
#include <spdlog/spdlog.h>
#include "db/credis.h"
#include "db/db.h"
#include "server.h"
#include "service.h"

using namespace std;


enum EnMsgType{
    LOGIN_MSG = 1, // 登录消息
    REG_MSG, // 注册消息
    ONECHAT_MSG,
};
enum EnMsgResponseType{
    MSG_REG_ACK = 1, //注册响应消息 
    MSG_LOG_ACK
};
extern std::unordered_map<int,string> err_tab;
#endif