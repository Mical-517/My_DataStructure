#include <iostream>
#include "DLinkList_new.h"
using namespace std;

//测试AddTail
void addtail()
{
    DLinkList<int> list;
    list.addTail(1);
    list.addTail(2);
    cout<<list<<endl;
}
int main()
{
    addtail();
    return 0;
}