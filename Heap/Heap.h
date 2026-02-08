#pragma once
#include <iostream>
#define MAX_SIZE 100
using namespace std;

template<class DataType>
class MaxHeap;

//利用数组实现最大堆
template<class DataType>
class Data
{
    public:
    friend class MaxHeap<DataType>;
    Data(int p=0,DataType d=DataType()):priority(p),data(d){}
    // 访问器：允许外部读取 data 和 priority（只读）
    int getPriority() const { return priority; }
    DataType getData() const { return data; }

    // 修正：operator= 作为非静态成员函数
    Data& operator=(const Data& rhs) {
        if (this != &rhs) {
            priority = rhs.priority;
            data = rhs.data;
        }
        return *this;
    }

    private:
    int priority=0;
    DataType data;

    bool operator<(const Data& other) const
    {
        return this->priority<other.priority;
    }
};

template<class DataType>
class MaxHeap
{
    public:
    MaxHeap():size(0){}
    //插入元素
    void insert(DataType& data,int priority);
    //删除堆顶元素
    void remove();
    //获取堆顶元素
    Data<DataType> getMax() const;
    private:
    Data<DataType> heap[MAX_SIZE];
    int size=0;
    //插入元素辅助函数swap
    void swap(Data<DataType>& a,Data<DataType>& b);
};

template<class DataType>
void MaxHeap<DataType>::insert(DataType& data,int priority)
{
    if(this->size>=MAX_SIZE)
    {
        cout<<"Heap is full"<<endl;
        return;
    }
    // 插入新元素到数组的末尾
    this->heap[this->size]=Data<DataType>(priority,data);
    this->size++;

    //上浮新插入的元素
    int index=this->size-1;
    while(index!=0)
    {
        int parentIndex=(index-1)/2;
        if(this->heap[parentIndex]<this->heap[index])
        {
            swap(this->heap[parentIndex],this->heap[index]);
            index=parentIndex;
        }
        else
        {
            return;
        }

    }
}

template<class DataType>
void MaxHeap<DataType>::swap(Data<DataType>& a,Data<DataType>& b)
{
    Data<DataType> temp=a;
    a=b;
    b=temp;
}

template<class DataType>
void MaxHeap<DataType>::remove()
{
    if(this->size==0)
    {
        cout<<"Heap is empty"<<endl;
        return;
    }
    // 将堆顶元素替换为最后一个元素
    this->heap[0]=this->heap[this->size-1];
    this->size--;

    // 下沉替换后的堆顶元素
    int index=0;
    while(2*index+1<this->size)
    {
        int childIndex=index*2+1;
        if(childIndex+1<this->size&&this->heap[childIndex]<this->heap[childIndex+1])
        {
            childIndex++;
        }
        if(this->heap[index]<this->heap[childIndex])
        {
            swap(this->heap[index],this->heap[childIndex]);
            index=childIndex;
        }
        else 
        {
            return;
        }

    }
}

template<class DataType>
Data<DataType> MaxHeap<DataType>::getMax() const
{
    if(this->size==0)
    {
        throw("Heap is empty");
    }
    return this->heap[0];
}