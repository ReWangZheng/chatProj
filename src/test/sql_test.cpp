#include <iostream>
#include "db/db.h"
using namespace std;

int main(){
    Mysql mysql;
    bool isok = mysql.connect();
    assert(isok);
    isok = mysql.update("update user set password=666666 where id=13");
    assert(isok);
    MYSQL_RES *res = mysql.querry("select * from user where id=13");
    MYSQL_ROW row = mysql_fetch_row(res);
    cout<<row[0]<<","<<row[1]<<","<<row[2]<<endl;
    mysql_free_result(res);
    return 0;
}