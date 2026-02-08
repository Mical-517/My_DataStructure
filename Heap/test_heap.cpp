#include <iostream>
#include "Heap.h"
using namespace std;

int main()
{
    // 测试 MaxHeap<string>
    MaxHeap<string> h;
    string a = "apple", b = "banana", c = "cherry", d = "date";

    h.insert(a, 5);    // apple 优先级 5
    h.insert(b, 2);    // banana 优先级 2
    h.insert(c, 8);    // cherry 优先级 8
    h.insert(d, 7);    // date 优先级 7

    try {
        auto top = h.getMax();
        cout << "当前堆顶: " << top.getData() << " (priority=" << top.getPriority() << ")\n";
    } catch (const char* msg) {
        cout << "异常: " << msg << "\n";
    }

    cout << "逐个删除并打印新的堆顶:\n";
    while (true) {
        try {
            auto top = h.getMax();
            cout << "  堆顶: " << top.getData() << " (priority=" << top.getPriority() << ")\n";
            h.remove();
        } catch (...) {
            cout << "堆为空，测试结束\n";
            break;
        }
    }

    // 测试 MaxHeap<int>
    MaxHeap<int> hi;
    int x=10,y=30,z=20;
    hi.insert(x, x);
    hi.insert(y, y);
    hi.insert(z, z);

    try {
        auto top = hi.getMax();
        cout << "int 堆顶: " << top.getData() << " (priority=" << top.getPriority() << ")\n";
    } catch (...) {
        cout << "int 堆为空\n";
    }

    return 0;
}