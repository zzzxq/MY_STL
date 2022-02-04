/*************************************************************************
	> File Name: test.c
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Mon 24 Jan 2022 02:21:43 PM CST
 ************************************************************************/

#include<stdio.h>
#include<iostream>

using std::cin;
using std::cout;
using std::endl;

#define BEGIN(x) namespace x {
#define END(x) }

#include"./my_shard_ptr.h"

BEGIN(shared_ptr_test)




class A {
public:
    A(int a, int b, int c) :a(a), b(b), c(c){
        printf("ctor\n");
    }
    ~A() {
        printf("dtor\n");
    }
    int a, b, c;

};

int main() {
    shared_ptr<A> p = make_shared<A>(1, 2, 3);
    cout << p->a << " " << p->b << " " << p->c << endl;
    printf("%d\n", p.use_count());
    shared_ptr<A> q = p;
    printf("%d\n", p.use_count());
    return 0;
}

END(shared_ptr_test)


BEGIN(my_pair)

#include"./my_pair.h"

int main() {
    pair<int, char> a(3, 'c');
    printf("%d %c\n", a.first, a.second);
    pair<int, char> b = make_pair(3, 'd');
    if (a < b) {
        printf("1111\n");
    } else {
        printf("2222\n");
    }
    return 0;
}


END(my_pair)


BEGIN(my_vector)

#include"./my_vector.h"



int main() {
    vector<int> arr;
    arr.push_back(1);
    cout << arr.capacity() << endl;
    arr.push_back(2);
    cout << arr.capacity() << endl;
    cout << &arr[0] << " " << &arr[1] << endl;
    return 0;
}

END(my_vector)

int main() {
    //shared_ptr_test::main();
    //my_pair::main();
    my_vector::main();
    return 0;
}
