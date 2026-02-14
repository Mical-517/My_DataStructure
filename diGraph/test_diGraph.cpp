#include "diGraph.hpp"
#include <iostream>
#include <string>
using namespace std;

// 测试插入删除函数
void test1()
{
    // 创建图（预期构造函数接受容量参数）
    DiGraph<string, int> g(10);

    // 添加顶点
    g.addVertex("A");
    g.addVertex("B");
    g.addVertex("C");

    // 添加有向边（head -> tail, weight）
    g.addEdge("A", "B", 5);
    g.addEdge("A", "C", 3);
    g.addEdge("B", "C", 2);

    cout << "Initial graph:\n";
    g.print();

    cout << "\nDegrees:\n";
    cout << "A out=" << g.outDegree("A") << " in=" << g.inDegree("A") << "\n";
    cout << "B out=" << g.outDegree("B") << " in=" << g.inDegree("B") << "\n";
    cout << "C out=" << g.outDegree("C") << " in=" << g.inDegree("C") << "\n";

    cout << "\nClear graph and print again:\n";
    g.clear();
    g.print();
}

// 辅助函数：根据Dijkstra算法的结果构建路径字符串
string buildPath(int targetIndex, const vector<int> &path, const DiGraph<string, int> &g)
{
    if (targetIndex == -1)
    {
        return "";
    }
    string basePath = buildPath(path[targetIndex], path, g);
    return basePath + g.getVertexData(targetIndex) + " -> ";
}

// 测试Dijkstra算法
void test2()
{

    DiGraph<string, int> g(10);
    g.addVertex("A"); // 索引 0
    g.addVertex("B"); // 索引 1
    g.addVertex("C"); // 索引 2
    g.addVertex("D"); // 索引 3
    g.addVertex("E"); // 索引 4

    g.addEdge("A", "B", 10);
    g.addEdge("A", "C", 3);
    g.addEdge("B", "C", 1);
    g.addEdge("B", "D", 2);
    g.addEdge("C", "B", 4);
    g.addEdge("C", "D", 8);
    g.addEdge("C", "E", 2);
    g.addEdge("D", "E", 7);
    g.addEdge("E", "D", 9);

    cout << "Graph structure:\n";
    g.print();
    cout << "\n-----------------------------------\n";

    string startNode = "A";
    auto result = g.dijkstra(startNode);
    cout << "Shortest distances from " << startNode << ":\n";
    for (int i = 0; i < result.dist.size(); i++)
    {
        string targetNode = g.getVertexData(i);
        cout << "To " << targetNode << ": \n";
        if (result.dist[i] == numeric_limits<int>::max())
        {
            cout << " Distance: Unreachable\n";
            cout << " Path: None\n";
        }
        else
        {
            cout << "Distance: " << result.dist[i] << "\n";
            // 构建并打印路径
            string pathStr = buildPath(i, result.path, g);
            if (!pathStr.empty())
            {
                // 去掉最后的" -> "
                pathStr = pathStr.substr(0, pathStr.size() - 4);
            }
            cout << " Path:     " << pathStr << "\n";
        }
        cout << "------------------------------------\n";
    }
}
int main()
{

    test2();

    return 0;
}