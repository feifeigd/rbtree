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
    return RED == left->Color() || RED == right->Color();
}
