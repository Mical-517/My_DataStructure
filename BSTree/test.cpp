#include <iostream>
#include <string>
#include "BSTree.hpp"

using namespace std;    

/**
 * @description 测试整数类型的 BSTree
 */
void testIntBSTree()
{
    cout << "--- Testing BSTree<int> ---" << endl;
    BSTree<int> tree;
    int values[] = {5, 3, 7, 2, 4, 6, 8};
    for (int v : values) {
        tree.insert(v);
    }

    cout << "Recursive PreOrder: ";
    tree.preOrder();
    cout << "Stack PreOrder:     ";
    tree.preOrderStack();

    cout << "Recursive InOrder:  ";
    tree.inOrder();
    cout << "Stack InOrder:      ";
    tree.inOrderStack();

    cout << "Recursive PostOrder: ";
    tree.postOrder();
    cout << "Stack PostOrder:     ";
    tree.postOrderStack();

    cout << "Level Order:        ";
    tree.levelOrder();
    cout << endl;
}

/**
 * @description 测试字符串类型的 BSTree
 */
void testStringBSTree()
{
    cout << "--- Testing BSTree<string> ---" << endl;
    BSTree<string> tree;
    string values[] = {"apple", "banana", "cherry", "date", "eggplant"};
    for (const string& v : values) {
        tree.insert(v);
    }

    cout << "InOrder (Sorted): ";
    tree.inOrder();
    
    cout << "Level Order:      ";
    tree.levelOrder();
    cout << endl;
}

/**
 * @description 测试 AVL 平衡逻辑 (插入顺序会导致不平衡)
 */
void testAVLBalance()
{
    cout << "--- Testing AVL Balancing ---" << endl;
    BSTree<int> tree;
    // 顺序插入 1, 2, 3 会导致 RR 型不平衡，触发左单旋
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    cout << "Level Order (Expected root 2): ";
    tree.levelOrder();
    cout << endl;
}

/**
 * @description 测试 DSW 算法平衡逻辑
 */
void testDSW()
{
    cout << "--- Testing DSW Balancing ---" << endl;
    BSTree<int> tree;
    // 顺序插入 1-7，创建一个完全不平衡的链表状树
    for (int i = 1; i <= 7; i++) {
        tree.insert(i);
    }

    cout << "Before DSW (Level Order): ";
    tree.levelOrder();

    tree.DSW();
  
    cout << "After DSW (Level Order):  ";
    tree.levelOrder();
    cout << endl;
}

int main()
{
    testIntBSTree();
    testStringBSTree();
    testAVLBalance();
    testDSW();
    
    return 0;
}
