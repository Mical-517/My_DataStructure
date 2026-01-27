#pragma once
#include <iostream>
using namespace std;

template<class DataType>
class DLinkList;

template<class DataType>
ostream& operator<<(ostream& os,const DLinkList<DataType>& temp);

template<class DataType>
class DataNode
{
    friend class DLinkList<DataType>;
    public:
    DataNode()=default;
    //注意这里节点的是浅拷贝，要搭配new的内存进行使用
    DataNode(const DataType inforTemp,DataNode<DataType>* preTemp=nullptr,DataNode<DataType>* nextTemp=nullptr)
    {
        this->pre=preTemp;
        this->next=nextTemp;
        this->infor=inforTemp;
    }
    private:
    DataNode<DataType>* pre=nullptr;
    DataNode<DataType>* next=nullptr;
    DataType infor;
};

template<class DataType>
class DLinkList
{
    public:
    DLinkList()
    {
        sum=0;
        headPtr=new DataNode<DataType>();
        tailPtr=new DataNode<DataType>();
        headPtr->next=tailPtr;
        tailPtr->pre=headPtr;
    }
    DLinkList(const DLinkList<DataType>& temp);
    ~DLinkList()
    {
        clear();
        delete this->headPtr;
        delete this->tailPtr;
        headPtr=tailPtr=nullptr;
    }

    void addTail(const DataType& data);
    ostream& printDLinkList(ostream& os) const;
    void clear();
    void erase(const DataType& data);
    bool empty() const;
    private:
    DataNode<DataType>* headPtr=nullptr;
    DataNode<DataType>* tailPtr=nullptr;
    int sum=0;
    friend ostream& operator<< <DataType>(ostream& os,const DLinkList<DataType>& temp);    

};


template<class DataType>
void DLinkList<DataType>::addTail(const DataType& data)
{
    DataNode<DataType> *actionPtr=new DataNode<DataType>(data);
    if(actionPtr)
    {
        actionPtr->next=this->tailPtr;
        actionPtr->pre=this->tailPtr->pre;
        this->tailPtr->pre->next=actionPtr;
        this->tailPtr->pre=actionPtr;
        this->sum++;
    }
    else
    {
        cout<<"AddTail error"<<endl;
    }
}

template<class DataType>
ostream& DLinkList<DataType>::printDLinkList(ostream& os) const
{
    DataNode<DataType>* actionPtr=this->headPtr->next;
    while(actionPtr!=this->tailPtr)
    {
        os<<actionPtr->infor<<" ";
        actionPtr=actionPtr->next;
    }
    return os;
}

template<class DataType>
ostream& operator<<(ostream& os,const DLinkList<DataType>& temp) 
{
    return temp.printDLinkList(os);
}

template<class DataType>
DLinkList<DataType>::DLinkList(const DLinkList<DataType>& temp):DLinkList()
{
    if(temp.sum==0) return;
    DataNode<DataType>* tempPtr=temp.headPtr->next;
    while(tempPtr!=temp.tailPtr)
    {
        this->addTail(tempPtr->infor);
        tempPtr=tempPtr->next;
    }
}

template<class DataType>
void DLinkList<DataType>::clear()
{
    DataNode<DataType>* currPtr=this->headPtr->next;
    DataNode<DataType>* nextPtr=nullptr;
    while(currPtr!=this->tailPtr)
    {
        nextPtr=currPtr->next;
        delete currPtr;
        currPtr=nextPtr;
    }
    this->sum=0;
}

template<class DataType>
void DLinkList<DataType>::erase(const DataType& data)
{
    if(this->sum==0) return;
    DataNode<DataType>* actionPtr=this->headPtr->next;
    //循环范围是有限的
    while(actionPtr!=this->tailPtr&&actionPtr->infor!=data)
    {
        actionPtr=actionPtr->next;
    }
    if(actionPtr==this->tailPtr) return;
    actionPtr->pre->next=actionPtr->next;
    actionPtr->next->pre=actionPtr->pre;
    delete actionPtr;
    this->sum--;
}

template<class DataType>
bool DLinkList<DataType>::empty() const
{
    return this->sum==0?true:false;
}