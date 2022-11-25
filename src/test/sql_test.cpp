#include <iostream>
#include <assert.h>
#include <functional>
#include <unordered_map>
#include <initializer_list>
using namespace std;
int add(int i,int j){
    return i+j;
}
int sub(int i,int j){
    return i-j;
}
int sum(initializer_list<int> l){
    int s = 0;
    for(int i:l){
        s+=i;
    }
    return s;
}
int  handle(function<int(int,int)> f,int i,int j){
    return f(i,j);
}
int sum_list(std::initializer_list<int> l);
int square_list(std::initializer_list<int> l);
int main(){
    function<int(int,int)> func = add;
    assert(func(1,2)==3);
    assert(handle(func,1,2)==3);

    unordered_map<int,function<int(int,int)>> tab;
    tab.insert({1,func});
    tab.insert({2,sub});
    assert(tab[1](1,2)==3);
    assert(tab[2](2,1)==1);
    assert(sum({1,2,3,4})==10);
    assert(sum_list({1,2,3})==6);
    assert(square_list({1,2,3})==14);
    return 0;
}