#include <iostream>
using namespace std;

// 排序函数，将keys排序(插入排序)
template <class KeyType>
void mySort(KeyType *ptr, int size)
{
    for (int i = 1; i < size; i++)
    {
        KeyType temp = ptr[i];
        int j = i - 1;
        for (j = i - 1; j >= 0; j--)
        {
            if (temp < ptr[j])
            {
                ptr[j + 1] = ptr[j];
            }
            else
                break;
        }
        ptr[j + 1] = temp;
    }
}

int main()
{
    int a[]{3, 2, 4, 1, 7, 5};
    mySort(a, 6);
    for (int i = 0; i < 6; i++)
    {
        cout << a[i] << " ";
    }
    return 0;
}