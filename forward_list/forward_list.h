//
// Created by Zilin Xiao on 2019/11/28.
//

// Reference: https://codereview.stackexchange.com/questions/155222/simple-forward-list
#pragma once
#ifndef CPP_TOYS_FORWARD_LIST_H
#define CPP_TOYS_FORWARD_LIST_H

#include <iterator>

namespace mytoys{
    template <typename T>
    class forward_list_node final{
    public:
        forward_list_node(const T& _ele, forward_list_node<T>* _next){
            ele = _ele;
            next = _next;
        }

        friend class forward_list<T, std::allocator<T> >;
    private:
        T ele;
        forward_list_node<T>* next;
    };

    template <typename _forward_list_t,
              typename _pointer,
              typename _reference>
    class forward_list_iter final : public std::iterator<
            std::forward_iterator_tag,
            typename _forward_list_t::value_type,
            typename _forward_list_t::difference_type,
            _pointer,
            _reference>{ // a wrapper for iterator&node
    public:

        using value_type = typename _forward_list_t::value_type;
        using difference_type = typename _forward_list_t::difference_type;
        using reference = typename _forward_list_t::reference;
        using const_reference = typename _forward_list_t::const_reference ;
        using pointer = typename _forward_list_t::pointer;
        using const_pointer = typename _forward_list_t::const_pointer;
        using iterator_category = std::forward_iterator_tag;

        using link_type = mytoys::forward_list_node<value_type>*;
        using iterator_base = forward_list_iter<_forward_list_t, _pointer, _reference>;

        // friend class forward_list<value_type, typename _forward_list_t::allocator_type>;

        reference operator*() const{
            return (*_p).ele;
        }

        forward_list_iter&operator++(){ // ++iter
            _p = (*_p).next;
            return *this;
        }

        forward_list_iter operator++(int){ // iter++
            forward_list_iter _tmp = *this;
            return ++(*this);
        }

        bool operator==(const iterator_base& rhs) const{
            return _p == rhs._p;
        }

        bool operator!=(const iterator_base& rhs) const{
            return _p != rhs._p;
        }


    private:
        link_type _p;  // pointer
        explicit forward_list_iter(const link_type&p): _p(p){

        }
    };


    template <typename T, typename _Alloc = std::allocator<T> >  // No Allocator Implemented
    class forward_list final{
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
        // custom alias

        using listType = forward_list<T, _Alloc>;
        using iterator = forward_list_iter<listType, pointer, reference>;
        using const_iterator = forward_list_iter<listType, pointer, const_reference>;
        using listNode = forward_list_node<value_type>;
        using node_pointer = forward_list_node<value_type>*;

        explicit forward_list(const alloc_type& alloc = alloc_type()){
            _alloc = alloc;
            init();
        }
        ~forward_list(){
            delete _head;
        }
        bool empty() const noexcept{
            return _sz == 0;
        }
        size_type size() const noexcept{
            return _sz;
        }
        alloc_type get_allocator(){
            return _alloc;
        }

        iterator make_iter(node_pointer &node){
            return iterator(node);
        }

        const_iterator make_iter(const node_pointer &node){
            return const_iterator(node);
        }

        iterator begin(){
            return make_iter(_head);
        }
        const_iterator cbegin(){
            return make_iter(const_cast<node_pointer>( _head));
        }

        iterator end(){
            return make_iter(_tail);
        }
        const_iterator cend(){
            return make_iter(const_cast<node_pointer>( _tail));
        }

        value_type front() const noexcept {
            return (*_head).ele;
        }

        // 修改器

        void pop_front(){
            if(_sz != 0) {
                auto tmp = _head->next;
                _head->next = _head->next->next;
                _sz -= 1;
                delete tmp;
            }
        }

        void clear(){
            while(_sz != 0){
                pop_front();
            }
        }

        iterator insert_after(const_iterator pos, const T& value){
            auto tmp = new listNode(value, (pos._p)->next);
            if(pos._p->next == nullptr){ // if inserted into last position, change the tail
                _tail = tmp;
            }
            pos._p->next = tmp;
            _sz += 1;
            return make_iter(tmp);
        }

//在容器中的指定位置后插入新元素。原位构造元素，即不进行复制或移动操作。
// 准确地以与提供给函数者相同的参数调用元素的构造函数。

//        template< class... Args >
//        iterator emplace_after( const_iterator pos, Args&&... args ){
//
//        };
        iterator erase_after(const_iterator pos){
            auto curPtr = pos._p->next;
            while(curPtr != nullptr){
                auto delPtr = curPtr;
                curPtr = curPtr->next;
                delete delPtr;
                _sz -= 1;
            }
        }

        void push_front(const T& value){
            insert_after(cbegin(), value);
        }


        void reverse() noexcept{
            if(_sz != 0){
                auto prev = nullptr, curr = _head->next, next = _head->next->next;
                while(next != nullptr){
                    curr->next = prev;
                    prev = curr;
                    curr = next;
                    next = next->next;
                }
                curr->next = prev;
                _head->next = curr;
            }
        }


    private:
        size_type _sz;
        alloc_type _alloc;
        node_pointer _head;
        node_pointer _tail;

        void init(){
            _head = CreateNode(value_type{}); // empty head node
            _tail = _head;
            _sz = 0;
        }

        node_pointer CreateNode(const_reference ele){
            auto newEle = _alloc.allocate(1);
            _alloc.construct(newEle, ele);

            auto newNode = new forward_list_node<T>(ele, nullptr);
            return newNode;
        }
    };


}
#endif //CPP_TOYS_FORWARD_LIST_H
