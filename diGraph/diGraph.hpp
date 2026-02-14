#pragma once
#include <iostream>
using namespace std;

template <class DataType, class WeightType>
class DiGraph;

// 边集节点
template <class WeightType>
class EdgeNode
{
public:
    EdgeNode(int headIndex = 0, int tailIndex = 0,
             EdgeNode<WeightType> *headEdge = nullptr,
             EdgeNode<WeightType> *tailEdge = nullptr,
             WeightType weight = WeightType());
    EdgeNode() = default;

private:
    // 让所有 DiGraph 模板实例成为友元（避免特化在类内引起编译器错误）
    template <class D, class W>
    friend class DiGraph;

    int headIndex;
    int tailIndex;
    EdgeNode<WeightType> *headEdge;
    EdgeNode<WeightType> *tailEdge;
    WeightType weight;
};

// 顶点节点
template <class DataType, class WeightType>
class VertexNode
{
public:
    VertexNode(DataType data = DataType(),
               EdgeNode<WeightType> *firstIn = nullptr,
               EdgeNode<WeightType> *firstOut = nullptr);

private:
    DataType data;
    EdgeNode<WeightType> *firstIn;
    EdgeNode<WeightType> *firstOut;
    friend class DiGraph<DataType, WeightType>;
};

// 十字链表有向图
template <class DataType, class WeightType>
class DiGraph
{
public:
    DiGraph(int vertexCapacity = 0);
    ~DiGraph()
    {
        this->clear();
        if (vertexArry)
            delete[] vertexArry;
    }

    // 添加顶点
    void addVertex(const DataType data);
    // 添加边
    void addEdge(const DataType head, const DataType tail, const WeightType weight);
    // 删除边
    void removeEdge(const DataType head, const DataType tail);
    // 清空函数
    void clear();
    // 打印函数
    void print();
    // 出度与入度函数
    int outDegree(const DataType data);
    int inDegree(const DataType data);

private:
    VertexNode<DataType, WeightType> *vertexArry = nullptr;
    int vertexNum = 0;
    int capacity = 0;
    // 利用顶点存储信息查询对应节点下标
    int search(const DataType data);
};

// 边节点构造函数
template <class WeightType>
EdgeNode<WeightType>::EdgeNode(int headIndex, int tailIndex,
                               EdgeNode<WeightType> *headEdge,
                               EdgeNode<WeightType> *tailEdge,
                               WeightType weight)
    : headIndex(headIndex), tailIndex(tailIndex),
      headEdge(headEdge), tailEdge(tailEdge), weight(weight)
{
}

// 顶点节点构造函数
template <class DataType, class WeightType>
VertexNode<DataType, WeightType>::VertexNode(DataType data,
                                             EdgeNode<WeightType> *firstIn,
                                             EdgeNode<WeightType> *firstOut)
    : data(data), firstIn(firstIn), firstOut(firstOut)
{
}

// 十字链表有向图构造函数
template <class DataType, class WeightType>
DiGraph<DataType, WeightType>::DiGraph(int vertexCapacity)
{
    this->vertexNum = 0;
    this->capacity = vertexCapacity > 0 ? vertexCapacity : 0;
    if (this->capacity > 0)
        vertexArry = new VertexNode<DataType, WeightType>[this->capacity];
    else
        vertexArry = nullptr;

    // (new 会在失败时抛出异常，保留检查以防编译器行为不同)
    if (this->capacity > 0 && vertexArry == nullptr)
    {
        cerr << "Memory allocation failed\n";
        exit(1);
    }
}

// 添加边
template <class DataType, class WeightType>
void DiGraph<DataType, WeightType>::addEdge(const DataType head, const DataType tail, const WeightType weight)
{
    int headIndex = search(head);
    int tailIndex = search(tail);
    if (headIndex == -1 || tailIndex == -1)
    {
        cerr << "Vertex Not Found\n";
        return;
    }
    EdgeNode<WeightType> *newEdge =
        new EdgeNode<WeightType>(headIndex, tailIndex,
                                 this->vertexArry[headIndex].firstOut,
                                 this->vertexArry[tailIndex].firstIn,
                                 weight);
    if (newEdge == nullptr)
    {
        cerr << "Memory allocation failed\n";
        exit(1);
    }
    this->vertexArry[headIndex].firstOut = newEdge;
    this->vertexArry[tailIndex].firstIn = newEdge;
}

// search函数
template <class DataType, class WeightType>
int DiGraph<DataType, WeightType>::search(const DataType data)
{
    for (int i = 0; i < this->vertexNum; i++)
    {
        if (vertexArry[i].data == data)
            return i;
    }
    return -1;
}

// 清空函数
template <class DataType, class WeightType>
void DiGraph<DataType, WeightType>::clear()
{
    if (!vertexArry)
        return;
    EdgeNode<WeightType> *curr = nullptr, *pre = nullptr;
    for (int i = 0; i < this->vertexNum; i++)
    {
        curr = vertexArry[i].firstOut;
        while (curr != nullptr)
        {
            pre = curr;
            curr = curr->headEdge;
            delete pre;
        }
        vertexArry[i].firstOut = nullptr;
        vertexArry[i].firstIn = nullptr;
    }
    vertexNum = 0;
}

// 打印函数
template <class DataType, class WeightType>
void DiGraph<DataType, WeightType>::print()
{
    if (!vertexArry)
    {
        cout << "(empty graph)\n";
        return;
    }
    EdgeNode<WeightType> *curr = nullptr;
    for (int i = 0; i < this->vertexNum; i++)
    {
        cout << vertexArry[i].data << ": ";
        curr = vertexArry[i].firstOut;
        while (curr != nullptr)
        {
            cout << vertexArry[curr->tailIndex].data << "(" << curr->weight << ") ";
            curr = curr->headEdge;
        }
        cout << " OutDegree:" << outDegree(vertexArry[i].data)
             << " InDegree:" << inDegree(vertexArry[i].data) << "\n";
    }
}

// 出入度函数
template <class DataType, class WeightType>
int DiGraph<DataType, WeightType>::outDegree(const DataType data)
{
    int index = search(data);
    if (index == -1)
    {
        cerr << "Vertex Not Found\n";
        return -1;
    }
    int degree = 0;
    EdgeNode<WeightType> *curr = vertexArry[index].firstOut;
    while (curr != nullptr)
    {
        degree++;
        curr = curr->headEdge;
    }
    return degree;
}

template <class DataType, class WeightType>
int DiGraph<DataType, WeightType>::inDegree(const DataType data)
{
    int index = search(data);
    if (index == -1)
    {
        cerr << "Vertex Not Found\n";
        return -1;
    }
    int degree = 0;
    EdgeNode<WeightType> *curr = vertexArry[index].firstIn;
    while (curr != nullptr)
    {
        degree++;
        curr = curr->tailEdge;
    }
    return degree;
}

// 添加顶点函数
template <class DataType, class WeightType>
void DiGraph<DataType, WeightType>::addVertex(const DataType data)
{
    if (this->capacity == 0 || this->vertexNum == this->capacity)
    {
        cerr << "Vertex Capacity Exceeded\n";
        return;
    }
    if (search(data) != -1)
    {
        cerr << "Vertex Already Exists\n";
        return;
    }
    vertexArry[this->vertexNum].data = data;
    vertexArry[this->vertexNum].firstIn = nullptr;
    vertexArry[this->vertexNum].firstOut = nullptr;
    ++this->vertexNum;
}
