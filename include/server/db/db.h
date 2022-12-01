#ifndef DB_H
#define DB_H 
#include <mysql/mysql.h>
#include <string>
#include <muduo/base/Logging.h>
#include <iostream>
using namespace std;
using std::string;
static string server = "10.211.55.2";
static string user = "root";
static string password = "123456";
static string dbname ="chatdb";
class Mysql{
private:
    MYSQL * m_sqlconn;
public:
    Mysql();
    ~Mysql();
    
    bool connect();
    bool update(string sql);
    MYSQL_RES *querry(string sql);
    

};

#endif  
