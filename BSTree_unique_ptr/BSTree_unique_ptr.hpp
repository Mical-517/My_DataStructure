#include <iostream>
#include <memory>
using namespace std;

template <class DataType>
class BSTree;

template <class DataType>
class Node
{
public:
    Node() = default;
    Node(const DataType &infor, unique_ptr<Node<DataType>> left, unique_ptr<Node<DataType>> right)
    {
        this->infor = infor;
        this->left = move(left);
        this->right = move(right);
    }

private:
    DataType infor;
    unique_ptr<Node<DataType>> left;
    unique_ptr<Node<DataType>> right;

    friend class BSTree<DataType>;
};

template <class DataType>
class BSTree
{
public:
    BSTree() = default;
    // 插入元素
    void insert_interface(const DataType &data);
    // 前序遍历
    void preOrder();

private:
    unique_ptr<Node<DataType>> root;
    int size{0};

    // 插入辅助函数
    void insertHelper(unique_ptr<Node<DataType>> &ptr, const DataType &data);
    // 得到节点树的平衡度
    int getBalance(const unique_ptr<Node<DataType>> &ptr);
    // 得到节点树的高度
    int getHeight(const unique_ptr<Node<DataType>> &ptr);
    // LL型,RR,LR,RL
    void turnLL(unique_ptr<Node<DataType>> &grandPtr);
    void turnRR(unique_ptr<Node<DataType>> &grandPtr);
    void turnLR(unique_ptr<Node<DataType>> &grandPtr);
    void turnRL(unique_ptr<Node<DataType>> &grandPtr);
    // 前序遍历
    void preOrderHelper(const unique_ptr<Node<DataType>> &ptr);
};

// 插入元素
template <class DataType>
void BSTree<DataType>::insert_interface(const DataType &data)
{
    if (this->root == nullptr)
    {
        this->root = make_unique<Node<DataType>>(data, nullptr, nullptr);
        return;
    }
    this->insertHelper(this->root, data);
}

// 插入元素辅助函数
template <class DataType>
void BSTree<DataType>::insertHelper(unique_ptr<Node<DataType>> &ptr, const DataType &data)
{
    if (ptr == nullptr)
    {
        ptr = make_unique<Node<DataType>>(data, nullptr, nullptr);
        this->size++;
        return;
    }
    // 判断分支
    if (data <= ptr->infor)
    {
        insertHelper(ptr->left, data);
    }
    else
    {
        insertHelper(ptr->right, data);
    }
    // AVL动态平衡树
    // 1.得到当前节点数的平衡度
    int balance = getBalance(ptr);
    if (balance > 1)
    {
        if (getBalance(ptr->left) == 1) // LL
        {
            turnLL(ptr);
        }
        else // LR
        {
            turnLR(ptr);
        }
    }
    if (balance < -1)
    {
        if (getBalance(ptr->right) == -1) // RR
        {
            turnRR(ptr);
        }
        else // RL
        {
            turnRL(ptr);
        }
    }
}

template <class DataType>
int BSTree<DataType>::getBalance(const unique_ptr<Node<DataType>> &ptr)
{
    if (ptr == nullptr)
        return 0;
    int leftTree = getHeight(ptr->left);
    int rightTree = getHeight(ptr->right);
    return leftTree - rightTree;
}

template <class DataType>
int BSTree<DataType>::getHeight(const unique_ptr<Node<DataType>> &ptr)
{
    if (ptr == nullptr)
        return 0;
    int left = getHeight(ptr->left);
    int right = getHeight(ptr->right);
    return left > right ? left + 1 : right + 1;
}

// 旋转
// LL型,RR,LR,RL
template <class DataType>
void BSTree<DataType>::turnLL(unique_ptr<Node<DataType>> &grandPtr)
{
    // 先处理节点孩子所有权
    auto gleft = move(grandPtr->left);
    grandPtr->left = move(gleft->right);
    gleft->right = move(grandPtr);
    grandPtr = move(gleft);
}

template <class DataType>
void BSTree<DataType>::turnRR(unique_ptr<Node<DataType>> &grandPtr)
{
    auto gright = move(grandPtr->right);
    grandPtr->right = move(gright->left);
    gright->left = move(grandPtr);
    grandPtr = move(gright);
}

template <class DataType>
void BSTree<DataType>::turnLR(unique_ptr<Node<DataType>> &grandPtr)
{
    turnRR(grandPtr->left);
    turnLL(grandPtr);
}

template <class DataType>
void BSTree<DataType>::turnRL(unique_ptr<Node<DataType>> &grandPtr)
{
    turnLL(grandPtr->right);
    turnRR(grandPtr);
}

// 前序遍历
template <class DataType>
void BSTree<DataType>::preOrderHelper(const unique_ptr<Node<DataType>> &ptr)
{
    if (ptr == nullptr)
        return;
    cout << ptr->infor << " ";
    preOrderHelper(ptr->left);
    preOrderHelper(ptr->right);
}
template <class DataType>
void BSTree<DataType>::preOrder()
{
    if (this->root == nullptr)
        return;
    preOrderHelper(this->root);
}
