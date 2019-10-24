//
// Created by Zilin Xiao on 2019/10/24.
//

#include "shared_ptr_test.h"
#include <iostream>
#include <thread>

std::mutex g_i_mutex;

using std::cout, std::endl;

myToys::shared_ptr<long> globalCnt = myToys::make_shared<long>(0);

void thread_test(){
    std::lock_guard<std::mutex> lock(g_i_mutex);
    for (int i = 0; i < 10000000; ++i) {
        *globalCnt = *globalCnt + 1;
    }
}

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
    std::thread thread1(thread_test);
    std::thread thread2(thread_test);
    thread1.join();
    thread2.join();

    cout<<"Global Count = "<<*globalCnt<<endl;

//    auto *ptr = new int[100];
//    myToys::shared_ptr<int> intsp1(ptr);
//    cout<<intsp1[10]<<endl;
    myToys::shared_ptr<Base> sp1(new Base(1));
    myToys::shared_ptr<Base> sp2(sp1); // sp2和sp1指向相同ControlBlock和Base
    cout<<"Now RefCnt:"<<sp1.use_count()<<endl;
    sp1->showIndex();
    sp1.reset(new Base(100));
    cout<<"Now RefCnt:"<<sp1.use_count()<<endl;
    auto sp3 = sp1;
    sp3->showIndex();
    // cout<<std::boolalpha<<sp3.owner_before(sp1)<<endl;
    cout<<"Now RefCnt:"<<sp1.use_count()<<endl;
    sp3.reset();
    sp2.reset();
    sp1.reset();
    cout<<"Now RefCnt:"<<sp1.use_count()<<endl;
    cout<<"Test End!"<<endl;
}