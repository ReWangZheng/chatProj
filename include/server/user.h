#ifndef USER_H
#define USER_H
#include <string>
using namespace std;
// 匹配User表的ORM类
class User{
public:
    User(int id = -1,string name="",string password="",string state="offline"):id(id),name(name),password(password),state(state){

    };
    string getPassword() const { return password; }
    void setPassword(const string &password_) { password = password_; }
    string getName() const { return name; }
    void setName(const string &name_) { name = name_; }
    string getState() const { return state; }
    void setState(const string &state_) { state = state_; }
    int getId() const { return id; }
    void setId(int id_) { id = id_; }
private:
    int id;
    string name;
    string password;
    string state;
};

#endif