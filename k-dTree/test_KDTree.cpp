#include <iostream>
#include "K-dTree.h"

int main()
{
    // 使用非 const 变量以匹配头文件中使用的非 const 引用参数签名
    KDTree<int> tree;
    int x1 = 3,  y1 = 6;
    int x2 = 17, y2 = 15;
    int x3 = 13, y3 = 15;
    int x4 = 6,  y4 = 12;
    int x5 = 9,  y5 = 1;
    int x6 = 2,  y6 = 7;
    int x7 = 10, y7 = 19;

    // 插入若干点
    tree.insert(x1,y1);
    tree.insert(x2,y2);
    tree.insert(x3,y3);
    tree.insert(x4,y4);
    tree.insert(x5,y5);
    tree.insert(x6,y6);
    tree.insert(x7,y7);

    // 搜索存在的点
    if (tree.search(x7,y7) != nullptr)
        std::cout << "Found point (" << x7 << "," << y7 << "), depth parity: " << "\n";
    else
        std::cout << "Point (" << x7 << "," << y7 << ") not found\n";

    // 搜索不存在的点
    int xa = 100, ya = 100;
    if (tree.search(xa,ya) != nullptr)
        std::cout << "Found point (" << xa << "," << ya << ")\n";
    else
        std::cout << "Point (" << xa << "," << ya << ") not found\n";

    // 删除一个点并再次搜索
    tree.erase(x7,y7);
    if (tree.search(x7,y7) != nullptr)
        std::cout << "After erase: still found (" << x7 << "," << y7 << ")\n";
    else
        std::cout << "After erase: (" << x7 << "," << y7 << ") not found\n";


}