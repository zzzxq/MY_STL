/*************************************************************************
	> File Name: my_heap.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Tue 25 Jan 2022 06:09:45 PM CST
 ************************************************************************/

#ifndef _MY_HEAP_H
#define _MY_HEAP_H

template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}

template<class RandomAccessIterator, class Distance, class T>
inline __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
    __push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
}

template<class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value) {  //大顶堆
        *(first + holeIndex) = *(first + parent);  //父节点的值往下
        holeIndex = parent;       //洞节点的值上调
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;  //填充洞值
}

template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __pop_heap_aux(first, last, value_type(first));
}

template<class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template<class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
    *result = *first;
    //设置尾值为首值，此时尾指就是结果
    __adjust_heap(first, Distance(0), Distance(last - first), value); //重新调整0-last
}

template<class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
    Distance father = holeIndex;
    Distance maxChild = 2 * holeIndex + 1;
    while (maxChild < len) {
        if (maxChild + 1 < len && *(first + maxChild) < *(first + (maxChild + 1))) maxChild++;
        //此时maxChild指向更大的
        if (value < *(first + maxChild)) {
            //向下调整
            *(first + father) = *(first + maxChild);
            father = maxChild;
            maxChild = 2 * father + 1;
        } else {
            break;
        }
    }
    *(first + father) = value;   //此时说明父节点比两个子节点都大，父节点填为value
    return;
}


template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    while (last - first > 1) {
        pop_heap(first last--);
    }
}

template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __make_heap(first, last, value_type(first), distance_type(first));
}

template<class RandomAccessIterator, class T class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
    if (last - first < 2) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2; //找到最后一个叶子节点的父节点
    while (parent >= 0) {
        __adjust_heap(first, parent, len, T(*(first + parent)));
        parent--;
    } //不断向下调整成 大顶堆
}


//重载自定义比较运算符
template<class RandomAccessIterator, class Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
    __push_heap_aux(first, last, cmp, distance_type(first), value_type(first));
}

template<class RandomAccessIterator, class Compare, class Distance, class T>
inline __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Compare cmp, Distance*, T*) {
    __push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)), cmp);
}

template<class RandomAccessIterator, class Distance, class T, class Compare>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, Compare cmp) {
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && cmp(*(first + parent),value)) {  //自定义比较
        *(first + holeIndex) = *(first + parent);  //父节点的值往下
        holeIndex = parent;       //洞节点的值上调
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;  //填充洞值
}

template<class RandomAccessIterator, class Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
    __pop_heap_aux(first, last, cmp, value_type(first));
}

template<class RandomAccessIterator, class Compare,  class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Compare cmp, T*) {
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first), cmp);
}

template<class RandomAccessIterator, class T, class Distance, class Compare>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare cmp) {
    *result = *first;
    //设置尾值为首值，此时尾指就是结果
    __adjust_heap(first, Distance(0), Distance(last - first), value, cmp); //重新调整0-last
}

template<class RandomAccessIterator, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare cmp) {
    Distance father = holeIndex;
    Distance maxChild = 2 * holeIndex + 1;
    while (maxChild < len) {
        if (maxChild + 1 < len && cmp(*(first + maxChild), *(first + (maxChild + 1)))) maxChild++;
        //此时maxChild指向更大的
        if (cmp(value, *(first + maxChild))) {
            //向下调整
            *(first + father) = *(first + maxChild);
            father = maxChild;
            maxChild = 2 * father + 1;
        } else {
            break;
        }
    }
    *(first + father) = value;   //此时说明父节点比两个子节点都大，父节点填为value
    return;
}





#endif
