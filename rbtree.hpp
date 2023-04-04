#pragma once

#include "rbnode.hpp"

/*
红黑色的性质
1) 每个节点或是红的，或是黑的
2) 根结点是黑的
3) 每个叶节点(NIL)是黑的
4) 如果一个结点是红的，则他必有两个子节点并且都是黑的（或者说从每个叶子到根的所有路径上不能有两个连续的红色节点。又或者说不存在两个相邻的红色节点，相邻指两个节点是父子关系。还或者说红色节点的父节点和子节点均是黑色的。）
5) 对每个节点，从该结点到其子孙节点的所有简单路径上包含相同数目的黑节点(黑高相同)
*/
// 该实现，不包含节点的内存管理
class RBTree{
public:
    // 查找插入位置
    RBNode* find(int key);
    bool Insert(RBNode* z);
private:
    RBNode* root; // 根节点

    // 查找插入位置
    RBNode* search(RBNode* parent, RBNode* node, int key);

    // 用以v为根的子树，替换以u为根子树u
    void transplant(RBNode* u, RBNode* v);

    // 无论是左旋还是右旋，x节点的层次都是下沉一层
    // 左旋, x的右孩子变成x的父亲，x右孩子的左孩子变成x的右孩子
    bool left_rotate(RBNode* x);
    // 右旋, x的左孩子变成x的父亲，x左孩子的右孩子变成x的左孩子
    bool right_rotate(RBNode* x);

    void insert_fixup(RBNode* z);
};

void RBTree::transplant(RBNode* u, RBNode* v){
    if(!u)
        return;
    RBNode* p = u->p;
    if(!p)// u为树根
        root = v;
    else if(u == p->left)
        p->left = v;
    else
        p->right = v;
    if(v)
        v->p = p;
}

bool RBTree::left_rotate(RBNode* x){
    if(!x)
        return false;
    RBNode* y = x->right;
    if(!y) // 左旋时，右孩子不能为空
        return false;  
    
    // y 的左子树变成x的右子树
    x->right = y->left;
    if(y->left)
        y->left->p = x;
    
    // y 接到x的父节点下, 取代x的位置
    transplant(x, y);

    // x变成y的左节点
    y->left = x;
    x->p = y;

    return true;
}

bool RBTree::right_rotate(RBNode* x){
    if(!x)
        return false;
    RBNode* y = x->left;
    if(!y) // 左旋时，右孩子不能为空
        return false;  
    
    // y 的左子树变成x的右子树
    x->left = y->right;
    if(y->right)
        y->right->p = x;
    
    // y 接到x的父节点下, 取代x的位置
    transplant(x, y);

    // x变成y的左节点
    y->right = x;
    x->p = y;
    
    return true;
}

RBNode* RBTree::search(RBNode* parent, RBNode* node, int key){
    if(!node)
        return parent;
    if(key == node->key)
        return node;
    if(key < node->key)
        return search(node, node->left, key);
    return search(node, node->right, key);
}

RBNode* RBTree::find(int key){
    return search(0, root, key);
}

bool RBTree::Insert(RBNode* z){
    if(!z)
        return false;

    RBNode* y = find(z->key);

    z->p = y;
    if(!y)
        root = z;
    else if(z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = z->right = 0;
    z->color = RED; // 新插入的节点总是红色
    insert_fixup(z); // 需要旋转和变色
    return true;
}


void RBTree::insert_fixup(RBNode* z){
    while(z->p && RED == z->p->Color()){
        RBNode* p = z->p;  // z 是红色，则z不是根，所以p肯定不是空
        RBNode* pp = p->p; // p 是红色，则p不是根，所以pp肯定不是空
        RBNode* y = z->uncle(); // 叔叔

        if(RED == y->Color()){ // y 是否可能为空?
            y->color = p->color = BLACK;    // case 1
            z = pp;                         // case 1 z 爬升2层
            z->color = RED;                 // case 1
            
            continue;
        }

        // z 爬升一层
        if(p == pp->left){  // BLACK == y.Color()
            if(z == p->right){
                z = p;          // case 2
                left_rotate(z); // case 2
            }

            z->p->color = BLACK;    // case 3
            z->p->p->color = RED;   // case 3
            right_rotate(z->p->p);  // case 3
        }else{  // p == pp.right
            if(z == p->left){
                z = p;              // case 2;
                right_rotate(z);    // case 2
            }

            z->p->color = BLACK;    // case 3
            z->p->p->color = RED;   // case 3
            left_rotate(z->p->p);   // case 3
        }
    }
    root->color = BLACK; // 结点总是黑色
}
