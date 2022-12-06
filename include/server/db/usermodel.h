#ifndef USERMODEL_H
#define USERMODEL_H
#include "../user.h"
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
};



#endif