/*************************************************************************
	> File Name: my_alloc.h
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Wed 19 Jan 2022 10:10:31 AM CST
 ************************************************************************/

#ifndef _MY_ALLOC_H
#define _MY_ALLOC_H


#ifndef __THROW_BAD_ALLOC
#  if defined(__STL_NO_BAD_ALLOC) || !defined(__STL_USE_EXCEPTIONS)
#    include <stdio.h>
#    include <stdlib.h>
#    define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)
#  else /* Standard conforming out-of-memory handling */
#    include <new>
#    define __THROW_BAD_ALLOC throw std::bad_alloc()
#  endif
#endif

#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

//一级配置器
template<int __inst>
class __malloc_alloc_template {
private:
    //处理内存不足的函数
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static void (* __malloc_alloc_oom_handler) ();
public:
    static void *allocate(size_t n) {
        void *result = malloc(n);
        if (0 == result) result = oom_malloc(n);
        return result;
    }
    static void deallocate(void *p, size_t /* __n */) {
        free(p);
    }
    static void *reallocate(void *p, size_t /*old_sz*/, size_t new_sz) {
        void *result = realloc(p, new_sz);
        if (0 == result) result = oom_realloc(p, new_sz);
        return result;
    }
    
    // new handler，通过它指定自己的out-of-memory handler
    static void (* set_malloc_handler(void (*f)())) () {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return (old);
    }

};

template <int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template <int __inst>
void* __malloc_alloc_template<__inst>::oom_malloc(size_t __n)
{
   void (* __my_malloc_handler)();
   void* __result;

   for (;;) {
       __my_malloc_handler = __malloc_alloc_oom_handler;
       if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
       (*__my_malloc_handler)();
       __result = malloc(__n);
       if (__result) return(__result);
    }
}

template <int __inst>
void* __malloc_alloc_template<__inst>::oom_realloc(void* __p, size_t __n)
{
    void (* __my_malloc_handler)();
    void* __result;

    for (;;) {
       __my_malloc_handler = __malloc_alloc_oom_handler;
       if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
       (*__my_malloc_handler)();
       __result = realloc(__p, __n);
       if (__result) return(__result);
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;

//第二级配置器

enum {_ALIGN = 8};
enum {_MAX_BYTES = 128};
enum {_NFREELISTS = 16}; 

template <bool threads, int inst>
class __default_alloc_template {
private:
    static size_t ROUND_UP(size_t bytes) {
        return (((bytes) + _ALIGN - 1) & ~(_ALIGN - 1));
    }
private:
    union obj {  //使用union来节省内存
        union obj *free_list_link;  //指向下一个obj
        char client_data[1];   //指向实际区块
    };
private:
    static obj* volatile free_list[_NFREELISTS];
    static size_t FREELIST_INDEX(size_t bytes) {
        return (((bytes) + _ALIGN - 1) / _ALIGN - 1);
    }
    static void *refill(size_t n);
    //配置一块nobjs*size的区域
    static char *chunk_alloc(size_t size, int &nobjs);

    static char *start_free;  //内存池开始
    static char *end_free;  //内存池结束
    static size_t heap_size;

public:
    static void *allocate(size_t n) {
        if (n > (size_t)_MAX_BYTES) { //大于128就直接调用一级配置器
            return (malloc_alloc::allocate(n));
        }
        obj *volatile *my_free_list;
        obj *result;
        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        if (result == 0) {
            //没找到可用的free list,重新填充free_list
            void *r = refill(ROUND_UP(n));
            return r;
        }
        *my_free_list = result->free_list_link; //指向下一个
        return result;
    }
    
    static void deallocate(void *p, size_t n) {
        if (n > (size_t)_MAX_BYTES) {
            malloc_alloc::deallocate(p, n);
            return;
        }
        obj *q = (obj *)p;
        obj *volatile *my_free_list;
        my_free_list = free_list + FREELIST_INDEX(n);
        q->free_list_link = *my_free_list;  
        *my_free_list = q;   //头插法，插入q节点
    }
    static void *reallocate(void *p, size_t old_sz, size_t new_sz) {
        void *result;
        size_t copy_sz;
        if (old_sz > (size_t)_MAX_BYTES && new_sz >(size_t) _MAX_BYTES) {
            return (realloc(p, new_sz));
        }
        if (ROUND_UP(old_sz) == ROUND_UP(new_sz)) return p;
        //重新配置大小
        result = allocate(new_sz);
        copy_sz = new_sz > old_sz ? old_sz : new_sz;  //取小的
        memcpy(result, p, copy_sz);
        deallocate(p, old_sz); //释放原来的空间
        return result;
    }

};

template<bool threads, int inst>
char *__default_alloc_template<threads, inst>::start_free = 0;

template<bool threads, int inst>
char *__default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::free_list[_NFREELISTS] = 
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };


typedef __default_alloc_template<true, 0> alloc;
typedef __default_alloc_template<false, 0> single_client_alloc;

//当free_list没有可用区块，调用refill，重新填充空间
//新的空间取自内存池(通过chunk_alloc完成)
template<bool threads, int inst>
void *__default_alloc_template<threads, inst>::refill(size_t n) {
    int nobjs = 20;
    char *chunk = chunk_alloc(n, nobjs); //nobjs是引用
    obj *volatile *my_free_list;
    obj *result;
    obj *current_obj, *next_obj;
    int i;
    //只有一块，直接返回
    if (1 == nobjs) return chunk;
    my_free_list = free_list + FREELIST_INDEX(n);
    result = (obj*)chunk;  //第一块返回给客端
    *my_free_list = next_obj = (obj*)(chunk + n); //指向下一块，一块大小是n
    for (i = 1; ; i++) {//将后面的区块串起来
        current_obj = next_obj;
        next_obj = (obj*)((char *)next_obj + n);
        if (nobjs - 1 == i) { //最后一块
            current_obj->free_list_link = 0;
            break;
        } else {
            current_obj->free_list_link = next_obj;
        }
    }
    return result;
}


//配置chunk,size大小为8的倍数，objs代表分配的块数
template<bool thread, int inst>
char *__default_alloc_template<thread, inst>::chunk_alloc(size_t size, int &nobjs) {
    char *result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;  // 内存池剩余大小
    if (bytes_left >= total_bytes) { //足够分配nobjs个
        result = start_free;
        start_free += total_bytes;
        return (char*)result;
    } else if (bytes_left >= size) { //不够，但足够供应一个以上的区块
        nobjs = bytes_left / size;
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return (char*)result;
    } else {  //一个都不足
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        //一次性申请两倍+已使用容量的1/16
        if (bytes_left > 0) {
            //内存池有剩余，把它连接到相应的free_list上
            obj *volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj*)start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj*)start_free;
        }
        start_free = (char*)malloc(bytes_to_get);
        if (0 == start_free) {
            //操作系统也没有内存了
            int i;
            obj *volatile *my_free_list, *p;
            //去检查目前手上有的更大的区块，而不是去合并较小的区块
            //因为在多进程机器上会导致灾难
            for (i = size; i <= _MAX_BYTES; i += _ALIGN) {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) { //还有多的
                    *my_free_list = p->free_list_link;
                    start_free = (char*)p; 
                    end_free = start_free + i;  //修改头和尾指针
                    return chunk_alloc(size, nobjs);
                }
            }
            end_free = 0; //没有内存可用
            start_free = (char*)malloc_alloc::allocate(bytes_to_get);  //试试一级配置器
        }
        //申请到了足够的内存
        heap_size += bytes_to_get; //记录申请的总内存
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs); //递归调用
    }

}


//多一层包装，使得Alloc具备标准接口
//可以通过自定义类型和分配器来调用simple_alloc
template<class _Tp, class _Alloc>
class simple_alloc {
public:
    static _Tp* allocate(size_t __n) {
        return 0 == __n ? 0 : (_Tp*) _Alloc::allocate(__n * sizeof(_Tp));
    }
    static _Tp* allocate(void) {
        return (_Tp*) _Alloc::allocate(sizeof (_Tp));
    }
    static void deallocate(_Tp *p, size_t __n) {
        if (0 != __n) _Alloc::deallocate(p, __n * sizeof (_Tp));
    }
    static void deallocate(_Tp* __p) {
        _Alloc::deallocate(__p, sizeof (_Tp));
    }
};

#endif
