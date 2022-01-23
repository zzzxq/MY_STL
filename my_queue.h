/*************************************************************************
	> File Name: my_queue.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Sun 23 Jan 2022 03:39:12 PM CST
 ************************************************************************/

#ifndef _MY_QUEUE_H
#define _MY_QUEUE_H

template <class T, class Sequence = deque<T> >
class queue {
public:
    typedef typename Sequence::value_type       value_type;
    typedef typename Sequence::size_type        size_type;
    typedef typename Sequence::reference        reference;
    typedef typename Sequence::const_reference  const_reference;
protected:
    Sequence  que;
public:
    bool empty() {return que.empty();}
    size_type size() {return que.size();}
    reference front() {return que.front();}
    const_reference front() const {return que.front();}
    reference back() {return que.back();}
    const_reference back() const {return que.back();}
    void push(const value_type& x) {return que.push_back(x);}
    void pop() {return que.pop_front();}

};

#endif
