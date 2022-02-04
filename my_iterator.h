/*************************************************************************
	> File Name: my_iterator.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Thu 20 Jan 2022 02:58:10 PM CST
 ************************************************************************/

#ifndef _MY_ITERATOR_H
#define _MY_ITERATOR_H

#include "./my_construct.h"



//第一部分   iterator_traits
//根据迭代器的类型，选取不同的处理方式

//不同的迭代器类型，便于重载
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


//为了避免写代码遗漏 某个类别，自行开发的迭代器最好继承下面这个
template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category   iterator_category;
    typedef T          value_type;
    typedef Distance   difference_type;
    typedef Pointer    pointer;
    typedef Reference  reference;
};

//萃取迭代器所指的各种类别
template<class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category   iterator_category;
    typedef typename Iterator::value_type          value_type;
    typedef typename Iterator::difference_type     difference_type;
    typedef typename Iterator::pointer             pointer;
    typedef typename Iterator::reference           reference;
};

//原生指针的偏特化版
template<class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag     iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef T*                             pointer;
    typedef T&                             reference;
};
//const pointer
template<class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag     iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef const T*                       pointer;
    typedef const T&                       reference;
};

//获取迭代器的类型
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
    typedef typename iterator_traits<Iterator>::difference_type* distance;
    return static_cast<distance>(0);
}

template<class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
    typedef typename iterator_traits<Iterator>::value_type* value_type_p;
    return static_cast<value_type_p>(0);
}

//使用distence函数
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first; ++n;
    }
    return n;
}

template<class RandomAccessIterator>
inline
typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag){
    typename iterator_traits<RandomAccessIterator>::difference_type n = last - first;
    return n;
}

//通过不同的迭代器类型，类选择调用不同的__distance函数
template <class InputIterator>
inline 
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return __distance(first, last, category());
}



//迭代器相关的前进函数 BEGIN

template <class InputIterator, class Distance>
inline
void __advance(InputIterator& i, Distance n, input_iterator_tag) {
    while (n--) ++i;
}


template <class BidirectionalIterator, class Distance>
inline
void __advance(BidirectionalIterator& first, Distance n, bidirectional_iterator_tag) {
    if (n >= 0) {
        while (n--){
            ++first;
        }
    } else{
        while (n++){
            --first;
        }
    }
}

template <class RandomAccessIterator, class Distance>
inline
void __advance(RandomAccessIterator& first, Distance n) {
    while (n--){
        ++first;
    }
}

//通过iterator_category来判断该迭代器的类型，然后选择不同的__advance方法
template <class InputIterator, class Distance>
inline 
void advance(InputIterator& first,Distance distance) {
    __advance(first, distance, iterator_category(first));
}


//第二部分  type_traits 方式
//类型萃取，根据是否是pod，有无构造函数，析构函数 来选取处理数据的方式

struct __true_type{};
struct __false_type{};

template<class type>
struct __type_traits {
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;  
};  //默认都是false，代表有意义的

//针对特定版本 来处理各种类型
template<>
struct __type_traits<signed char>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<short>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned short>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<int>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned int>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned long>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<float>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<double>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long double>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<class T>
struct __type_traits<T*>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};


#endif
