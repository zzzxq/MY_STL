/*************************************************************************
	> File Name: List.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 17 Jan 2022 09:29:02 AM CST
 ************************************************************************/

#include<iostream>
using namespace std;


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ostream& operator<<(ostream &out, ListNode *node) {
    while (node) {
        out << node->val << " ";
        node = node->next;
    }
    return out;
}

void merge(ListNode* &L1, ListNode* &L2) { //归并两个升序链表
    ListNode *q = L2;
    ListNode ret(0), *p = &ret;
    p->next = L1;
    while (p->next != nullptr && q != nullptr) {
        if (p->next->val > q->val) {
            ListNode *next = q->next;
            q->next = p->next;
            p->next = q;
            q = next;
        } else {
            p = p->next;
        }  
    }
    if (q) {
        p->next = q;
        q = nullptr;
    }
    L1 = ret.next;
    L2 = nullptr;
    return;
}

ListNode *sortList(ListNode *head) {
    if (head == nullptr || head->next == nullptr) return head;
    ListNode *carry, *counter[64];
    int fill = 0;
    while (head) {
        carry = head;
        head = head->next;
        carry->next = nullptr;
        //取一个节点
        int i = 0;
        while (i < fill && counter[i] != nullptr) {
            merge(counter[i], carry); //将carry合并到counter[i]上，然后carry指向空
            swap(counter[i++], carry);
        }
        swap(carry, counter[i]);
        if (i == fill) ++fill;
    }
    for (int i = 1; i < fill; i++) {
        merge(counter[i], counter[i - 1]);
    }
    return counter[fill - 1];
}

int main() {
    int n;
    cin >> n;
    ListNode *head, *p;
    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;
        if (i == 0) {
            head = new ListNode(a);
            p = head;
        } else {
            p->next = new ListNode(a);
            p = p->next;
        }
    }
    cout << head << endl;
    head = sortList(head);
    cout << head << endl;
    return 0;
}

