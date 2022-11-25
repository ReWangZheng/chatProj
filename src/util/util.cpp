#include <iostream>
#include <initializer_list>
int sum_list(std::initializer_list<int> l){
    int ret=0;
    for(int t:l){
        ret+=t;
    }
    std::cout<<"update...okokook\n";
    return ret;
}
int square_list(std::initializer_list<int> l){
    int ret=0;
    for(int t:l){
        ret+=t*t;
    }
    return ret;
}