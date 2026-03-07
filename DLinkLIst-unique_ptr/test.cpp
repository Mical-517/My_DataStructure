#include <iostream>
#include "DLinList-unique_ptr.h"
using namespace std;

int main()
{
    DLinkList<int> link1;
    link1.push_front(1);
    link1.push_front(2);
    link1.push_front(3);
    link1.push_front(4);
    link1.print();
    cout << endl;
    auto link2 = link1;
    link2.push_back(1);
    link2.push_back(2);
    link2.push_back(3);
    link2.push_back(4);
    link2.print();
    return 0;
}