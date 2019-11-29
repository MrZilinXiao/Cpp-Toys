//
// Created by Zilin Xiao on 2019/11/29.
//

#include <iostream>
#include "vector.h"

int main(){
    mytoys::vector<int> v;
    for(int i = 0; i < 1001; ++i) {
        v.push_back(i);
    }
    for(auto it = v.begin(); it != v.end(); it++){
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}