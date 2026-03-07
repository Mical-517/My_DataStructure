#pragma once
#include <memory>
using namespace std;

template <class DataType>
class DLinkList;

template <class DataType>
class Node
{
public:
    Node() = default;
    Node(const DataType &infor, Node<DataType> *pre, std::unique_ptr<Node<DataType>> next)
    {
        this->infor = infor;
        this->pre = pre;
        this->next = move(next);
    }

private:
    // 受到智能指针特性的限制，一块堆内存智能有一个智能指针管理，所以node成员有一个智能指针用于管理，一个裸指针用于观测
    DataType infor;
    std::unique_ptr<Node<DataType>> next;
    Node<DataType> *pre{nullptr};

    friend class DLinkList<DataType>;
};

template <class DataType>
class DLinkList
{
public:
    // 默认构造函数
    DLinkList()
    {
        this->head = make_unique<Node<DataType>>();
        this->head->next = make_unique<Node<DataType>>();
        this->tail = this->head->next.get();
        tail->pre = this->head.get();
    }
    // 拷贝构造函数
    DLinkList(const DLinkList &other);
    // 析构函数
    ~DLinkList() = default;
    // 前插入
    void push_front(const DataType &data);
    // 后插入
    void push_back(const DataType &data);
    // 打印所有数据
    void print();

private:
    std::unique_ptr<Node<DataType>> head;
    Node<DataType> *tail{nullptr}; // 仅用来观测
};

// 拷贝构造函数
template <class DataType>
DLinkList<DataType>::DLinkList(const DLinkList &other) : DLinkList()
{
    // 深拷贝
    // 行动指针遍历other,应该用普通指针遍历，因为智能指针遍历是会释放空间
    Node<DataType> *otherPtr = other.head->next.get();
    Node<DataType> *currPtr = this->head.get();
    while (otherPtr != other.tail)
    {
        // 创建新节点，将当前节点的下个节点的所有权交给当前节点的next
        auto newNode = make_unique<Node<DataType>>(otherPtr->infor, currPtr, move(currPtr->next));
        tail->pre = newNode.get();
        // 转移当前节点所有权
        currPtr->next = move(newNode);
        // 更新行动指针
        otherPtr = otherPtr->next.get();
        currPtr = currPtr->next.get();
    }
}

// 前插入
template <class DataType>
void DLinkList<DataType>::push_front(const DataType &data)
{
    auto newNode = make_unique<Node<DataType>>(data, this->head.get(), move(head->next));
    newNode->next->pre = newNode.get();
    head->next = move(newNode);
}

// 后插入
template <class DataType>
void DLinkList<DataType>::push_back(const DataType &data)
{
    auto ptr = make_unique<Node<DataType>>(data, this->tail->pre, move(this->tail->pre->next));
    this->tail->pre = ptr.get();
    ptr->pre->next = move(ptr);
}

// 打印
template <class DataType>
void DLinkList<DataType>::print()
{
    auto ptr = this->head->next.get();
    while (ptr != this->tail)
    {
        cout << ptr->infor << " ";
        ptr = ptr->next.get();
    }
}
