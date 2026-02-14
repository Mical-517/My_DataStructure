#include "diGraph.hpp"
#include <iostream>
#include <string>

int main()
{
    using namespace std;

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

    return 0;
}