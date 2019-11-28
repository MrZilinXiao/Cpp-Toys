//
// Created by Zilin Xiao on 2019/11/28.
//

#include <iostream>
#include "forward_list.h"


int main(){
    mytoys::forward_list<int> flist;
    for(int i = 0; i < 100; i++){
        flist.push_front(i);
    }
    mytoys::forward_list<int>::iterator it = flist.begin();
    for(; it != flist.end(); it++){
        std::cout<<*it<<std::endl;
    }
}