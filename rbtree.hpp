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
    bool Delete(RBNode* z);
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
    void delete_fixup(RBNode* x);
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

bool RBTree::Delete(RBNode* z){
    if(!z)
        return false;
    // 要删除的节点z的（非空节点或nil）；或者z的后继的右孩子
    RBNode* x = 0;
    // 确定要删除的节点y或者是要移动的z的后继
	RBNode*y = z; // 当z至多只有一个子女的时候，y指向z；否则y指向z的后继
    RBColor y_original_color = y->Color();
    // z至多只有一个子女
	if(!z->left){
		x = z->right;
		transplant(y, x); // 孩子取代待删节点的位置, y=z
	} else if(!z->right){
		x = z->left;
		transplant(y, x); // 孩子取代待删节点的位置,y=z
	} else { // z有两个孩子
		y = z->Successor();            // z有两个子女, z的后继y至多也只有一个子女
		y_original_color = y->Color(); // y 变了，重新赋值
		x = y->right;                  // x 是y的右孩子或者nil
		if(y->p != z){
			transplant(y, x);
			y->right = z->right;
			y->right->p = y;
		}
		// y取代z的位置和颜色
		transplant(z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}

	if(BLACK == y_original_color) { // 删除红色节点，红黑性质不变
		delete_fixup(x);
	}
}

void RBTree::delete_fixup(RBNode* x){
    if(!x)
        return;
    while(x != root && BLACK == x->Color()){
		RBNode*p = x->p;
		if (x == p->left) {
			RBNode* w = p->right; // x 的兄弟
			if (RED == w->Color()) {
				w->color = BLACK;  // case 1
				p->color = RED;    // case 1
				left_rotate(p); // case 1
				w = p->right;      // case 1
			}
			if (BLACK == w->left->Color() && BLACK == w->right->Color()) {
				w->color = RED; // case 2
				x = p;         // case 2，爬升一级，会引起继续循环
			} else { // 至少有一个红色的孩子
				if (BLACK == w->right->Color()) { // w 左红，右黑
					swap_color(w, w->left); // case 3
					right_rotate(w);        // case 3
					w = p->right;           // case 3
				}
				// w是黑色，左右两个孩子都是红色
				w->color = p->Color();      // case 4
				p->color = BLACK;           // case 4
				w->right->color = BLACK;    // case 4
				left_rotate(p);             // case 4
				x = root;                   // case 4，终止循环
			}
		} else {
			RBNode* w = p->left; // x 的兄弟
			if (RED == w->Color()) {
				w->color = BLACK;   // case 1
				p->color = RED;     // case 1
				right_rotate(p); // case 1
				w = p->left;        // case 1
			}
			if (BLACK == w->left->Color() && BLACK == w->right->Color()) {
				w->color = RED; // case 2
				x = p;         // case 2，爬升一级，会引起继续循环
			} else { // 至少有一个红色的孩子
				if (BLACK == w->left->Color()) { // w 左红，右黑
					swap_color(w, w->right); // case 3
					left_rotate(w);       // case 3
					w = p->left;             // case 3
				}
				// w是黑色，左右两个孩子都是红色
				w->color = p->Color();  // case 4
				p->color = BLACK;      // case 4
				w->left->color = BLACK; // case 4
				right_rotate(p);    // case 4
				x = root;           // case 4，终止循环
			}
		}

    }
    x->color = BLACK;
}
