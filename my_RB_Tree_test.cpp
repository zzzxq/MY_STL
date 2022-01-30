/*************************************************************************
	> File Name: my_RB_Tree_test.cpp
	> Author: zxq
	> Mail: 1459190705@qq.com
	> Created Time: Sat 29 Jan 2022 02:33:24 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;


struct Node {
    int val;
    int color;  // 0红  1黑 2双重黑
    Node *left, *right;
};

Node __NIL;
#define NIL (&__NIL)
__attribute__((constructor))
void init() {
    NIL->color = 1;
    NIL->val = 0;
    NIL->left = NIL->right = NIL;
}


Node *getNewNode(int val) {
    Node *p = (Node*)malloc(sizeof(Node));
    p->val = val;
    p->color = 0; //红色
    p->left = p->right = NIL;
    return p;
}

int has_red_child(Node *root) {
    return root->left->color == 0 || root->right->color == 0;
}

Node *left_rotate(Node *root) {
    Node *temp = root->right;
    root->right = temp->left;
    temp->left = root;
    return temp;
}

Node *right_rotate(Node *root) {
    Node *temp = root->left;
    root->left = temp->right;
    temp->right = root;
    return temp;
}


Node *insert_maintain(Node *root) {
    if (!has_red_child(root)) return root;
    if (root->left->color == 0 && root->right->color == 0) {
        if (!has_red_child(root->left) && !has_red_child(root->right)) return root;
        root->color = 0;
        root->left->color = root->right->color = 1;
        return root;
    }
    if (root->left->color == 0 && !has_red_child(root->left)) return root;
    if (root->right->color == 0 && !has_red_child(root->right)) return root;
    if (root->left->color == 0) {
        if (root->left->right->color == 0) {
            root->left = left_rotate(root->left);
        }
        root = right_rotate(root);
        root->color = 0;
        root->left->color = root->right->color = 1;
    } else {
        if (root->right->left->color == 0) {
            root->right = right_rotate(root->right);
        }
        root = left_rotate(root);
        root->color = 0;
        root->left->color = root->right->color = 1;
    }
    return root;

}


Node *__insert(Node *root, int val) {
    if (root == NIL) return getNewNode(val);
    if (root->val == val) return root;
    else if (root->val < val) {
        root->right = __insert(root->right, val);
    } else {
        root->left = __insert(root->left, val);
    }
    return insert_maintain(root);
} 

Node *insert(Node *root, int val) {
    root = __insert(root, val);
    root->color = 1;
    return root;
}

Node *precedessor(Node *root) {
    Node *temp = root->left;
    while (temp->right != NIL) temp = temp->right;
    return temp;
}


Node *erase_maintain(Node *root) {
    if (root->left->color < 2 && root->right->color < 2) return root;
    if (has_red_child(root)) {
        root->color = 0;
        int flag = 0;
        if (root->left->color == 0) {
            root = right_rotate(root);
            flag = 0;
            //root->color = 1;
            //root->right = erase_maintain(root->right);
        } else {
            root = left_rotate(root);
            flag = 1;
            //root->color = 1;
            //root->left = erase_maintain(root->left);
        }
        root->color = 1;
        if (flag == 0) root->right = erase_maintain(root->right);
        else root->left = erase_maintain(root->left);
        return root;
    }
    if ((root->left->color == 2 && !has_red_child(root->right)) ||
        (root->right->color == 2 && !has_red_child(root->left))) {
            root->color += 1;
            root->left->color -= 1;
            root->right->color -= 1;
            return root;
        }
    if (root->left->color == 2) {
        root->left->color = 1;
        if (root->right->right->color != 0) {
            root->right->color = 0;
            root->right = right_rotate(root->right);
            root->right->color = 1;
        }
        root->right->color = root->color;
        root = left_rotate(root);
        //root->color = root->left->color;
        root->right->color = root->left->color = 1;
    } else {
        root->right->color = 1;
        if (root->left->left->color != 0) {
            root->left->color = 0;
            root->left = left_rotate(root->left);
            root->left->color = 1;
        }
        root->left->color = root->color;
        root = right_rotate(root);
        //root->color = root->right->color;
        root->left->color = root->right->color = 1;
    }
    return root;
}

Node *__erase(Node *root, int val) {
    if (root == NIL) return root;
    if (root->val > val) {
        root->left = __erase(root->left, val);
    } else if (root->val < val) {
        root->right = __erase(root->right, val);
    } else {
        if (root->left == NIL || root->right == NIL) {
            Node *temp = (root->left == NIL ? root->right : root->left);
            temp->color += root->color;
            free(root);
            return temp;
        } else {
            Node *pre = precedessor(root);
            root->val = pre->val;
            root->left = __erase(root->left, pre->val);
        } 
    }
    return erase_maintain(root);
}

Node *erase(Node *root, int val) {
    root = __erase(root, val);
    root->color = 1;
    return root;
}

void print(Node *root) {
    printf("%d %d %d %d\n", root->val, root->color, root->left->val, root->right->val);
    return;
}

void output(Node *root) {
    if (root == NIL) return;
    output(root->left);
    print(root);
    output(root->right);
    return;
}



int main() {
    Node *root = NIL;
    int op, val;
    while (scanf("%d %d", &op, &val) != EOF) {
        if (op == 1) {
            root = insert(root, val);
        } else if (op == 2) {
            root = erase(root, val);
        } else {
            output(root);
        }
    }
    return 0;
}



