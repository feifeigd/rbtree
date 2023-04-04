
#include "rbtree.hpp"

#include <cassert>
#include <vector>
using namespace std;

int main(){
    //vector<int> keys{11, 2, 14, 1, 7, 15, 5, 8, 4};
    vector<int> keys;
    keys.push_back(11);
    keys.push_back(2);
    keys.push_back(14);
    keys.push_back(1);
    keys.push_back(7);
    keys.push_back(15);
    keys.push_back(5);
    keys.push_back(8);
    keys.push_back(4);
    RBTree rbtree;

    for(vector<int>::iterator it = keys.begin(); it != keys.end(); ++it){
        int v = *it;
        // RBNode* z = new RBNode(v);
    }
    
    return 0;
}
