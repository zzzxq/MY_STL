/*************************************************************************
	> File Name: my_list.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Fri 21 Jan 2022 10:25:47 AM CST
 ************************************************************************/

#ifndef _MY_LIST_H
#define _MY_LIST_H


//双向链表
template<class T>
struct __list_node {
    __list_node<T>* prev;
    __list_node<T>* next;
    T data;
};

//list迭代器
template<class T, class Ref, class Ptr>
struct __list_iterator {
    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, Ref, Ptr> self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_Type;
    typedef Ptr pointer;
    typedef Ref reference;   //迭代器的五种类型
    typedef __list_node<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    link_type node;  //指向节点的普通指针
    
    //构造函数
    __list_iterator(link_type x): node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator& x) : node(x.node) {}

    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }

    reference operator*() const { return (*node).data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() {
        node = (link_type)((*node).next);
        return *this;
    }
    self operator++(int) {
        self temp = *this;
        ++*this;
        return temp;
    }
    self& operator--() {
        node = (link_type)((*node).prev);
        return *this;
    }
    self operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }
};

//list的实现

template<class T, class Alloc = alloc>
class list {
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef __list_node<T> list_node;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef list_node* link_type;
    typedef link_type node; //关键  尾结点，next指向第一个，prev指向最后一个

    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, const T&, cosnt T*> const_iterator;
protected:
    typedef simple_alloc<list_node, Alloc> list_node_allocator;

    link_type get_node() { return list_node_allocator::allocator(); }
    void put_node(link_type p) { list_node_allocator::deallocate(p); }
    link_type create_node(const T& x) {
        link_type p = get_node();
        construct(&p->data, x);
        return p;
    }
    void destroy_node(link_type p) {
        destroy(&p->data);
        put_node(p);
    }
    void empty_initialize() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
public:
    list() { empty_initialize(); }
    ~list() {
        clear();
        destroy_node(node);
        return;
    }    
    size_type size() const {
        size_type _result = 0;
        distance(begin(), end(), _result);
        return _result;
    }
    bool empty() const { return node->next == node; }
    iterator begin() { return (link_type)((*node).next); }
    const_iterator begin() const { return (const_iterator)((*node).next); }
    
    iterator end() { return node; }
    const_iterator end() const { return (const_iterator)(node); }

    reference front() { return *begin(); }
    reference back() { return *(--end()); }

    iterator insert(iterator position, const T& x) {
        link_type temp = create_node(x);
        temp->next = position.node;
        temp.prev = position.node->prev;
        (link_type(position.node->prev))->next = temp;
        position.node->prev = temp;
        return temp;
    }

    iterator erase(iterator position) {
        link_type next_node = link_type(position.node->next);
        link_type prev_node = link_type(position.node->prev);
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(position);
        return iterator(next_node);
    }

    void push_back(const T& x) { insert(end(), x); }
    void push_front(const T& x) { insert(begin(), x); }
    void pop_back() {
        iterator temp = end();
        erase(--temp);
    }
    void pop_front() {
        erase(begin());
    }
    
    void clear() {
        link_type cur = (link_type)node->next;
        while (cur != node) {
            link_type tmp = cur;
            cur = (link_type)cur->next;
            destroy_node(tmp);
        }
        node->next = node;
        node->prev = node; //回归原始状态
    }
    //删除所有数值为Value的节点
    void remove(const T& value) {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            iterator next = first;
            ++next;
            if (*first == value) erase(first);
            first = next;
        }
    }
    //移除连续且相同的元素
    void unique() {
        iterator first = begin();
        iterator last = end();
        if (first == last) return;
        iterator next = first;
        while (++next != last) {
            if (*first == *next) {
                erase(next);
            } else {
                first = next;
            }
            next = first;
        }
        return;
    }
    void swap(list<T, Alloc>& _x) {
        std::swap(node, _x.node);
    }
    //把区间[first, last)内的所有元素移动到position之前
protected:
    void transfer(iterator position, iterator first, iterator last) {
        if (position != last) {
            (*(link_type((*last.node).prev))).next = position.node;
            (*(link_type((*first.node).prev))).next = last.node;
            (*(link_type((*position.node).prev))).next = first.node;
            link_type temp = link_type((*position.node).prev);
            (*position.node).prev = (*last.node).prev;
            (*last.node).prev = (*first.node).prev;
            (*first.node).prev = temp;
        }
    }
public:
    //公开接口splice
    void splice(iterator position, list &x) {
        if (!x.empty()) {
            transfer(position, x.begin(), x.end());
        }
    }
    //将i所指的元素接和与position所指的位置之前,position和i可指向同一个list
    void splice(iterator position, list &, iterator i) {
        iterator j = i;
        ++j;
        if (position == i || position == j) return;
        transfer(position, i, j);
    }
    //将[first, last)内的所有元素接合与position位置之前
    void splice(iterator position, list &, iterator first, iterator last) {
        if (first != last) {
            transfer(position, first, last);
        }
    }
    //将x合并到*this上，两个链表必须都是递增， 类似于归并排序
    void merge(list& _x) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();
        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;  //将first2插入到first1之前
            } else 
                ++first1;
        }
        if (first2 != last2) transfer(last1, first2, last2); 
            //如果x中的链表还有剩余，直接接到last1之后
    }
    void sort() { //非常巧妙的归并算法
        if (node->next == node || link_type(node->next)->next == node) return;
        list<T, Alloc> carry;
        list<T, Alloc> counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());  //取一个放在carry
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }
        for (int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i - 1]); 
        }
        swap(counter[fill - 1]);
    }
    void reverse() {
        if (node->next == node || link_type(node->next)->next == node) return;
        iterator first = begin();
        ++first;
        while (first != end()) {
            iterator old = first;
            ++first;
            transfer(begin(), old, fisrt); //每次取一个，放到头结点
        }
    }
    
};



#endif
