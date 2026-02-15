#pragma once
#include <iostream>
#include <vector>     // 新增：用于存储结果
#include <queue>      // 新增：用于优先队列
#include <limits>     // 新增：用于表示无穷大
#include <functional> // 新增：用于优先队列的比较器

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
    int outDegree(const DataType data);
    int inDegree(const DataType data);

    // 新增：Dijkstra 算法的返回结果结构体
    struct DijkstraResult
    {
        vector<WeightType> dist; // 存储从源点到各顶点的最短距离
        vector<int> path;        // 存储最短路径的前驱顶点索引
    };

    // 新增：Dijkstra 算法的声明
    DijkstraResult dijkstra(const DataType startVertex);

    // 新增：利用下标可以获取顶点的值
    DataType getVertexData(int index) const;

    // 新增WFIalgorithm返回结构体（多源最短路径）
    struct WFIResult
    {
        // 存储各个顶点到各个顶点的最短路径
        vector<vector<WeightType>> dist;
        // 存出路径
        vector<vector<int>> path;
    };
    // 新增：WFI算法声明
    WFIResult WFIalgorithm();
    // 将十字链表转换为邻接矩阵，方便WFI算法的实现
    vector<vector<WeightType>> toAdjacencyMarix();

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

/*
// 新增：Dijkstra 算法的实现
template <class DataType, class WeightType>
typename DiGraph<DataType, WeightType>::DijkstraResult DiGraph<DataType, WeightType>::dijkstra(const DataType startVertex)
{
    DijkstraResult result;
    result.dist.resize(vertexNum, numeric_limits<WeightType>::max());
    result.path.resize(vertexNum, -1);

    int startIndex = search(startVertex);
    if (startIndex == -1)
    {
        cerr << "Start vertex not found\n";
        return result;
    }
    result.dist[startIndex] = 0;

    using PII = pair<WeightType, int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    pq.push({0, startIndex});

    while (!pq.empty())
    {
        WeightType currDist = pq.top().first;
        int currIndex = pq.top().second;
        pq.pop();

        if (currDist > result.dist[currIndex])
            continue;

        EdgeNode<WeightType> *edge = vertexArry[currIndex].firstOut;
        while (edge != nullptr)
        {
            int nextIndex = edge->tailIndex;
            WeightType newDist = currDist + edge->weight;

            if (newDist < result.dist[nextIndex])
            {
                result.dist[nextIndex] = newDist;
                result.path[nextIndex] = currIndex;
                pq.push({newDist, nextIndex});
            }
            edge = edge->headEdge;
        }
    }

    return result;
}
*/

// Dijkstra 算法实现
template <class DataType, class WeightType>
typename DiGraph<DataType, WeightType>::DijkstraResult
DiGraph<DataType, WeightType>::dijkstra(const DataType startVertex)
{
    DijkstraResult result;
    // 初始化result
    result.dist.resize(this->vertexNum, numeric_limits<WeightType>::max());
    result.path.resize(this->vertexNum, -1);
    int startIndex = search(startVertex);
    if (startIndex == -1)
    {
        cerr << "Start vertex not found\n";
        return result;
    }
    result.dist[startIndex] = 0;
    // 优先队列，存储<距离,顶点索引>，始终将最小的距离放在前面，将其节点下标作为中转站更新到其他邻接节点的最短距离
    using Pair = pair<WeightType, int>;
    priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
    pq.push({0, startIndex});
    while (!pq.empty())
    {
        // 得到当前记录的最短路径以及索引
        WeightType currDist = pq.top().first;
        int currIndex = pq.top().second;
        pq.pop();
        // 判断当前记录的最短路径是否已经过时（如果当前记录的距离大于result中存储的距离，说明已经有更短的路径被更新了）
        if (currDist > result.dist[currIndex])
        {
            continue;
        }
        // 当前索引对应的节点作为中转站更新其所有邻接节点的最短路径
        EdgeNode<WeightType> *edge = this->vertexArry[currIndex].firstOut;
        while (edge != nullptr)
        {
            // 得到邻接节点的索引以及通过当前节点到达邻接节点的距离
            int nextIndex = edge->tailIndex;
            int nextDist = edge->weight;
            if (result.dist[nextIndex] > result.dist[currIndex] + nextDist)
            {
                // 如果当前记录的到邻接节点距离大于通过当前节点再到邻接节点的距离就更新距离
                result.dist[nextIndex] = result.dist[currIndex] + nextDist;
                // 更新到达邻接节点的前驱节点
                result.path[nextIndex] = currIndex;
                // 将更新后的距离和邻接节点索引加入优先队列
                pq.push({result.dist[nextIndex], nextIndex});
            }
            edge = edge->headEdge;
        }
    }
    return result;
}

// 新增：利用下标获取顶点数据
template <class DataType, class WeightType>
DataType DiGraph<DataType, WeightType>::getVertexData(int index) const
{
    if (index < 0 || index >= this->vertexNum)
    {
        cerr << "Index out of bounds\n";
        return DataType();
    }
    return this->vertexArry[index].data;
}

// WFI算法实现多源最短路径
template <class DataType, class WeightType>
typename DiGraph<DataType, WeightType>::WFIResult
DiGraph<DataType, WeightType>::WFIalgorithm()
{
    WFIResult result;
    int vNum = this->vertexNum;
    // 初始化result
    result.dist.resize(vNum, vector<WeightType>(vNum, numeric_limits<WeightType>::max()));
    result.path.resize(vNum, vector<int>(vNum, -1));
    vector<vector<WeightType>> adjMatrix = this->toAdjacencyMarix();

    // 初始化 dist 和 path 矩阵
    for (int i = 0; i < vNum; i++)
    {
        for (int j = 0; j < vNum; j++)
        {
            if (i == j)
            {
                result.dist[i][j] = 0;
            }
            else if (adjMatrix[i][j] != numeric_limits<WeightType>::max())
            {
                result.dist[i][j] = adjMatrix[i][j];
                result.path[i][j] = i; // 从 i 到 j 的直接路径，j 的前驱是 i
            }
        }
    }

    // Floyd-Warshall 算法核心
    // k 是中转顶点
    for (int k = 0; k < vNum; k++)
    {
        // i 是起始顶点
        for (int i = 0; i < vNum; i++)
        {
            // j 是结束顶点
            for (int j = 0; j < vNum; j++)
            {
                // 防止溢出
                if (result.dist[i][k] != numeric_limits<WeightType>::max() &&
                    result.dist[k][j] != numeric_limits<WeightType>::max() &&
                    result.dist[i][j] > result.dist[i][k] + result.dist[k][j])
                {
                    result.dist[i][j] = result.dist[i][k] + result.dist[k][j];
                    // 路径从 i -> ... -> k -> ... -> j
                    // 所以到达 j 的前驱节点和从 k 到达 j 的前驱节点是一样的
                    result.path[i][j] = result.path[k][j];
                }
            }
        }
    }
    return result;
}

template <class DataType, class WeightType>
vector<vector<WeightType>> DiGraph<DataType, WeightType>::toAdjacencyMarix()
{
    vector<vector<WeightType>> adjMarix(this->vertexNum, vector<WeightType>(this->vertexNum, numeric_limits<WeightType>::max()));
    for (int i = 0; i < this->vertexNum; i++)
    {
        adjMarix[i][i] = 0; // 顶点到自身的距离为0
        EdgeNode<WeightType> *edge = this->vertexArry[i].firstOut;
        while (edge != nullptr)
        {
            adjMarix[i][edge->tailIndex] = edge->weight;
            edge = edge->headEdge;
        }
    }
    return adjMarix;
}
