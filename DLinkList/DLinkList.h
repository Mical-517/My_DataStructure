#pragma once
#include <iostream>
using namespace std;

template <class DataType>
class DLinkList;

template <class DataType>
class DLNode
{
    friend class DLinkList<DataType>;

public:
    DLNode() = default;
    DLNode(const DataType data, DLNode<DataType> *pre = nullptr, DLNode<DataType> *next = nullptr)
    {
        this->data = data;
        this->pre = pre;
        this->next = next;
    }

private:
    DataType data;
    DLNode<DataType> *pre = nullptr;
    DLNode<DataType> *next = nullptr;
};

template <class DataType>
class DLinkList
{
public:
    DLinkList()
    {
        head = new DLNode<DataType>();
        tail = new DLNode<DataType>();
        head->next = tail;
        tail->pre = head;
        number = 0;
    }
    DLinkList(const DLinkList<DataType> &temp); // 深拷贝实现
    ~DLinkList()
    {
        DLNode<DataType> *current = head;
        // 遍历所有节点（包括头尾哨兵），完整释放
        while (current != nullptr)
        {
            DLNode<DataType> *next_node = current->next; // 先保存下一个节点
            delete current;                              // 删除当前节点
            current = next_node;                         // 移动到下一个节点
        }
        // 清空指针，避免野指针
        head = tail = nullptr;
        number = 0;
    }
    bool empty() const;
    void insert(DataType infor);
    void erase(DataType infor);
    int size() const;
    void print() const;
private:
    DLNode<DataType> *head = nullptr;
    DLNode<DataType> *tail = nullptr;
    int number = 0;
};

template <class DataType>
DLinkList<DataType>::DLinkList(const DLinkList<DataType> &temp)
{
    head = new DLNode<DataType>();
    tail = new DLNode<DataType>();
    head->next = tail;
    tail->pre = head;
    DLNode<DataType> *p = nullptr;
    DLNode<DataType> *q = this->head;
    DLNode<DataType> *temp_ptr = temp.head->next;
    while (temp_ptr != temp.tail)
    {
        p = new DLNode<DataType>(temp_ptr->data, q, this->tail);
        q->next = p;
        q = p;
        temp_ptr = temp_ptr->next;
    }
    this->number=temp.size();
}

template <class DataType>
bool DLinkList<DataType>::empty() const
{
    return this->number == 0 ? true : false;
}

template <class DataType>
void DLinkList<DataType>::insert(DataType infor)
{
    DLNode<DataType> *p = new DLNode<DataType>(infor);
    DLNode<DataType> *q = this->tail->pre;
    q->next = p;
    p->next = this->tail;
    p->pre = q;
    tail->pre = p;
    this->number++;
}

template <class DataType>
void DLinkList<DataType>::erase(DataType infor)
{
    if (this->empty())
        return;
    DLNode<DataType> *p = this->head;
    while (p != this->tail)
    {
        if (p->next->data == infor)
            break;
        p = p->next;
    }
    if (p != this->tail)
    {
        p->next->next->pre = p;
        DLNode<DataType> *q = p->next;
        p->next = p->next->next;
        delete q;
        this->number--;
    }
    return;
}

template <class DataType>
int DLinkList<DataType>::size() const
{
    return this->number;
}

// 遍历打印元素
template <class DataType>
void DLinkList<DataType>::print() const
{
    if (empty())
    {
        cout << "链表为空" << endl;
        return;
    }
    DLNode<DataType>* p = head->next;
    cout << "链表元素：";
    while (p != tail)
    {
        cout << p->data << " ";
        p = p->next;
    }
    cout << endl;
}