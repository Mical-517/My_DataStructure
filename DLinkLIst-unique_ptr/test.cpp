#include <iostream>
#include "DLinList-unique_ptr.h"
#include <vector>
using namespace std;

// 这是一个泛型函数，可以计算序列中元素的总和
// 它需要知道元素的类型（value_type）来初始化 sum 变量
template <class InputIterator>
typename std::iterator_traits<InputIterator>::value_type
sum_sequence(InputIterator first, InputIterator last)
{
    // 使用 traits 获取 value_type 来声明 sum 变量
    typename std::iterator_traits<InputIterator>::value_type sum = 0;
    for (; first != last; ++first)
    {
        sum = sum + *first;
    }
    return sum;
}

int main()
{
    // 1. 测试你的自定义迭代器
    DLinkList<int> myList;
    myList.push_back(10);
    myList.push_back(20);
    myList.push_back(30);

    cout << "Sum of myList: " << sum_sequence(myList.begin(), myList.end()) << endl; // 输出 60

    // 2. 测试原生指针
    int arr[] = {1, 2, 3, 4, 5};
    cout << "Sum of array: " << sum_sequence(arr, arr + 5) << endl; // 输出 15

    // 3. 测试标准库迭代器 (例如 vector)
    std::vector<int> myVec = {100, 200};
    cout << "Sum of vector: " << sum_sequence(myVec.begin(), myVec.end()) << endl; // 输出 300

    return 0;
}
