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

    RBNode* root; // 根节点
private:
    // 查找插入位置
    RBNode* search(RBNode* parent, RBNode* node, int key);

    // 用以v为根的子树，替换以u为根子树u
    void transplant(RBNode* u, RBNode* v);

    // 无论是左旋还是右旋，x节点的层次都是下沉一层
    // 左旋, x的右孩子变成x的父亲，x右孩子的左孩子变成x的右孩子
    bool left_rotate(RBNode* x);
    // 右旋, x的左孩子变成x的父亲，x左孩子的右孩子变成x的左孩子
    bool right_rotate(RBNode* x);
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
