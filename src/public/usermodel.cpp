#include "server/db/usermodel.h"
#include "db.h"
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
