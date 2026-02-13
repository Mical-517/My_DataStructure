/*
    Generalized Splay Tree这是一个自适应的二叉搜索树，主要使用半张开策略来体现自适应树的思想，进而提要查找策略
*/

#pragma once
#include <iostream>
using namespace std;

template <class DataType>
class SplayTree;

template <class DataType>
class SplayNode
{
public:
    friend class SplayTree<DataType>;
    SplayNode() : left(nullptr), right(nullptr), parent(nullptr) {}
    // 有参使用const引用形式
    SplayNode(const DataType &key, SplayNode<DataType> *p = nullptr, SplayNode<DataType> *l = nullptr, SplayNode<DataType> *r = nullptr) : key(key), parent(p), left(l), right(r) {}
    // 访问私有成员函数接口
    DataType &value() { return this->key; }
    const DataType &value() const { return this->key; }

private:
    SplayNode<DataType> *left;
    SplayNode<DataType> *right;
    SplayNode<DataType> *parent;
    DataType key;
};

template <class DataType>
class SplayTree
{
public:
    SplayTree() : root(nullptr) {}
    void inorder();                                    // 中序遍历
    virtual void visit(SplayNode<DataType> *node) {}   // 访问函数
    void insert(const DataType &data);                 // 访问函数
    SplayNode<DataType> *search(const DataType &data); // 查找函数,注意自适应树要进行splay操作
private:
    SplayNode<DataType> *root;
    void inorder(SplayNode<DataType> *node);                                                                                      // 中序遍历辅助函数
    void semisplay(SplayNode<DataType> *node);                                                                                    // 半张开操作
    void rotateR(SplayNode<DataType> *node);                                                                                      // 右旋操作
    void rotateL(SplayNode<DataType> *node);                                                                                      // 左旋操作
    void continueRotation(SplayNode<DataType> *gr, SplayNode<DataType> *par, SplayNode<DataType> *ch, SplayNode<DataType> *desc); // 继续旋转操作，处理父节点和祖父节点的关系
};

template <class DataType>
void SplayTree<DataType>::inorder()
{
    inorder(this->root);
}

template <class DataType>
void SplayTree<DataType>::inorder(SplayNode<DataType> *node)
{
    if (node == nullptr)
        return;
    inorder(node->left);
    this->visit(node);
    inorder(node->right);
}

template <class DataType>
void SplayTree<DataType>::insert(const DataType &data)
{
    SplayNode<DataType> *curr = this->root, *pre = nullptr, *newNode = nullptr;
    while (curr != nullptr)
    {
        if (curr->key == data)
            break;
        else if (curr->key > data)
        {
            pre = curr;
            curr = curr->left;
        }
        else
        {
            pre = curr;
            curr = curr->right;
        }
    }
    if (newNode = new SplayNode<DataType>(data, pre))
    {
        if (pre == nullptr)
            this->root = newNode;
        else if (pre->key > data)
            pre->left = newNode;
        else
            pre->right = newNode;
    }
    else
    {
        cerr << "内存分配失败!" << endl;
        return;
    }
}

template <class DataType>
SplayNode<DataType> *SplayTree<DataType>::search(const DataType &data)
{
    SplayNode<DataType> *curr = this->root;
    while (curr != nullptr)
    {
        if (curr->key == data)
        {
            this->semisplay(curr);
            return curr;
        }
        else if (curr->key > data)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return nullptr;
}

template <class DataType>
void SplayTree<DataType>::semisplay(SplayNode<DataType> *node)
{
    while (node != this->root)
    {
        if (node->parent->parent == nullptr) // zig操作：单旋转
        {
            if (node->parent->left == node) // 右旋
            {
                rotateR(node);
            }
            else
                rotateL(node);
        }
        else if (node->parent->left == node)
        {
            if (node->parent->parent->left == node->parent) // 左左情况
            {
                rotateR(node->parent);
                node = node->parent;
            }
            else
            {
                rotateR(node);
                rotateL(node);
            }
        }
        else
        {
            if (node->parent->parent->right == node->parent) // 右右情况
            {
                rotateL(node->parent);
                node = node->parent;
            }
            else
            {
                rotateL(node);
                rotateR(node);
            }
        }
        if (this->root == nullptr)
            this->root = node;
    }
}

template <class DataType>
void SplayTree<DataType>::rotateR(SplayNode<DataType> *node)
{
    // 处理关键逻辑，处理有关parent指向使用一个函数统一处理
    node->parent->left = node->right;
    node->right = node->parent;
    this->continueRotation(node->parent->parent, node->parent, node, node->right->left);
}

template <class DataType>
void SplayTree<DataType>::rotateL(SplayNode<DataType> *node)
{
    // 处理旋转节点的逻辑，处理有关parent指向使用一个函数统一处理
    node->parent->right = node->left;
    node->left = node->parent;
    this->continueRotation(node->parent->parent, node->parent, node, node->left->right);
}

template <class DataType>
void SplayTree<DataType>::continueRotation(SplayNode<DataType> *gr, SplayNode<DataType> *par, SplayNode<DataType> *ch, SplayNode<DataType> *desc)
{
    if (gr != nullptr)
    {
        if (gr->left == ch->parent)
            gr->left = ch;
        else
            gr->right = ch;
    }
    else

    {
        this->root = ch;
    }
    if (desc != nullptr)
        desc->parent = par;
    par->parent = ch;
    ch->parent = gr;
}