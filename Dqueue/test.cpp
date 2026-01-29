#include <iostream>
#include <string>
#include "Dqueue.h"

using namespace std;

// 测试基本功能
void testBasicOperations() {
    cout << "========== 测试基本操作 ==========" << endl;
    
    // 创建双端队列
    Dqueue<int> dq;
    
    // 测试添加元素
    cout << "添加元素到队尾: ";
    for (int i = 1; i <= 5; i++) {
        dq.addTail(i);
        cout << i << " ";
    }
    cout << endl;
    
    cout << "当前队列: " << dq << endl;
    cout << "队列大小: " << dq.size() << endl;
    
    // 测试添加元素到队头
    cout << "添加元素到队头: ";
    for (int i = 10; i <= 15; i++) {
        dq.addHead(i);
        cout << i << " ";
    }
    cout << endl;
    
    cout << "当前队列: " << dq << endl;
    cout << "队列大小: " << dq.size() << endl;
    
    // 测试删除队头元素
    cout << "删除队头元素: ";
    for (int i = 0; i < 3; i++) {
        cout << dq.deleteHead() << " ";
    }
    cout << endl;
    
    cout << "当前队列: " << dq << endl;
    cout << "队列大小: " << dq.size() << endl;
    
    // 测试删除队尾元素
    cout << "删除队尾元素: ";
    for (int i = 0; i < 3; i++) {
        cout << dq.deleteTail() << " ";
    }
    cout << endl;
    
    cout << "当前队列: " << dq << endl;
    cout << "队列大小: " << dq.size() << endl;
    
    cout << endl;
}

// 测试随机访问
void testRandomAccess() {
    cout << "========== 测试随机访问 ==========" << endl;
    
    Dqueue<string> dq;
    
    // 添加一些字符串
    dq.addTail("Hello");
    dq.addTail("World");
    dq.addTail("C++");
    dq.addTail("Data");
    dq.addTail("Structure");
    
    cout << "当前队列: " << dq << endl;
    
    // 测试随机访问
    cout << "随机访问测试:" << endl;
    for (int i = 0; i < dq.size(); i++) {
        cout << "dq[" << i << "] = " << dq[i] << endl;
    }
    
    cout << endl;
}

// 测试边界情况
void testEdgeCases() {
    cout << "========== 测试边界情况 ==========" << endl;
    
    // 测试空队列
    Dqueue<int> emptyDq;
    cout << "空队列是否为空: " << (emptyDq.empty() ? "是" : "否") << endl;
    cout << "空队列大小: " << emptyDq.size() << endl;
    
    // 测试单个元素
    emptyDq.addTail(42);
    cout << "添加一个元素后: " << emptyDq << endl;
    cout << "队列是否为空: " << (emptyDq.empty() ? "是" : "否") << endl;
    
    // 删除单个元素
    int deleted = emptyDq.deleteHead();
    cout << "删除的元素: " << deleted << endl;
    cout << "删除后队列是否为空: " << (emptyDq.empty() ? "是" : "否") << endl;
    
    cout << endl;
}

// 测试拷贝构造函数
void testCopyConstructor() {
    cout << "========== 测试拷贝构造函数 ==========" << endl;
    
    Dqueue<int> original;
    for (int i = 1; i <= 5; i++) {
        original.addTail(i * 10);
    }
    
    cout << "原始队列: " << original << endl;
    
    // 使用拷贝构造函数
    Dqueue<int> copy(original);
    cout << "拷贝队列: " << copy << endl;
    
    // 修改原始队列
    original.addHead(100);
    original.deleteTail();
    
    cout << "修改后的原始队列: " << original << endl;
    cout << "拷贝队列(应不变): " << copy << endl;
    
    cout << endl;
}

// 测试clear函数
void testClear() {
    cout << "========== 测试clear函数 ==========" << endl;
    
    Dqueue<int> dq;
    for (int i = 1; i <= 10; i++) {
        dq.addTail(i);
    }
    
    cout << "清空前队列: " << dq << endl;
    cout << "清空前大小: " << dq.size() << endl;
    
    dq.clear();
    
    cout << "清空后队列是否为空: " << (dq.empty() ? "是" : "否") << endl;
    cout << "清空后大小: " << dq.size() << endl;
    
    // 清空后再次添加元素
    dq.addTail(100);
    dq.addHead(200);
    
    cout << "清空后重新添加元素: " << dq << endl;
    
    cout << endl;
}

int main() {
    try {
        testBasicOperations();
        testRandomAccess();
        testEdgeCases();
        testCopyConstructor();
        testClear();
        
        cout << "所有测试完成!" << endl;
    } catch (const char* msg) {
        cerr << "错误: " << msg << endl;
        return 1;
    }
    
    return 0;
}
