#pragma once
#include <iostream>
using namespace std;

# define dataBlockSize 10
# define ptrBlockSize 10

template<class DataType>
class Dqueue;

template<class DataType>
ostream& operator<<(ostream& os,const Dqueue<DataType>& temp);

template<class DataType>
class Dqueue
{
    public:
    //默认构造函数
    Dqueue()
    {
        this->blocks=new DataType*[ptrBlockSize]{};
        int ptrMiddle=ptrBlockSize/2;
        int dataMiddle=dataBlockSize/2;
        this->headDataPtr=dataMiddle;
        this->tailDataPtr=dataMiddle-1;
        this->headBlockPtr=this->tailBlockPtr=ptrMiddle;
        blocks[ptrMiddle]=new DataType[dataBlockSize];
        this->sum=0;
        this->blockSize=ptrBlockSize;
    }
    //拷贝构造函数
    Dqueue(const Dqueue<DataType>& others);
    //析构函数
    ~Dqueue()
    {
        if (this->blocks) {
            for (int i = this->headBlockPtr; i <= this->tailBlockPtr; i++) {
                if (this->blocks[i]) {
                    delete[] this->blocks[i];
                }
            }
            delete[] this->blocks;
        }
    }
    //添加元素
    void addHead(const DataType& data);
    void addTail(const DataType& data);
    //删除元素
    DataType deleteHead();
    DataType deleteTail();
    //判空函数,只读函数使用const
    bool empty() const;
    //打印函数配合重载<<
    ostream& printDqueue(ostream& os) const;
    friend ostream& operator<< <DataType>(ostream& os,const Dqueue<DataType>& temp);
    //clear函数
    void clear();
    //重载[]运算符,实现随机访问
    DataType& operator[](int index);
    //元素数目
    int size() const;

    private:
    int headDataPtr;
    int tailDataPtr;
    int headBlockPtr;
    int tailBlockPtr;
    DataType** blocks=nullptr;
    int sum=0;
    int blockSize=0;

    //扩容函数,对指针块扩大一倍
    void expandVolume();
};

template<class DataType>
ostream& Dqueue<DataType>::printDqueue(ostream& os) const
{
    if(this->sum==0) return os;
    if (this->headBlockPtr == this->tailBlockPtr)
    {
        for (int i = headDataPtr; i <= tailDataPtr; i++)
        {
            os << blocks[headBlockPtr][i] << "  ";
        }
        os << endl;
        return os;
    }
    //打印第一个数据块
    for (int i=headDataPtr;i<dataBlockSize;i++)
    {
        os<<blocks[headBlockPtr][i]<<"  ";
    }
    for (int i=this->headBlockPtr+1;i<tailBlockPtr;i++)
    {
        for (int j=0;j<dataBlockSize;j++)
        {
            os<<blocks[i][j]<<" ";
        }
    }
    //打印最后一个数据块
    for (int i=0;i<=tailDataPtr;i++)
    {
        os<<blocks[tailBlockPtr][i]<<"  ";
    }
    return os;
}

template<class DataType>
ostream& operator<<(ostream& os,const Dqueue<DataType>& temp)
{
    return temp.printDqueue(os);
}


template<class DataType>
Dqueue<DataType>::Dqueue(const Dqueue<DataType>& others)
{
    this->sum=others.sum;
    this->blockSize=others.blockSize;
    this->headDataPtr=others.headDataPtr;
    this->tailDataPtr=others.tailDataPtr;
    this->headBlockPtr=others.headBlockPtr;
    this->tailBlockPtr=others.tailBlockPtr;
    this->blocks=new DataType*[this->blockSize]{};
    for (int i=others.headBlockPtr;i<=others.tailBlockPtr;i++)
    {
        this->blocks[i]=new DataType[dataBlockSize];
        DataType* currArry=this->blocks[i];
        for (int j=0;j<dataBlockSize;j++)
        {
            currArry[j]=others.blocks[i][j];
        }
    }
}

template<class DataType>
void Dqueue<DataType>::clear()
{
    if (this->blocks) {
        for (int i = this->headBlockPtr; i <= this->tailBlockPtr; i++)
        {
            if (this->blocks[i]) {
                delete[] this->blocks[i];
                this->blocks[i] = nullptr;
            }
        }
        delete[] this->blocks;
    }
    this->blockSize = ptrBlockSize;
    this->blocks = new DataType*[this->blockSize]{};
    this->headBlockPtr = this->tailBlockPtr = this->blockSize / 2;
    this->headDataPtr = dataBlockSize / 2;
    this->tailDataPtr = dataBlockSize / 2 - 1;
    this->sum = 0;
    this->blocks[this->headBlockPtr] = new DataType[dataBlockSize];
}

template<class DataType>
void Dqueue<DataType>::expandVolume()
{
    int newBlockSize = this->blockSize * 2;
    DataType** newBlocks = new DataType*[newBlockSize]{};
    
    int oldBlockNum = this->tailBlockPtr - this->headBlockPtr + 1;
    int newHeadBlockPtr = (newBlockSize - oldBlockNum) / 2;
    
    for (int i = 0; i < oldBlockNum; i++)
    {
        newBlocks[newHeadBlockPtr + i] = this->blocks[this->headBlockPtr + i];
    }
    
    delete[] this->blocks;
    this->blocks = newBlocks;
    this->headBlockPtr = newHeadBlockPtr;
    this->tailBlockPtr = newHeadBlockPtr + oldBlockNum - 1;
    this->blockSize = newBlockSize;
}


template<class DataType>
void Dqueue<DataType>::addHead(const DataType& data)
{
    //第一种情况：队头数据块没有满
    if(this->headDataPtr!=0)
    {
        this->blocks[this->headBlockPtr][--this->headDataPtr]=data;
        this->sum++;
    }
    else
    {
        //第二种情况：队头数据块已经满了，需要对指针块扩容
        //判断指针块是否满
        if(this->headBlockPtr==0)
        {
            //指针块满了，直接扩容
            expandVolume();
        }
        
        //没有满或者扩容后，直接new一个新的数据块
        this->blocks[--this->headBlockPtr]=new DataType[dataBlockSize];
        this->headDataPtr=dataBlockSize-1;
        this->blocks[this->headBlockPtr][this->headDataPtr]=data;
        this->sum++;
    }
}

template<class DataType>
void Dqueue<DataType>::addTail(const DataType& data)
{
    //第一种情况：队尾数据块没有满
    if(this->tailDataPtr!=dataBlockSize-1)
    {
        this->blocks[this->tailBlockPtr][++this->tailDataPtr]=data;
        this->sum++;
    }
    else
    {
        //第二种情况：队尾数据块已经满了，需要对指针块扩容
        if(this->tailBlockPtr==this->blockSize-1)
        {
            //指针块满了，直接扩容
            expandVolume();
        }
        
        //没有满或者扩容后，直接new一个新的数据块
        this->blocks[++this->tailBlockPtr]=new DataType[dataBlockSize];
        this->tailDataPtr=0;
        this->blocks[this->tailBlockPtr][this->tailDataPtr]=data;
        this->sum++;
    }
}

template<class DataType>
DataType Dqueue<DataType>::deleteHead()
{
    if(this->sum==0)
    {
        throw "队列为空";
    }
    
    DataType temp = this->blocks[this->headBlockPtr][this->headDataPtr];
    
    // 情况1：队列中只有一个元素
    if (this->headBlockPtr == this->tailBlockPtr && this->headDataPtr == this->tailDataPtr)
    {
        delete[] this->blocks[this->headBlockPtr];
        this->blocks[this->headBlockPtr] = new DataType[dataBlockSize];
        this->headDataPtr = dataBlockSize / 2;
        this->tailDataPtr = dataBlockSize / 2 - 1;
    }
    // 情况2：当前数据块还有其他元素
    else if (this->headDataPtr < dataBlockSize - 1)
    {
        this->headDataPtr++;
    }
    // 情况3：当前数据块已空，跳到下一个块
    else
    {
        delete[] this->blocks[this->headBlockPtr];
        this->blocks[this->headBlockPtr] = nullptr;
        this->headBlockPtr++;
        this->headDataPtr = 0;
    }
    
    this->sum--;
    return temp;
}

template<class DataType>
DataType Dqueue<DataType>::deleteTail()
{
    if(this->sum==0)
    {
        throw "队列为空";
    }
    
    DataType temp = this->blocks[this->tailBlockPtr][this->tailDataPtr];
    
    // 情况1：队列中只有一个元素
    if (this->headBlockPtr == this->tailBlockPtr && this->headDataPtr == this->tailDataPtr)
    {
        delete[] this->blocks[this->tailBlockPtr];
        this->blocks[this->tailBlockPtr] = new DataType[dataBlockSize];
        this->headDataPtr = dataBlockSize / 2;
        this->tailDataPtr = dataBlockSize / 2 - 1;
    }
    // 情况2：当前数据块还有其他元素
    else if (this->tailDataPtr > 0)
    {
        this->tailDataPtr--;
    }
    // 情况3：当前数据块已空，跳到上一个块
    else
    {
        delete[] this->blocks[this->tailBlockPtr];
        this->blocks[this->tailBlockPtr] = nullptr;
        this->tailBlockPtr--;
        this->tailDataPtr = dataBlockSize - 1;
    }
    
    this->sum--;
    return temp;
}

template<class DataType>
bool Dqueue<DataType>::empty() const
{
    return this->sum==0;
}

template<class DataType>
int Dqueue<DataType>::size() const
{
    return this->sum;
}

template<class DataType>
DataType& Dqueue<DataType>::operator[](int index)
{
    if(index<0||index>=this->sum)
    {
        throw "索引错误";
    }
    //先找到指针块
    //第一块有多少元素
    int firstBlockNum=dataBlockSize-headDataPtr;

    //先看第一块有没有
    if(firstBlockNum>=index+1)
    {
        int trueIndex=headDataPtr+index;
        return blocks[headBlockPtr][trueIndex];
    }
    else
    {
        //再看剩下的相当于几个整块
        int blockNum=(index+1-firstBlockNum)/dataBlockSize;
        //再看剩下的元素数
        int lastElemNum=(index+1-firstBlockNum)%dataBlockSize;
        if(lastElemNum>0)
        {
            return blocks[headBlockPtr+blockNum+1][lastElemNum-1];
        }
        else
        {
            return blocks[headBlockPtr+blockNum][dataBlockSize-1];
        }
    }
}

