#pragma once 
#include <iostream>
#include <stack>
#include <queue>
#include <cmath>
using namespace std;

template<class DataType>
class BSTree;

//树节点
template <class DataType>
class BSTNode
{
    public:
    BSTNode():left(nullptr),right(nullptr){}
    BSTNode(const DataType& data,BSTNode<DataType>* left=nullptr,BSTNode<DataType>* right=nullptr)
    {
        this->data=data;
        this->left=left;
        this->right=right;
    }
    bool operator==(const BSTNode<DataType>& other)
    {
        return this->data==other.data&&this->left==other.left&&this->right==other.right;
    }
    private:
    DataType data;
    BSTNode<DataType>* left;
    BSTNode<DataType>* right;

    friend class BSTree<DataType>;
};

//二叉搜索树
template <class DataType>
class BSTree
{
    public:
    BSTree()=default;
    ~BSTree()
    {
        clear();
    }

    //添加节点
    void insert(const DataType& data);
    //删除节点,两种方式，合并删除与复制删除
    void eraseByMerge(const DataType& data);
    void eraseByCopy(const DataType& data);
    //递归遍历树的三种遍历方式
    void preOrder();
    void inOrder();
    void postOrder();
    //使用栈遍历树的三种方式
    void preOrderStack();
    void inOrderStack();
    void postOrderStack();
    //广度遍历树
    void levelOrder();
    //清空
    void clear();
    //DSW算法将二叉搜索树转换为平衡二叉树,一次性转换
    void DSW();

    private:
    BSTNode<DataType>* root=nullptr;
    int sum=0;

    //计算节点的度
    int getBalance(BSTNode<DataType>* node);
    //旋转节点
    void turnLL(BSTNode<DataType>*& node);
    void turnRR(BSTNode<DataType>*& node);
    void turnLR(BSTNode<DataType>*& node);
    void turnRL(BSTNode<DataType>*& node);

    //清空树辅助函数
    void clearHelper(BSTNode<DataType>* rootPtr);
    //添加节点辅助函数
    void insertHelper(BSTNode<DataType>*& rootPtr,const DataType& data);
    //递归遍历辅助函数
    void preOrderHelper(BSTNode<DataType>* rootPtr);
    void inOrderHelper(BSTNode<DataType>* rootPtr);
    void postOrderHelper(BSTNode<DataType>* rootPtr);
    //广度遍历辅助函数
    void levelOrderHelper(BSTNode<DataType>* rootPtr);
    //删除节点辅助函数
    void eraseByMergeHelper(BSTNode<DataType>*& rootPtr);
    //删除节点辅助函数
    void eraseByCopyHelper(BSTNode<DataType>*& rootPtr);
    //DSW辅助函数
    void DSWHelper(BSTNode<DataType>*& rootPtr);//改变树的结构，变为一整条右链
};



// 递归遍历实现
template<class DataType>
void BSTree<DataType>::preOrder()
{
    preOrderHelper(this->root);
    cout<<endl;
}

template<class DataType>
void BSTree<DataType>::preOrderHelper(BSTNode<DataType>* rootPtr)
{
    if(rootPtr==nullptr) return;
    cout<<rootPtr->data<<" ";
    preOrderHelper(rootPtr->left);
    preOrderHelper(rootPtr->right);
}

template<class DataType>
void BSTree<DataType>::inOrder()
{
    inOrderHelper(this->root);
    cout<<endl;
}

template<class DataType>
void BSTree<DataType>::inOrderHelper(BSTNode<DataType>* rootPtr)
{
    if(rootPtr==nullptr) return;
    inOrderHelper(rootPtr->left);
    cout<<rootPtr->data<<" ";
    inOrderHelper(rootPtr->right);
}

template<class DataType>
void BSTree<DataType>::postOrder()
{
    postOrderHelper(this->root);
    cout<<endl;
}

template<class DataType>
void BSTree<DataType>::postOrderHelper(BSTNode<DataType>* rootPtr)
{
    if(rootPtr==nullptr) return;
    postOrderHelper(rootPtr->left);
    postOrderHelper(rootPtr->right);
    cout<<rootPtr->data<<" ";
}

//使用栈遍历树的三种方式
template<class DataType>
void BSTree<DataType>::preOrderStack()
{
    if(this->root==nullptr) return;
    stack<BSTNode<DataType>*> treeStack;
    treeStack.push(this->root);
    while(!treeStack.empty())
    {
        BSTNode<DataType>* node=treeStack.top();
        treeStack.pop();
        cout<<node->data<<" ";
        if(node->right!=nullptr)
        {
            treeStack.push(node->right);
        }
        if(node->left!=nullptr)
        {
            treeStack.push(node->left);
        }
    }
    cout<<endl;
}

template<class DataType>
void BSTree<DataType>::inOrderStack()
{
    if(this->root==nullptr) return;
    stack<BSTNode<DataType>*> stackTree;
    BSTNode<DataType>* node = this->root;
    while (node != nullptr || !stackTree.empty())
    {
        while (node != nullptr)
        {
            stackTree.push(node);
            node = node->left;
        }
        node = stackTree.top();
        stackTree.pop();
        cout << node->data << " ";
        node = node->right;
    }
    cout<<endl;
}

template<class DataType>
void BSTree<DataType>::postOrderStack()
{
    if(this->root==nullptr) return;
    stack<BSTNode<DataType>*> treeStack;
    BSTNode<DataType>* node = this->root;
    BSTNode<DataType>* lastVisited = nullptr;
    while (node != nullptr || !treeStack.empty())
    {
        while (node != nullptr)
        {
            treeStack.push(node);
            node = node->left;
        }
        node = treeStack.top();
        if (node->right == nullptr || node->right == lastVisited)
        {
            cout << node->data << " ";
            treeStack.pop();
            lastVisited = node;
            node = nullptr;
        }
        else
        {
            node = node->right;
        }
    }
    cout<<endl;
}

//添加节点，实现动态调整树的平衡
template<class DataType>
void BSTree<DataType>::insertHelper(BSTNode<DataType>*& rootPtr,const DataType& data)
{
    if(rootPtr==nullptr)
    {
        rootPtr=new BSTNode<DataType>(data);
        this->sum++;
        return;
    }
    if(data>=rootPtr->data)
    {
        insertHelper(rootPtr->right,data);
    }
    else
    {
        insertHelper(rootPtr->left,data);
    }
    // 使用 AVL 树通过检查节点的平衡因子来判断是否需要旋转
    int balance = getBalance(rootPtr->left) - getBalance(rootPtr->right);

    if (balance > 1)
    {
        if (getBalance(rootPtr->left->left) >= getBalance(rootPtr->left->right))
        {
            // LL 型，进行右旋（代码中的 turnLL）
            turnLL(rootPtr);
        }
        else
        {
            // LR 型，先左旋后右旋（代码中的 turnLR）
            turnLR(rootPtr);
        }
    }
    else if (balance < -1)
    {
        if (getBalance(rootPtr->right->right) >= getBalance(rootPtr->right->left))
        {
            // RR 型，进行左旋（代码中的 turnRR）
            turnRR(rootPtr);
        }
        else
        {
            // RL 型，先右旋后左旋（代码中的 turnRL）
            turnRL(rootPtr);
        }
    }

}

template<class DataType>
void BSTree<DataType>:: insert(const DataType& data)
{
    insertHelper(this->root,data);
}

template<class DataType>
void BSTree<DataType>::turnRR(BSTNode<DataType>*& node)
{
    if (node == nullptr || node->right == nullptr) return;
    BSTNode<DataType>* rightNode = node->right;
    node->right = rightNode->left;
    rightNode->left = node;
    node = rightNode;
}

template<class DataType>
void BSTree<DataType>::turnLL(BSTNode<DataType>*& node)
{
    if (node == nullptr || node->left == nullptr) return;
    BSTNode<DataType>* leftNode = node->left;
    node->left = leftNode->right;
    leftNode->right = node;
    node = leftNode;
}

template<class DataType>
void BSTree<DataType>::turnLR(BSTNode<DataType>*& node)
{
    turnRR(node->left);
    turnLL(node);
}

template<class DataType>
void BSTree<DataType>::turnRL(BSTNode<DataType>*& node)
{
    turnLL(node->right);
    turnRR(node);
}

//清空函数
template<class DataType>
void BSTree<DataType>::clear()
{
    this->clearHelper(this->root);
    this->root=nullptr;
    this->sum=0;
}

//清空树辅助函数
template<class DataType>
void BSTree<DataType>::clearHelper(BSTNode<DataType>* rootPtr)
{
    if(rootPtr==nullptr) return;
    clearHelper(rootPtr->left);
    clearHelper(rootPtr->right);
    delete rootPtr;
}

template<class DataType>
int BSTree<DataType>::getBalance(BSTNode<DataType>* node)
{
    if(node==nullptr) return 0;
    int leftHeight=getBalance(node->left);
    int rightHeight=getBalance(node->right);
    return leftHeight>rightHeight?leftHeight+1:rightHeight+1;
}

template<class DataType>
void BSTree<DataType>::levelOrder()
{
    if(this->root==nullptr) return;
    levelOrderHelper(this->root);
}

template<class DataType>
void BSTree<DataType>::levelOrderHelper(BSTNode<DataType>* rootPtr)
{
    queue<BSTNode<DataType>*> treeQueue;
    treeQueue.push(rootPtr);
    while(!treeQueue.empty())
    {
        BSTNode<DataType>* node = treeQueue.front();
        treeQueue.pop();
        cout<<node->data<<" ";
        if(node->left!=nullptr) treeQueue.push(node->left);
        if(node->right!=nullptr) treeQueue.push(node->right);
    }
    cout<<endl;
}

//删除节点，并且还要调整树的平衡
template<class DataType>
void BSTree<DataType>::eraseByMerge(const DataType& data)
{
    BSTNode<DataType>* node=this->root,* prev=nullptr;
    while(node!=nullptr)
    {
        if(node->data==data) break;
        prev=node;
        if(data<node->data) node=node->left;
        else node=node->right;
    }
    if(node!=nullptr&&node->data==data)
    {
        if(node==this->root)
        {
            eraseByMergeHelper(this->root);
        }
        else
        {
            if(prev->left==node)
            {
                eraseByMergeHelper(prev->left);
            }
            else
            {
                eraseByMergeHelper(prev->right);
            }
        }
    }
    else
    {
        if(this->root!=nullptr)
        {
            cout<<"data"<<data<<"is not in the tree"<<endl;
        }
        else
        {
            cout<<"the tree is empty"<<endl;
        }
    }
}

template<class DataType>
void BSTree<DataType>::eraseByMergeHelper(BSTNode<DataType>*& rootPtr)
{
    BSTNode<DataType>* node=rootPtr;
    if(node!=nullptr)
    {
        if(node->right==nullptr)
        {
            rootPtr=rootPtr->left;
        }
        else
        {
            if(node->left==nullptr)
            {
                rootPtr=rootPtr->right;
            }
            else
            {
                //找到左子树的最右节点
                node=node->left;
                while(node->right!=nullptr)
                {
                    node=node->right;
                }
                //将待删除节点的右子树挂到左子树的最右节点上
                node->right=rootPtr->right;
                node=rootPtr;
                rootPtr=rootPtr->left;
            }
        }
        delete node;
        this->sum--;
    }
    this->DSW();
}

//DSW算法将二叉搜索树转换为平衡二叉树,一次性转换
template<class DataType>
void BSTree<DataType>::DSW()
{
    if (this->root == nullptr) return;

    // 1. 将树转换为右链 (Vine)
    DSWHelper(this->root);

    // 2. 压缩平衡
    int n = this->sum;
    // m 是最大的 2^k - 1，且 m <= n
    int m = 1;
    while (m <= n) {
        m = m * 2 + 1;
    }
    m /= 2;

    // 创建虚拟头节点简化旋转逻辑
    BSTNode<DataType> dummy;
    dummy.right = this->root;

    // 第一阶段：消除多余节点，使其数量达到 m
    BSTNode<DataType>* p = &dummy;
    for (int i = 0; i < n - m; i++)
    {
        turnRR(p->right); // 左旋
        p = p->right;
    }

    // 第二阶段：反复压缩，直到成为平衡树
    while (m > 1)
    {
        m /= 2;
        p = &dummy;
        for (int i = 0; i < m; i++)
        {
            turnRR(p->right); // 左旋
            p = p->right;
        }
    }

    this->root = dummy.right;
}

template<class DataType>
void BSTree<DataType>::DSWHelper(BSTNode<DataType>*& rootPtr)
{
    BSTNode<DataType> dummy;
    dummy.right = rootPtr;
    BSTNode<DataType>* p = &dummy;
    BSTNode<DataType>* q = p->right;

    while (q != nullptr)
    {
        if (q->left != nullptr)
        {
            // 如果有左子树，进行右旋 (turnLL)
            // 旋转后，p->right 会变成原来的 q->left
            turnLL(p->right);
            q = p->right; // 更新 q 指向新的局部根节点
        }
        else
        {
            // 如果没有左子树，向下移动
            p = q;
            q = q->right;
        }
    }
    rootPtr = dummy.right;
}
