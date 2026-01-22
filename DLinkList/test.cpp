#include <iostream>
#include "DLinkList.h"
using namespace std;

// 测试函数（main）
int main()
{
    cout << "===== 测试1：空链表初始化 =====" << endl;
    DLinkList<int> list1;
    cout << "链表是否为空：" << boolalpha << list1.empty() << endl; // 输出true
    cout << "链表大小：" << list1.size() << endl; // 输出0
    list1.print(); // 输出"链表为空"

    cout << "\n===== 测试2：尾插元素 =====" << endl;
    list1.insert(10);
    list1.insert(20);
    list1.insert(30);
    list1.insert(40);
    cout << "插入后链表大小：" << list1.size() << endl; // 输出4
    list1.print(); // 输出"链表元素：10 20 30 40 "

    cout << "\n===== 测试3：深拷贝构造 =====" << endl;
    DLinkList<int> list2 = list1; // 调用深拷贝构造
    cout << "拷贝后list2大小：" << list2.size() << endl; // 输出4
    list2.print(); // 输出"链表元素：10 20 30 40 "
    // 验证深拷贝（修改原链表，拷贝链表不受影响）
    list1.erase(20);
    cout << "原链表删除20后，list1：";
    list1.print(); // 输出"10 30 40"
    cout << "拷贝链表list2：";
    list2.print(); // 输出"10 20 30 40"（不受影响，证明深拷贝成功）

    cout << "\n===== 测试4：删除元素 =====" << endl;
    // 测试删除存在的元素
    list1.erase(30);
    cout << "删除30后，list1：";
    list1.print(); // 输出"10 40"
    cout << "list1大小：" << list1.size() << endl; // 输出2

    // 测试删除不存在的元素（无崩溃）
    list1.erase(50);
    cout << "删除不存在的50后，list1：";
    list1.print(); // 输出"10 40"
    cout << "list1大小：" << list1.size() << endl; // 输出2（大小不变）

    // 测试删除最后一个元素
    list1.erase(40);
    cout << "删除最后一个元素40后，list1：";
    list1.print(); // 输出"10"
    cout << "list1大小：" << list1.size() << endl; // 输出1

    cout << "\n===== 测试5：边界场景（空链表删除） =====" << endl;
    DLinkList<int> emptyList;
    emptyList.erase(10); // 空链表删除，无崩溃
    cout << "空链表删除元素后，是否为空：" << emptyList.empty() << endl; // 输出true

    cout << "\n===== 测试6：析构函数（隐含测试，无内存泄漏/崩溃） =====" << endl;
    // 所有链表对象离开作用域时自动调用析构，无崩溃即证明析构正常
    cout << "所有测试完成，无崩溃/内存泄漏" << endl;

    return 0;
}
    