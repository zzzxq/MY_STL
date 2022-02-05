/*************************************************************************
	> File Name: my_hash_func.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Sat 05 Feb 2022 07:58:38 PM CST
 ************************************************************************/

#ifndef _MY_HASH_FUNC_H
#define _MY_HASH_FUNC_H
//一些常用形式的哈希函数


template <class Key>
struct hash {};

// 字符串统一使用BKDR_HASH
inline 
size_t BKDRHash(const char *s) {
    size_t seed = 31;
    size_t h = 0;
    for (int i = 0; s[i]; ++i) {
        h = h * seed + s[i];
    }
    return h & 0x7fffffff;
}

template<>
struct hash<char *>
{
    size_t operator()(const char *s) const {
        return BKDRHash(s);
    }  
};

template<>
struct hash<const char *>
{
    size_t operator()(const char *s) const {
        return BKDRHash(s);
    }  
};

template<>
struct hash<char>
{
    size_t operator()(char x) const {
        return x;
    }  
};

template<>
struct hash<const char>
{
    size_t operator()(char x) const {
        return x;
    }  
};

template<>
struct hash<unsigned char>
{
    size_t operator()(unsigned char x) const {
        return x;
    }  
};

template<>
struct hash<signed char>
{
    size_t operator()(unsigned char x) const {
        return x;
    }  
};

template<>
struct hash<short>
{
    size_t operator()(short x) const {
        return x;
    }  
};

template<>
struct hash<int>
{
    size_t operator()(int x) const {
        return x;
    }  
};

template<>
struct hash<long>
{
    size_t operator()(long x) const {
        return x;
    }  
};

template<>
struct hash<long long int>
{
    size_t operator()(long long int x) const {
        return x;
    }  
};

template<>
struct hash<const short>
{
    size_t operator()(short x) const {
        return x;
    }  
};

template<>
struct hash<const int>
{
    size_t operator()(int x) const {
        return x;
    }  
};

template<>
struct hash<const long>
{
    size_t operator()(long x) const {
        return x;
    }  
};

template<>
struct hash<const long long int>
{
    size_t operator()(long long int x) const {
        return x;
    }  
};

#endif
