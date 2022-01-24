/*************************************************************************
	> File Name: my_shard_ptr.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Mon 24 Jan 2022 01:48:45 PM CST
 ************************************************************************/

#ifndef _MY_SHARD_PTR_H
#define _MY_SHARD_PTR_H


#include<utility>

template<class T>
class ptr_data{
public:
    ptr_data(T *p, int *cnt = nullptr) : ptr(p), cnt(cnt) {
        if (p == nullptr) this->cnt = new int(0);
        else if (cnt == nullptr) this->cnt = new int(1);
    }
    void increase_one() {
        *cnt += 1;
        return;
    }
    void decrease_one() {
        *cnt -= 1;
        if (*cnt == 0) delete ptr;
        return;
    }
    bool operator==(const ptr_data& p) const {
        return p.ptr == ptr && p.cnt == cnt;
    }
    ~ptr_data() {}

    T *ptr;
    int *cnt;
};




template<class T>
class shared_ptr {
public:
    shared_ptr(T *ptr) : p(ptr) {}
    shared_ptr(const shared_ptr<T> &ptr) : p(ptr.p){
        this->p.increase_one();
    }
    shared_ptr<T> &operator=(const shared_ptr<T> &obj) {
        if (this->p == obj.p) return *this;
        p.decrease_one();
        p = obj.p;
        p.increase_one();
        return *this;
    }
    T &operator*() { return *p.ptr; }
    T *operator->() { return p.ptr; }
    int use_count() { return *(p.cnt); }
    ~shared_ptr() {
        p.decrease_one();
    }
private:
    ptr_data<T> p;
};


template<class T, class ...Args>
inline shared_ptr<T>
make_shared(Args&&... args) {
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}





#endif
