#pragma once
#include <memory>
#include <iostream> // For cout in print()
#include <iterator> // For iterator traits
using namespace std;

template <class DataType>
class DLinkList;

template <class T>
class MyIterator;

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
    friend class MyIterator<DataType>;
};

// 自定义list的迭代器类型
template <class T>
class MyIterator : public std::iterator<std::bidirectional_iterator_tag, T>
{
    /*
    std::iterator 是一个辅助模板类（在 C++17 中被弃用，但其思想仍然是理解 traits 的核心）。它的定义大致如下：
template<class Category, class T, class Distance = ptrdiff_t,
         lass Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category  iterator_category;
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
};
当你写 public std::iterator<std::bidirectional_iterator_tag, T> 时，你其实在说：

iterator_category: 我的迭代器是 std::bidirectional_iterator_tag（双向迭代器）。
value_type: 我的迭代器指向的值类型是 T（也就是 DLinkList 的 DataType）。
difference_type: 使用默认的 ptrdiff_t。
pointer: 使用默认的 T*。
reference: 使用默认的 T&。
这样，你的 MyIterator 类内部就自动拥有了这五个 typedef。

    */
public:
    // 添加构造函数
    MyIterator(Node<T> *p = nullptr) : ptr(p) {}

    // 重载*，返回指向对象的引用
    T &operator*() const
    {
        return ptr->infor; // 修正：返回节点的数据
    }

    // 建议增加 -> 运算符
    T *operator->() const
    {
        return &(ptr->infor);
    }

    // 前置++，返回迭代器本身的引用
    MyIterator<T> &operator++()
    {
        ptr = ptr->next.get();
        return *this;
    }

    // 后置++,返回迭代器自身拷贝
    MyIterator<T> operator++(int)
    {
        MyIterator<T> temp = *this; // 修正：创建当前迭代器的副本
        ptr = ptr->next.get();
        return temp; // 修正：返回副本
    }

    // 添加 operator-- 以支持双向迭代
    MyIterator<T> &operator--()
    {
        ptr = ptr->pre;
        return *this;
    }

    MyIterator<T> operator--(int)
    {
        MyIterator<T> temp = *this;
        ptr = ptr->pre;
        return temp;
    }

    // 添加比较运算符
    bool operator==(const MyIterator<T> &other) const
    {
        return ptr == other.ptr;
    }

    bool operator!=(const MyIterator<T> &other) const
    {
        return ptr != other.ptr;
    }

private:
    Node<T> *ptr{nullptr};
};

template <class DataType>
class DLinkList
{
public:
    // 定义迭代器类型别名
    using iterator = MyIterator<DataType>;

    // 默认构造函数
    DLinkList()
    {
        this->head = make_unique<Node<DataType>>();
        this->head->next = make_unique<Node<DataType>>();
        this->tail = this->head->next.get();
        tail->pre = this->head.get();
    }

    // 删除成员变量 iterator，替换为 begin() 和 end() 方法
    iterator begin()
    {
        return iterator(head->next.get());
    }

    iterator end()
    {
        return iterator(tail);
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
    // 使用迭代器进行打印
    for (iterator it = begin(); it != end(); ++it)
    {
        cout << *it << " ";
    }
    // 修正：加上换行
    cout << endl;
}
