//
// Created by Zilin Xiao on 2019/10/24.
//

#include "shared_ptr_test.h"
#include <iostream>

using std::cout, std::endl;

class Base{
    int ind;
public:
    explicit Base(int _ind){
        ind = _ind;
        cout<<ind<<" Constructed!"<<endl;
    }
    ~Base(){
        cout<<ind<<" Deconstructed!"<<endl;
    }
    void showIndex(){
        cout<<ind<<": Show!"<<endl;
    }
};
int main(){
    myToys::shared_ptr<Base> sp1(new Base(1));
    myToys::shared_ptr<Base> sp2(sp1); // sp2和sp1指向相同ControlBlock和Base
    cout<<"Now RefCnt:"<<sp1.use_count()<<endl;
    sp1->showIndex();
    sp1.reset(new Base(100));
    cout<<"Now RefCnt:"<<sp1.use_count()<<endl;
    auto sp3 = sp1;
    sp3->showIndex();
    cout<<"Now RefCnt:"<<sp1.use_count()<<endl;
    sp3.reset();
    sp2.reset();
    sp1.reset();
    cout<<"Now RefCnt:"<<sp1.use_count()<<endl;
    cout<<"Test End!"<<endl;
}