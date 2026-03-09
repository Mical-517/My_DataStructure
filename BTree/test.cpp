#include <iostream>
#include "BTree.hpp"

using namespace std;

int main()
{
    BTree<int> bt;

    cout << "--- 开始插入数据 ---" << endl;
    // 连续插入数据，利用 M_COUNT=5，这些数据会触发多次节点分裂和向上构建
    int dataToInsert[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 15, 25, 35, 45, 55};
    for (int val : dataToInsert)
    {
        cout << "插入: " << val << endl;
        bt.insert(val);
    }

    cout << "\n--- 开始测试查询 ---" << endl;
    // 测试查询存在的数据
    int searchTargets[] = {10, 50, 55, 100};
    for (int target : searchTargets)
    {
        try
        {
            int &result = bt.search(target);
            cout << "查找 " << target << " 成功，找到的值为: " << result << endl;
        }
        catch (...)
        {
            cout << "查找 " << target << " 失败！(异常)" << endl;
        }
    }

    // 测试查询不存在的数据 (假设你的代码里未找到会抛出异常或处理)
    cout << "\n--- 测试查询不存在的数据 ---" << endl;
    int missingTarget = 999;
    try
    {
        // 注意：取决于你 search 函数的具体实现方式，
        // 如果未找到时直接解引用空指针可能会导致崩溃。
        int &result = bt.search(missingTarget);
        cout << "查找 " << missingTarget << "，返回结果: " << result << endl;
    }
    catch (...)
    {
        cout << "数据 " << missingTarget << " 不存在。" << endl;
    }

    cout << "\n测试完成！" << endl;

    return 0;
}