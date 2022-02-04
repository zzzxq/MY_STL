/*************************************************************************
	> File Name: my_construct.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Wed 19 Jan 2022 09:31:52 PM CST
 ************************************************************************/

#ifndef _MY_CONSTRUCT_H
#define _MY_CONSTRUCT_H

#include "./my_iterator.h"

template<class T1, class T2>
inline void construct(T1 *p, const T2& value) {
    new((void*)p) T1(value);
}

template<class T1>
inline void construct(T1 *p) {
    new((void*)p) T1();
}

template<class Tp>
inline void destroy(Tp *p) {
    p->~Tp();
}
//非pod类型，需要析构函数
template<class ForwardIterator>
void destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for (; first != last; ++first) {
        destroy(&*first);
    }
}

//对于pod类型，不需要析构函数
template<class ForwardIterator>
void destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}


template<class ForwardIterator, class Tp>
inline void destroy(ForwardIterator first, ForwardIterator last, Tp*) {
    typedef typename __type_traits<Tp>::has_trivial_destructor Trivial_destructor;
    //类型萃取
    destroy_aux(first, last, Trivial_destructor());
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    destroy(first, last, value_type(first)); //类型萃取
}



#endif
