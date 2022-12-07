#ifndef USERMODEL_H
#define USERMODEL_H
#include "../user.h"
#include "json.hpp"
using namespace nlohmann;
class UserModel
{
private:
    /* data */
public:
    UserModel();
    ~UserModel();
public:
    bool insert(User *user);
    User* querry(string name,string password);
    User* querry(User *user);
    bool update(User *user);
    bool save_message(json &js);
    vector<json> fetchMessage(string name);

    bool deleteMessage(string name);
}; 



#endif