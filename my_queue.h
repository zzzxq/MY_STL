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

template <class T, class Sequence = vector<T>,
         class Compare = less<typename Sequence::value_type> >
class priority_queue {
public:
    typedef typename Sequence::value_type   value_type;
    typedef typename Sequence::size_type    size_type;
    typedef typename Sequence::reference    reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence vec;
    Compare  cmp;
public:
    priority_queue() : vec() {}
    explicit priority_queue(const Compare& cmp) : vec(), cmp(cmp) {}

public:
    bool empty() {return vec.empty();}
    size_type size() const {return vec.size();}
    const_reference top() {return vec.front();}
    void push(const value_type& x) {
        vec.push_back(x); 
        push_heap(vec.begin(), vec.end(), cmp);
    }
    void pop() {
        pop_heap(vec.begin(), vec.end(), cmp);
        vec.pop_back();
    }
};
#endif
