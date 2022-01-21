/*************************************************************************
	> File Name: my_vector.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Thu 20 Jan 2022 09:07:57 PM CST
 ************************************************************************/

#ifndef _MY_VECTOR_H
#define _MY_VECTOR_H

//实现动态数组vector
template<class T, class Alloc = alloc>
class vector {
public:
    typedef T            value_type;
    typedef value_type*  pointer;
    typedef const value_type* const_pointer;
    typedef value_type*  iterator;   //迭代器类型是指针
    typedef const value_type* const_iterator;
    typedef value_type&  reference;
    typedef const value_type& const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;
protected:
    //空间配置器
    typedef simple_alloc<value_type, Alloc> data_allocator;
    iterator start;               //第一个元素位置
    iterator finish;              //最后一个元素的下一个
    iterator end_of_storage;      //可用空间的结尾

    void insert_aux(iterator position, const T& x);
    void deallocate() {
        if (start) {
            data_allocator::deallocate(start, end_of_storage - start);
        }
    }
    //申请空间并初始化
    void fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }
    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocator::allocator(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }
public:
    iterator begin() { return start; }
    const_iterator cbegin() const { return start; }
    iterator end() { return finish; }
    const_iterator end() const { return finish; }
    size_type size() { return size_type(end() - begin()); }
    size_type capacity() { return size_type(end_of_storage - begin()); }
    bool empty() const { return begin() == end(); }
    reference operator[](size_type n) { return *(begin() + n); }
    const_reference operator[](size_type n) const { return *(cbegin() + n) ;}
    
    reference front() { return *begin(); }
    reference back() { return *(end - 1); }
    //构造函数    
    vector():start(0), finish(0), end_of_storage(0){}
    vector(size_type n, const T& value) { fill_initialize(n, value); }
    vector(long n, const T& value) { fill_initialize(n, value); }
    explicit vector(size_type n) { fill_initialize(n, T()); }
    
    ~vector() {
        destroy(start, finish);
        deallocate();
    }
    
    vector(const vector<T, Alloc> &__x) {
        this->~vector();
        size_type n = __x.capacity();
        start = data_allocator::allocate(n);
        for (int i = 0; i < __x.capacity(); i++) {
            construct(start + i, __x[i]);
        }
        finish = start + __x.size();
        end_of_storage = start + __x.capacity();
        return;
    }

    vector(vector<T, Alloc>&& __x) : start(__x.begin()), finish(__x.end())
    , end_of_storage(__x.begin() + __x.capacity()){
        __x.start = __x.finish = __x.end_of_storage = nullptr;
    }
    
    vector<T, Alloc>& operator=(const vector<T, Alloc>& _x) {
        if (&_x != this) {
            const size_type _xlen = _x.size();
            if (_xlen > capacity()) { //需要扩容
                iterator _temp = data_allocator::allocate(_xlen);
                uninitialized_copy(_x.start, _x.end(), _temp);
                destroy(start, finish);
                deallocate();
                start = _temp;
                finish = start + _xlen;
                end_of_storage = start + _xlen;
            } else if (size() >= _xlen) {
                iterator __i = copy(_x.begin(), _x.end(), begin());
                destroy(__i, finish);
                finish = start + _xlen;
            } else {
                copy(_x.begin(), _x.begin() + size(), start);
                uninitialized_copy(_x.begin() + size(), _x.end(), finish);
                finish = start + _xlen;
            }
        }
        return *this;
    }
    

    void insert(iterator pos, const T& x);
    void insert(iterator pos, size_type n, const T& x);

    void push_back(const T& x) {
        if (finish != end_of_storage) {
            construct(finish, x);   //全局函数，就是placement new
            ++finish;
        } else {
            insert_aux(end(), x);    //满了，调用扩容函数
        }
    }

    void pop_back() {
        --finish;
        destroy(finish);
    }

    iterator erase(iterator position) {
        if (position + 1 != end()) {
            copy(position + 1, finish, position); //后续元素前移一位
        }
        --finish;
        destroy(finish);
        return position;
    }
    
    iterator erase(iterator first, iterator last) {
        iterator __i = copy(last, finish, first);
        destroy(__i, finish);
        finish = finish - (last - first);
        return first;
    }

    void resize(size_type new_size, const T& x) {
        if (new_size < size()) {
            erase(begin() + new_size, end());
        } else {
            insert(end(), new_size - size(), x);
        }
    }

    void resize(size_type new_size) { resize(new_size, T()); }
    void clear() { erase(begin(), end()); }

};

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, const T& x) {
    insert_aux(pos, x);
    return;
}

template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator pos, const T& x) {
    if (pos != end_of_storage) {
        construct(finish, *(finish - 1));
        ++finish;
        copy_backward(pos, finish - 2, finish - 1);
        *pos = x;
    }
    else{
        const size_type old_size = size();
        const size_type len = old_size != 0 ? old_size * 2 : 1;  //双倍扩容

        iterator new_start = data_allocatetor::allocate(len);
        iterator new_finish = new_start;
        try {
            new_finish = copy(start, pos, new_start);
            construct(new_finish, x);
            ++new_finish;
            new_finish = copy(pos, finish, new_finish);
        }
        catch(...) {
            destroy(new_start, new_finish);
            data_allocatetor::deallocate(new_start, len);
            throw;
        }
        destroy(begin(), end());
        deallocate();

        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    }
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, size_type n, const T& x) {
    if (n == 0) return ;
    //当备用空间够用时;
    if (size_type(end_of_storage - finish) >= n) {
        T x_copy = x;
        const size_type __elements_after = finish - pos;
        iterator old_finish = finish;
        if (__elements_after > n) {
            copy(finish - n, finish, finish);  //把后n个元素后移
            finish += n;
            copy_backward(pos, old_finish - n, old_finish);  //把pos到finish-n的元素后移
            _M_fill(pos, pos + n, x_copy); //填充pos到pos+n
        } else {
            uninitialized_fill_n(finish, n - __elements_after, x_copy);
            finish += n - __elements_after;
            uninitialized_copy(pos, old_finish, finish);
            finish += __elements_after;
            fill(pos, old_finish, x_copy);
        }
    }
    else {
        const size_type old_size = size();
        const size_type len = old_size + std::max(old_size, n);

        iterator new_start = data_allocatetor::allocate(len);
        iterator new_finish = new_start;

        try{
            new_finish = copy(start, pos, new_start);
            new_finish = uninitialized_fill_n(new_finish, n, x);
            new_finish = copy(pos, finish, new_finish);
        }
        catch(...){
            destroy(new_start, new_finish);
            data_allocatetor::deallocate(new_start, len);
            throw;
        }
        destroy(start, finish);
        deallocate();

        start = new_start;
        finish = new_finish;
        end_of_storage =  new_start + len;
    }
}




#endif
