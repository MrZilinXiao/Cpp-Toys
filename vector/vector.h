//
// Created by Zilin Xiao on 2019/11/29.
//
// Reference: https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html
#pragma once
#ifndef CPP_TOYS_VECTOR_H
#define CPP_TOYS_VECTOR_H

#include <iterator>
#include <iostream>

namespace mytoys{
    template <typename _vector_t,
            typename _pointer,
            typename _reference>
    class vector_iter final : public std::iterator<
            std::forward_iterator_tag,
            typename _vector_t::value_type,
            typename _vector_t::difference_type,
            _pointer,
            _reference>{ // a wrapper for iterator
    public:
        using value_type = typename _vector_t::value_type;
        using difference_type = typename _vector_t::difference_type;
        using reference = typename _vector_t::reference;
        using const_reference = typename _vector_t::const_reference ;
        using pointer = typename _vector_t::pointer;
        using const_pointer = typename _vector_t::const_pointer;
        using iterator_category = std::forward_iterator_tag;

        using iterator_base = vector_iter<_vector_t, _pointer, _reference>;

        reference operator*() const{
            return *_v;
        }

        vector_iter&operator++(){ // ++iter
            _v = _v + sizeof(value_type);
            return *this;
        }

        vector_iter&operator--(int){ // iter--
            _v = _v - sizeof(value_type);
            return *this;
        }

        vector_iter&operator++(int){ // iter++
            auto tmp = _v;
            _v = _v + sizeof(value_type);
            return iterator_base(tmp);
        }

        bool operator==(const iterator_base& rhs) const noexcept {
            return (*_v) == (*rhs);
        }

        bool operator!=(const iterator_base& rhs) const noexcept {
            return (*_v) != (*rhs);
        }

        //private:
        pointer _v; // a pointer
        explicit vector_iter(const pointer* v):_v(v){

        }


    };

    template<class T,
            class _Alloc = std::allocator<T>
    > class vector{
    public:
        // some alias below, define some types from Allocator
        using alloc_type = _Alloc;
        using value_type = typename _Alloc::value_type;
        using size_type = typename _Alloc::size_type; // usually size_t
        using difference_type = typename _Alloc::size_type;
        using reference = typename _Alloc::reference;
        using const_reference = typename _Alloc::const_reference;
        using pointer = typename _Alloc::pointer;
        using const_pointer = typename _Alloc::const_pointer;

        using vectorType = vector<T, _Alloc>;
        using iterator = vector_iter<vectorType, pointer, reference>;
        using const_iterator = vector_iter<vectorType, pointer, const_reference>;

        explicit vector(const alloc_type& Alloc = alloc_type()){
            _alloc = Alloc;
            init();
        }

        explicit vector(size_type max_sz, const alloc_type& Alloc = alloc_type()){
            _alloc = Alloc;
            _max_sz = max_sz;
            init();
        }

        iterator make_iter(value_type* v){
            return iterator(v);
        }

        const_iterator make_iter(const value_type* v){
            return iterator(v);
        }

        iterator begin(){
            if(!empty()) return make_iter(&_ele[0]);
            else throw "Empty Vector!";
        }

        const_iterator cbegin(){
            if(!empty()) return make_iter(const_cast<value_type*>(&_ele[0]));
            else throw "Empty Vector!";
        }

        iterator end(){
            return make_iter(&_ele[_sz]);  //TODO:Not SAFE
        }

        const_iterator cend(){
            return make_iter(const_cast<value_type*>(&_ele[_sz]));  //TODO:Not SAFE
        }

        size_type size() const noexcept {
            return _sz;
        }

        bool empty() const noexcept {
            return _sz == 0;
        }

        void reverse(size_type new_cap){
            // 若 new_cap > max_size() 则为 std::length_error 。
            if(new_cap > capacity()){
                reallocate(new_cap);
            }
        }

        size_type capacity() const noexcept {
            return _max_sz;
        }

        void shrink_to_fit(){
            reallocate(_sz);
        }

        value_type& at(size_type idx){
            if(idx < _sz) return _ele[idx];
            else throw "Out of boundary!";
        }

        value_type& operator[](size_type idx){
            return at(idx);
        }

        value_type& front(){
            if(!empty()) return _ele[0];
            else throw "Empty Vector!";
        }

        value_type& back(){
            if(!empty()) return _ele[_sz-1];
            else throw "Empty Vector!";
        }

        value_type*& data(){
            return _ele;
        }

        void clear() noexcept { // 请保持capacity不变
            delete []_ele;
            _ele = new value_type[_max_sz];
        }

        iterator insert(iterator pos, const value_type& value){ // Insert in the front of pos
            if(_sz + 1 > _max_sz) {
                reallocate(_max_sz + _step);
            }
            for(auto it = end(); it != pos; it--){
                *it = *(it._v - sizeof(value_type));
            }
            *pos = value; // value -- ref type   *pos -- ref type
            return pos;
        }

        iterator push_back(const value_type& value){
            if(_sz + 1 > _max_sz){
                reallocate(_max_sz + _step);
            }
            _ele[_sz] = value;
            return make_iter(&_ele[_sz]);
        }

        iterator pop_back(){
            _sz--;
            return make_iter(&_ele[_sz - 1]);
        }

//        size_type max_size() const noexcept {
//            //TODO: How to?
//        }

        // private:
        size_type _sz;
        size_type _max_sz = 100;
        size_type _step = 100;
        alloc_type _alloc;
        value_type* _ele;


        void init(){
            _ele = new value_type[_max_sz];
            _sz = 0;
            std::cout<<"Init a vector, maxSize = "<<_max_sz<<std::endl;
        }

        void reallocate(size_type new_max_sz){
            auto new_ele = new value_type[new_max_sz];
            for(int i = 0; i < _sz; i++){
                new_ele[i] = _ele[i];
            }
            delete []_ele;
            _ele = new_ele;
            std::cout<<"Inflate a vector, from "<<_max_sz<<" to "<<new_max_sz<<std::endl;
            _max_sz = new_max_sz;
        }
    };
}

#endif //CPP_TOYS_VECTOR_H
