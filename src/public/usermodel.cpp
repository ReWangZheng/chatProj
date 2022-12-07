#include "server/db/usermodel.h"
#include "db.h"
#include "json.hpp"
#include "public.hpp"
using nlohmann::json;
UserModel::UserModel() {
    
}

UserModel::~UserModel() {
    
}

bool UserModel::insert(User* user) {
    char sql[512]={0};
    sprintf(sql,"insert into user(name,password,state) values('%s','%s','%s')",user->getName().c_str(),user->getPassword().c_str(),user->getState().c_str());
    Mysql mysql;
    if(mysql.connect()){
        mysql.update(sql);
        return true;
    }
    return false;
}

User* UserModel::querry(string name,string password) {
    char sql[512]={0};
    sprintf(sql,"select * from user where name='%s' and password='%s'",name.c_str(),password.c_str());

    Mysql mysql;
    if(mysql.connect()){
        MYSQL_RES *res = mysql.querry(sql);
        if(res!=nullptr){
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row==nullptr){
                return nullptr;
            }else{
                User *user = new User;
                user->setId(atoi(row[0])); // id;
                user->setName(row[1]);
                user->setPassword(row[2]);
                user->setState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }else{  
            return nullptr;
        }

    }
    return nullptr;

}

User* UserModel::querry(User *user) {
    char sql[512] = {0};
    sprintf(sql,"select * from user where name='%s'",user->getName().c_str());
    User *ret = user;
    Mysql mysql;
    if(mysql.connect()){
        MYSQL_RES * res = mysql.querry(sql);
        MYSQL_ROW row = mysql_fetch_row(res);
        ret->setId(atoi(row[0])); // id;
        ret->setName(row[1]);
        ret->setPassword(row[2]);
        ret->setState(row[3]);
        return ret;
    }
    return nullptr;
}

bool UserModel::update(User *user) {
    char sql[512]={0};
    cout<<sql<<"\n";
    sprintf(sql,"update user set state='%s',password='%s' where name='%s'",user->getState().c_str(),user->getPassword().c_str(),user->getName().c_str());
    Mysql mysql;
    if(mysql.connect()){
        if(mysql.update(sql)){
            return true;
        }else{
            return false;
        }
    }
    return false;
}

bool UserModel::save_message(json &js) {
    Mysql mysql;
    if(mysql.connect()){
        string to = js["to"];
        string content = js.dump();
        char sql[512];
        sprintf(sql,"insert into offlinemessage values('%s','%s')",to.c_str(),content.c_str());
        if(mysql.update(sql)){
            spdlog::info("离线信息存入数据库成功");
            return true;
        }else{
            spdlog::critical("离线信息存入数据库失败");
        }
    }else{
        spdlog::critical("数据库连接失败");
        return false;
    }
}

vector<json> UserModel::fetchMessage(string name) {
    Mysql mysql;
    vector<json> ret;
    if(mysql.connect()){
        char sql[512];
        sprintf(sql,"select message from offlinemessage where name='%s'",name.c_str());
        SPDLOG_DEBUG(sql);
        MYSQL_RES *res = mysql.querry(sql);
        if(res==nullptr){
            spdlog::warn("{} 没有任何离线消息",name);
            return ret;
        }
        MYSQL_ROW row = nullptr;
        while((row=mysql_fetch_row(res))!=nullptr){
            string mes = row[0]; 
            ret.push_back(json::parse(mes));
        }
        mysql_free_result(res);
        return ret;
    }else{
        spdlog::critical("数据库连接失败");
        return ret;
    }
}
bool UserModel::deleteMessage(string name) {
    Mysql mysql;
    vector<json> ret;
    if(mysql.connect()){
        char sql[512];
        sprintf(sql,"delete from offlinemessage where name='%s'",name.c_str());
        SPDLOG_DEBUG("》》》》{}",sql);
        if(mysql.update(sql)){
            spdlog::warn("{}的离线信息全部删除",name);
            return true;
        }else{
            return false;
        }
    }else{
        spdlog::critical("数据库连接失败");
        return false;
    }
}
