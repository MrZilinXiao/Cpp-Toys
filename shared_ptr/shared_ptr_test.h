//
// Created by Zilin Xiao on 2019/10/24.
//

#ifndef CPP_TOYS_SHARED_PTR_TEST_H
#define CPP_TOYS_SHARED_PTR_TEST_H

#include <cstddef> // for size_t
#include <utility> // for swap

namespace myToys{
    class _ControlBlock{
    protected:
        size_t refCnt = 0;  // 唯一会导致线程不安全的因素
    public:
        _ControlBlock(size_t sz):refCnt(sz){}
        ~_ControlBlock(){}
        size_t getCnt(){
            return refCnt;
        };
        void dropCnt(){ // Q: 线程安全的 How to do?
            // 加锁？原子量？ Lock atomic

            if(refCnt > 0) refCnt -= 1;
        }
        void raiseCnt(){
            refCnt += 1;
        }
    };

    template <class T>
    class shared_ptr final{ // 有final 析构函数可以不加virtual
    protected:
        T *objPtr = nullptr;
        _ControlBlock *ctrlPtr;
        void clear(){
            ctrlPtr->dropCnt();
            if(ctrlPtr->getCnt() == 0){
                delete objPtr;
                delete ctrlPtr;
            }
            objPtr = nullptr;
            ctrlPtr = nullptr;
        }
        bool empty(){
            return objPtr == nullptr;
        }
    public:
        shared_ptr(): objPtr(nullptr), ctrlPtr(nullptr){}
        ~shared_ptr(){
            reset();
        }
        void reset() noexcept { // noexcept 标志不会有异常
            if(objPtr != nullptr){
                clear();
            }
        }
        void reset(T *newPtr){
            reset();
            if(newPtr != nullptr){
                auto tmp = new shared_ptr(newPtr);
                objPtr = tmp->objPtr;
                ctrlPtr = tmp->ctrlPtr;
            }
        }

        void swap(shared_ptr &ptr){
            std::swap(objPtr, ptr.objPtr);
        }
        explicit shared_ptr(T *t){ // 指向Obj，前提是没有其他shared_ptr指向Obj
            // explicit  避免隐式转换
            if(t != nullptr){
                objPtr = t;
                ctrlPtr = new _ControlBlock(1);
            }else{
                throw "Null Pointer Exception!";
            }
        }

        shared_ptr<T>(const shared_ptr<T> &r){ // copy constructor
            if(r.objPtr != nullptr){
                objPtr = r.objPtr;
                ctrlPtr = r.ctrlPtr;
                ctrlPtr->raiseCnt();
            }
        }

        shared_ptr<T>&operator=(const shared_ptr<T> &r){ //cppReference: 以 r 所管理者替换被管理对象
            if(&r == *this) return *this;
            clear();
            if(!r.empty()){
                objPtr = r.objPtr;
                ctrlPtr = r.ctrlPtr;
                ctrlPtr->raiseCnt();
            }
            return *this;
        }

        T *get() const{
            if(objPtr != nullptr) return objPtr;
            else return nullptr;
        }

        size_t use_count(){
            return ctrlPtr != nullptr ? ctrlPtr->getCnt():0;
        }

        bool unique(){ // C++ 20中移除 Why?
            return use_count() == 1;
        }

        // Operators overloads are as following

        T *operator->() const{
            return get();
        }

        T &operator*() const{
            return *get();
        }

        T &operator[](size_t ind){
            return *(get() + ind);
        }
        explicit operator bool() const{ // usage: if(XXX) 可直接用于bool判断条件
            return get() != nullptr;
        }
        bool owner_before(const shared_ptr<T> & other){
            //true if the object is considered to be different from x and go before it in a strict weak order based on ownership.
            //false otherwise.
            //return this < &other;
        }
    };
    template <typename T, typename... ArgsT>
    shared_ptr<T> make_shared(ArgsT &&... args){
        return shared_ptr<T>(new T(std::forward<ArgsT>(args)...));
    }

}

#endif //CPP_TOYS_SHARED_PTR_TEST_H
