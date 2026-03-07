#include <iostream>
#include "BSTree_unique_ptr.hpp"
using namespace std;

int main()
{
    BSTree<int> bst;
    bst.insert_interface(3);
    bst.insert_interface(2);
    bst.insert_interface(1);
    bst.preOrder();
    return 0;
}
