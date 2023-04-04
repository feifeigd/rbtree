#pragma once

enum RBColor{
    BLACK,
    RED,
};

struct RBNode
{
    int key;
    RBColor color;
    RBNode *p, *left, *right;
    RBColor Color()const{
        return color;
    }
    RBNode* uncle()const;
    RBNode* subling()const;
    // 至少拥有一个红色孩子
    bool has_red_children()const;

    RBNode(int key){
        this->key = key;
        p = 0;
        left = 0;
        right = 0;
    }

    // 后继
    RBNode* Successor();
    // 最小节点
    RBNode* MinNode();
};

RBNode* RBNode::uncle()const{
    if(!p)
        return 0;
    
    RBNode* pp = p->p;
    if(!pp)
        return 0;
    return p == pp->left ? pp->right : pp->left;
}

RBNode* RBNode::subling()const{
    if(!p)
        return 0;
    
    return this == p->left ? p->right : p->left;
}

bool RBNode::has_red_children()const{
    return (left && RED == left->Color()) || (right && RED == right->Color());
}

// 后继
RBNode* RBNode::Successor(){
	RBNode *  x = this;
    if(!x)
        return 0;
	if(right) {
		return right->MinNode();
	}
	RBNode * y = p;
	while(y && x == y->right){
		x = y;
		y = y->p;
	}
	return y;
}

RBNode* RBNode::MinNode() {
	RBNode*x = this;
	while(x && x->left)  {
		x = x->left;
	}
	return x;
}

void swap_color(RBNode* x, RBNode* y){
    RBColor x_color = x->Color();
    RBColor y_color = y->Color();
    x->color = y_color;
    y->color = x_color;
}
