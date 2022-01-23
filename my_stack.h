/*************************************************************************
	> File Name: my_stack.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Sun 23 Jan 2022 03:35:16 PM CST
 ************************************************************************/

#ifndef _MY_STACK_H
#define _MY_STACK_H

template<class T, class Sequence = deque<T>>
class stack {
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_Type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c; //底层容器，一般用deque， list也行
public:
    bool empty() const { return c.empty(); }
    size_type size() {return que.size();}
    reference top() {return que.back();}
    const_reference top() const {return que.back();}
    void push(const value_type& x) {
        que.push_back(x);
        return ;
    }
    void pop() {
        que.pop_back();
        return;
    }
};

#endif
