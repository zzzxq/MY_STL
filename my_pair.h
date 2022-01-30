/*************************************************************************
	> File Name: my_pair.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Sun 30 Jan 2022 11:25:34 PM CST
 ************************************************************************/

#ifndef _MY_PAIR_H
#define _MY_PAIR_H

template<class T, class U>
struct pair {
    typedef T first_type;
    typedef U second_type;

    T first;
    U second;
    pair() : first(T()), second(U()){}
    pair(const T& a, const U& b) : first(a), second(b) {}
    template<class T1,class U1>
    pair(const pair<T1, U1>& p) : first(p.first), second(p.second) {}

};

template<class T, class U>
inline bool operator==(const pair<T, U>& x, const pair<T, U>& y) {
    return x.first == y.first && x.second == y.second;
}

template<class T, class U>
inline bool operator<(const pair<T, U>& x, const pair<T, U>& y) {
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template<class T, class U>
inline bool operator!=(const pair<T, U>& x, const pair<T, U>& y) {
    return !(x == y);
}


template<class T, class U>
inline bool operator<=(const pair<T, U>& x, const pair<T, U>& y) {
    return !(y < x);
}


template<class T, class U>
inline bool operator>=(const pair<T, U>& x, const pair<T, U>& y) {
    return !(x < y);
}

template<class T, class U>
inline pair<T, U> make_pair(const T& x, const U& y) {
    return pair<T, U>(x, y);
}



#endif
