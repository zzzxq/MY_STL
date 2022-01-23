/*************************************************************************
	> File Name: my_deque.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Sat 22 Jan 2022 12:53:15 PM CST
 ************************************************************************/

#ifndef _MY_DEQUE_H
#define _MY_DEQUE_H

//双端队列 的配置类似于vector，也是一种空间满了就换一片空间(或者调整自己的)
//实现上多了一个中控器，便于实现连续访问的假象(迭代器)
//头部尾部的插入删除时间复杂度都是O(1)
inline size_t __deque_buf_size(size_t __size) {
   return __size < 512 ? size_t(512 / __size) : size_t(1);
}

//迭代器是deque的灵魂，可以实现连续访问的假象
template <class _Tp, class _Ref, class _Ptr>
struct deque_iterator {
    typedef deque_iterator<_Tp, _Tp&, _Tp*>             iterator;
    typedef deque_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;
    static size_t buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
  
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp** map_pointer;
  
    typedef deque_iterator Self;
    
    _Tp* cur;          //指向该缓冲区中目前的元素
    _Tp* first;        //指向缓冲区的头部
    _Tp* last;         //指向缓冲区的尾部
    map*_pointer node; //指向管控中心

    deque_iterator() : cur(0), first(0), last(0), node(0) {}
    deque_iterator(_Tp *x, map_pointer y) : cur(x), first(*y), 
        last(*y + buffer_size()), node(y){}
    deque_iterator(const iterator& x) : cur(x.cur), first(x.first),
        last(x.last), node(x.node){}

    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + difference_type(buffer_size());
    }

    reference operator*() const { return *cur; }
    pointer operator->() const { return cur; }
    //返回两个迭代器的距离
    difference_type operator-(const self& x) const {
        return difference_type(buffer_size()) * (node - x.node - 1) + 
            (cur - first) + (x.last - x.cur);
    }

    self& operator++() {
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    self operator++(int) {
        self temp = *this;
        ++*this;
        return temp;
    }

    self& operator--() {
        if (cur == first) {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }

    //实现随机存取，必须要实现+=,-=的操作
    self &operator+=(difference_type n) {
        difference_type offset = n + (cur - first);
        if (offset >= 0 && offset < difference_type(buffer_size())) {
            cur += n; //在同一个缓冲区内，直接加
        } else {
            difference_type node_offset = 
            offset > 0 ? offset / difference_type(buffer_size()) : 
            -difference_type((-offset - 1) / buffer_size()) - 1;

            //切换到正确的节点
            set_node(node + node_offset);
            cur = first + (offset - node_offset * difference_type(buffer_size()));
        }
        return *this;
    }

    self operator+(difference_type n) const {
        self temp = *this;
        return tmp += n;
    }

    self& operator-=(difference_type n) { return *this += -n; }

    self operator-(difference_type n) const {
        self temp = *this;
        return temp -= n;
    }
    
    reference operator[](difference_type n) const{ return *(*this + n); }

    bool operator==(const self& x) const{ return cur == x.cur; }
    bool operator!=(const self& x) const { return !(*this == x); }
    //先比较节点，然后比较节点内的指针cur
    bool operator<(const self& x) const{
        return (node == x.node) ? (cur < x.cur) : (node < x.node);
    }

};

template<class T, class Alloc = alloc>
class deque {
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef deque_iterator<T, T&, T*> iterator;
    
protected:
    typedef pointer* map_pointer;
    iterator start;   //指向第一个节点
    iterator finish;  //最后一个节点
    map_pointer map;  // 中控中心，每个元素都是指针，指向一个节点(缓冲区)
    size_type map_size;
    enum {M_init_map_size = 8}; //一个map最少8个节点
    typedef typename simple_alloc<value_type, Alloc> data_allocator;
    typedef typename simple_alloc<pointer, Alloc> map_allocator;
public:
    iterator begin(){ return start; }
    iterator end() { return finish; }
    reference operator[](size_type n) {
        return start[difference_type(n)];  //调用迭代器的 operator[]
    }
    reference front() { return *start; }   //迭代器的operator*，返回cur所指的
    reference back() {
        iterator temp = finish;
        --temp;
        return *temp;
    }
    size_type size() const {return finish - start;}  //重载了迭代器的-
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return finish == start; }  //重载了迭代器的==
    
    void fill_initialize(size_type n, const value_type& x = T());
    void create_map_and_nodes(size_type num_elements);

    //构造函数
    deque():start(), finish(), map(0), map_size(0) {
        fill_initialize(0);
    }
    deque(int n, const value_type& x) :start(), finish(), map(0), map_size(0) {
        fill_initialize(n, x);
    }
    ~deque() {
        destroy(start, finish);
    }
    void push_back(const value_type& t);
    void push_back_aux(const value_type& t);
    void push_front(const value_type& t);
    void push_front_aux(const value_type& t);
    void pop_back();
    void pop_front();
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    iterator insert(iterator pos, const value_type& x);
    iterator insert_aux(iterator pos, const value_type& x);
    void clear();
    size_type init_map_size() {
        return static<size_type>(M_init_map_size);
    }

protected:
    void reserve_map_at_back(size_type nodes_to_add = 1);
    void reserve_map_at_front(size_type nodes_to_add = 1);
    void reallocate_map(size_type nodes_to_add, bool add_at_front);

};


template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(size_type n, const value_type& value) {
    create_map_and_nodes(n); //初始化map
    map_pointer cur;
    for (cur = start.node; cur < finish.node; ++cur) {
        unitialized_fill(*cur, **cur + buffer_size(), value);
    }
    unitialized_fill(finish.first, finish.cur, value); //最后一个节点需要单独设置
}

template<class T, class Alloc>
void deque<T, Alloc>::create_map_and_nodes(size_type n) {
    //需要的节点数 = 元素个数/每个缓冲区可容纳的元素个数 + 1
    size_type num_nodes = n / buffer_size() + 1;

    map_size = max(init_map_size(), num_nodes + 2); 
        //num_node需要前后各留一个，方便扩充
    map = map_allocator::allocator(map_size);
    //分配空间
    map_pointer nstart = map + (map_size - num_node) / 2;
    map_pointer nfinish = nstart + num_nodes - 1;  //将start和finish控制在中央区域
    map_pointer cur;
    for (cur = nstart; cur <= nfinish; ++cur)  {
        *cur = allocate_node();
    }

    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    finish.cur = finish.first + n % buffer_size();
}

template<class T, class Alloc>
void deque<T, Alloc>::push_back(const value_type &x) {
    if (finish.cur != finish.last - 1) {
        //缓冲区还有两个及以上的空间，cur和lsat都是指向后一个元素
        construct(finish.cur, x);
        ++finish.cur;
    } else { //只剩最后一个空间
        push_back_aux(x);
    }
}

template<class T, class Alloc>
void deque<T, Alloc>::push_back_aux(const value_type& x) {
    value_type t_copy = t;
    reserve_map_at_back(); //若符合某种条件，需要更换一个map
    *(finish.node + 1) = allocate_node(); //配置一个新缓冲区
    construct(finish.cur, t_copy);
    finish.set_node(finish.node + 1);
    finish.cur = finish.first;
}

template<class T, class Alloc>
void deuqe<T, Alloc>::push_front(const value_type& x) {
    if (start.cur != start.first) {
        construct(start.cur - 1, x);
        --start.cur;
    } else {
        push_front_aux(x);
    }
}

template<class T, class Alloc>
void deque<T, Alloc>::push_front_aux(const value_type& x) {
    value_type x_copy = x;
    reserve_map_at_front();
    *(start.node - 1) = allocate_node();
    start.set_node(start.node - 1);
    start.cur = start.last - 1;
    construct(start.cur, x_copy);
}

template<class T, class Alloc>
void deque<T, Alloc>::reserve_map_at_back(size_type nodes_to_add = 1) {
    if (nodes_to_add > max_size - (finish.node - map + 1)) {
        //尾部没有可用的缓冲区
        reallocate_map(nodes_to_add, false);
    }
}

template<class T, class Alloc>
void deque<T, Alloc>::reserve_map_at_front(size_type nodes_to_add = 1) {
    if (nodes_to_add > start.node - map) {
        reallocate_map(nodes_to_add, true);
    }
}

// 重新配置map的大小,拷贝原来的node节点过来, 只用拷贝指针, 原map上存的就是指针;
template <class T, class Alloc>
void deque<T, Alloc>::reallocate_map(size_type nodes_to_add, bool add_to_front) {
    // 计算扩容后的节点数量;
    size_type old_num_nodes = finish.node - start.node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;

    Map_pointer new_start;
    // 如果当前map大小大于加入新空间的2倍,那么移动指针就行;
    if (map_size > 2 * new_num_nodes) {
        new_start = map + (map_size - new_num_nodes) / 2 + (add_to_front ? nodes_to_add : 0);
        if (new_start < start.node) {
            copy(start.node, finish.node + 1, new_start);
        } else {
            copy_backward(start.node, finish.node + 1, new_start + old_num_nodes);
        }
    } // 当空间不足, 需要重新分配map;
    else {
        size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
        Map_pointer new_map = map_allocatetor::allocate(new_map_size);
        // 计算新的开始位;
        new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_to_front ? nodes_to_add : 0);
        // 只用拷贝原map指针过来; 这里注意 copy的last= finish.M_node + 1;
        copy(start.node, finish.node + 1, new_start);
        map_allocatetor::deallocate(this->map, map_size);
        this->map = new_map;
        this->map_size = new_map_size;
    }
    // 这个时候只是扩容, 并未插入元素;
    start.set_node(new_start);
    finish.set_node(new_start + old_num_nodes - 1);
    //start 和 finish中的cur, first , last都还是指向原来的元素, 变得不过是map中存放的指向node的指针;

}

template<class T, class Alloc>
void deque<T, Alloc>::pop_back() {
    if (finish.cur != finish.first) {
        --finish.cur;
        destroy(finish.cur);
    } else { //缓冲区空的
        pop_back_aux(); 
    }
}

template<class T, class Alloc>
void deque<T, Alloc>::pop_back_aux() {
    deallocate_node(finish.first);
    finish.set_node(finish.node - 1);
    finish.cur = finish.last - 1;
    destroy(finish.cur);   //释放缓冲区，移动finish.cur 并调用析构函数
}

template<class T, class Alloc>
void deque<T, Alloc>::pop_front() {
    if (start.cur != start.last - 1) {
        destroy(start.cur);
        ++start.cur;
    } else {
        pop_front_aux();
    }
}

template<class T, class Alloc>
void deque<T, Alloc>::pop_front_aux() {
    destroy(start.cur);
    deallocate_node(start.first);
    start.set_node(start.node + 1);
    start.cur = start.first;
}

template<class T, class Alloc>
void deque<T, Alloc>::clear() {
    for (map_pointer node = start.node + 1; node < finish.node; ++node) {
        destroy(*node, *node + buffer_size());
        data_allocator::deallocate(*node, buffer_size());
    }
    if (start.node != finish.node) { //至少有头尾两个缓冲区
        destroy(start.cur, start.last);
        destroy(finish.first, finish.cur);
        data_allocator::deallocate(finish.first, buffer_size());
    } else { //只有一个缓冲区
        destroy(start.cur, finish.cur);   
    }
    finish = start;
}

// 在指定位置插入元素;
template <class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::insert(iterator pos, const value_type& x) {
    if (pos == start) { //最前端
        push_front(x);
        return start;
    }
    else if (pos == finish){  //最后端
        push_back(x);
        iterator tmp = finish;
        --tmp;
        return tmp;
    }
    else{
        return insert_aux(pos, x);
    }
}

// 封装 insert_aux ,用来处理复杂情况;
template <class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::insert_aux(iterator pos, const value_type& x) {
    difference_type index = pos - start; // 计算插入点之前的元素个数;
    value_type x_copy = x;
    // 分类处理, 1.插入点前方元素少 2. 插入点后边元素少;
    if (index < size() / 2) {
        push_front(front());
        iterator front1 = start;
        ++front1; // start, -> front1, -> front2;
        iterator front2 = front1;
        ++front2;
        pos = start + index;
        iterator pos1 = pos;
        ++pos1;  //  将 pos 之前的元素向前移动一位;
        copy(front2, pos1, front1);
    } else {
        push_back(back());
        iterator back1 = finish;
        --back1;
        iterator back2 = back1;
        --back2;
        pos = start + index; // 将pos 后面的元素整体后移一位;
        copy_backward(pos, back2, back1);
    }
    *pos = x_copy;
    return pos;
}

//从指定迭代器位删除元素;
template <class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::erase(iterator pos) {
    iterator next = pos;
    ++next;
    difference_type index = pos - start;
    if (index < (size() >> 1)) { // 当pos前方元素较少时;
        copy_backward(start, pos, next);
        pop_front();
    } else {
        copy(next, finish, pos);
        pop_back();
    }
    return start + index;
}

//删除迭代器范围内的元素;
template <class T, class Alloc>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::erase(iterator first, iterator last) {
    if (first == start && last == finish) {
        clear();
        return finish;
    }
    else {
        difference_type n = last - first; // 计算要删除元素的多少;
        difference_type elems_before = first - start;
        if (elems_before < (size() - n) / 2) { // first前面的元素较少时;
            copy_backward(start, first, last);
            iterator new_start = start + n;
            destroy(start, new_start);   // 删除前面空的节点;
            for (Map_pointer cur = start.node; cur < new_start.node; ++cur) {
                data_allocatetor::deallocate(*cur, buffer_size());
            }
            start = new_start;
        } else {  // 移动后面的元素;
            copy(last, finish, first);
            iterator new_finish = finish - n;
            destroy(new_finish, finish); // 析构掉多余元素;
            for (Map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur) {
                data_allocatetor::deallocate(*cur, buffer_size());
            }
            finish = new_finish;
        }
        return start + elems_before;
    }
}



#endif
