#include "db/db.h"
Mysql::Mysql()
{
    m_sqlconn = mysql_init(nullptr);
}

Mysql::~Mysql()
{
    mysql_close(m_sqlconn);
}

bool Mysql::connect() {
    MYSQL * p = mysql_real_connect(m_sqlconn,server.c_str(),
                                    user.c_str(),password.c_str(),dbname.c_str(),3306,nullptr,0);
    if(p!=nullptr){
        mysql_query(m_sqlconn,"set names gbk");
    }
    return p;
}
bool Mysql::update(string sql){
    if(mysql_query(m_sqlconn,sql.c_str())){
        return false;
    }
    return true;
}
MYSQL_RES *Mysql::querry(string sql) {
    if(mysql_query(m_sqlconn,sql.c_str())){
        return nullptr;
    }else{
        return mysql_use_result(m_sqlconn);
    }
}