#pragma once
#include <iostream>

template<class DataType>
class KDTree;


//二维K-d树节点类
template<class DataType>
class KDNode
{
    public:
    friend class KDTree<DataType>;
    KDNode():left(nullptr),right(nullptr){}
    KDNode(DataType value1,DataType value2,KDNode<DataType>* l=nullptr,KDNode<DataType>* r=nullptr)
    {
        data[0]=value1;
        data[1]=value2;
        left=l;
        right=r;
    }
    //重载==运算符
    bool operator==(const KDNode<DataType>& other) const
    {
        return (data[0] == other.data[0] && data[1] == other.data[1]);
    }
    private:
    DataType data[2];
    KDNode<DataType>* left;
    KDNode<DataType>* right;
};

template<class DataType>
class KDTree
{
    public:
    KDTree():root(nullptr),sum(0){}
    ~KDTree()
    {
        clear();
    }

    //添加节点
    void insert(DataType& value1,DataType& value2);
    //删除节点
    void erase(DataType& value1,DataType& value2);
    //查找节点
    KDNode<DataType>* search(DataType& value1,DataType& value2);
    //清空树
    void clear();
    //最近邻搜索
    KDNode<DataType>* nearestNeighborSearch(const DataType& value1, const DataType& value2);

    private:
    KDNode<DataType>* root;
    int sum;
    //删除节点辅助函数
    KDNode<DataType>* eraseHelper(KDNode<DataType>* rootPtr, DataType& value1, DataType& value2, int currDi);
    //找到目标维度下的最小值节点
    KDNode<DataType>* findMin(KDNode<DataType>* rootPtr, int targetDi, int currDi);
    //清空树
    void clearHelper(KDNode<DataType>*& rootPtr);
    //最近邻搜索辅助函数
    KDNode<DataType>* nearestNeighborSearchHelper(const KDNode<DataType>* rootPtr,const KDNode<DataType>& targetNode,const KDNode<DataType>* rootPtrParent,int currDi);
};

template<class DataType>
void KDTree<DataType>::insert(DataType& value1,DataType& value2)
{
    KDNode<DataType>* newNode=new KDNode<DataType>(value1,value2);
    KDNode<DataType>* pre=nullptr;
    KDNode<DataType>* curr=this->root;
    int i=0;//比较的维度
    while(curr!=nullptr)
    {
        pre=curr;
        if(curr->data[i]>=newNode->data[i]) curr=curr->left;
        else curr=curr->right;
        i=(i+1)%2;
    }
    if(pre==nullptr) this->root=newNode;
    else
    {
        i=(i+1)%2;
        if(pre->data[i]>=newNode->data[i]) pre->left=newNode;
        else pre->right=newNode;
    }
    this->sum++;
}

//分情况讨论,这里我们将找到目标节点的直接后继，意味着如果没有右子树，我们便将左子树变为右子树
template<class DataType>
void KDTree<DataType>::erase(DataType& value1,DataType& value2)
{
    // 检查节点是否存在
    if (search(value1, value2) != nullptr) {
        root = eraseHelper(root, value1, value2, 0);
        sum--;
    }
}

template<class DataType>
KDNode<DataType>* KDTree<DataType>::eraseHelper(KDNode<DataType>* rootPtr, DataType& value1, DataType& value2, int currDi)
{
    if (rootPtr == nullptr) {
        return nullptr;
    }

    // 检查是否找到了要删除的节点
    if (rootPtr->data[0] == value1 && rootPtr->data[1] == value2) {
        // 情况1: 节点有右子树
        if (rootPtr->right != nullptr) {
            KDNode<DataType>* minNode = findMin(rootPtr->right, currDi, (currDi + 1) % 2);
            rootPtr->data[0] = minNode->data[0];
            rootPtr->data[1] = minNode->data[1];
            rootPtr->right = eraseHelper(rootPtr->right, minNode->data[0], minNode->data[1], (currDi + 1) % 2);
        }
        // 情况2: 节点只有左子树
        else if (rootPtr->left != nullptr) {
            KDNode<DataType>* minNode = findMin(rootPtr->left, currDi, (currDi + 1) % 2);
            rootPtr->data[0] = minNode->data[0];
            rootPtr->data[1] = minNode->data[1];
            // 递归删除左子树中的minNode，然后将更新后的左子树挂到右边
            rootPtr->right = eraseHelper(rootPtr->left, minNode->data[0], minNode->data[1], (currDi + 1) % 2);
            rootPtr->left = nullptr;
        }
        // 情况3: 叶子节点
        else {
            delete rootPtr;
            return nullptr;
        }
        return rootPtr;
    }

    // 递归向下查找
    int nextDi = (currDi + 1) % 2;
    DataType targetData[] = {value1, value2};
    if (targetData[currDi] < rootPtr->data[currDi]) {
        rootPtr->left = eraseHelper(rootPtr->left, value1, value2, nextDi);
    } else {
        rootPtr->right = eraseHelper(rootPtr->right, value1, value2, nextDi);
    }
    return rootPtr;
}


template<class DataType>
KDNode<DataType>* KDTree<DataType>::findMin(KDNode<DataType>* rootPtr, int targetDi, int currDi)
{
    if (rootPtr == nullptr) {
        return nullptr;
    }

    KDNode<DataType>* minNode = rootPtr;
    int nextDi = (currDi + 1) % 2;

    if (currDi == targetDi) {
        // 如果当前维度是目标维度，最小值只可能在左子树
        if (rootPtr->left != nullptr) {
            KDNode<DataType>* leftMin = findMin(rootPtr->left, targetDi, nextDi);
            if (leftMin->data[targetDi] < minNode->data[targetDi]) {
                minNode = leftMin;
            }
        }
    } else {
        // 否则，需要同时检查左右子树
        if (rootPtr->left != nullptr) {
            KDNode<DataType>* leftMin = findMin(rootPtr->left, targetDi, nextDi);
            if (leftMin->data[targetDi] < minNode->data[targetDi]) {
                minNode = leftMin;
            }
        }
        if (rootPtr->right != nullptr) {
            KDNode<DataType>* rightMin = findMin(rootPtr->right, targetDi, nextDi);
            if (rightMin->data[targetDi] < minNode->data[targetDi]) {
                minNode = rightMin;
            }
        }
    }
    return minNode;
}


template<class DataType>
KDNode<DataType>* KDTree<DataType>::search(DataType& value1,DataType& value2)
{
    KDNode<DataType>* curr=this->root;
    int i=0;
    KDNode<DataType> newNode(value1,value2);
    while(curr!=nullptr)
    {
        if(*curr==newNode) return curr;
        if(curr->data[i]>=newNode.data[i]) curr=curr->left;
        else curr=curr->right;
        i=(i+1)%2;
    }
    return nullptr;
}

template<class DataType>
void KDTree<DataType>::clear()
{
    clearHelper(this->root);
    this->root=nullptr;
    this->sum=0;
}

template<class DataType>
void KDTree<DataType>::clearHelper(KDNode<DataType>*& rootPtr)
{
    if(rootPtr==nullptr) return;
    clearHelper(rootPtr->left);
    clearHelper(rootPtr->right);
    delete rootPtr;
    rootPtr=nullptr;
}

template<class DataType>
KDNode<DataType>* KDTree<DataType>::nearestNeighborSearch(const DataType& value1, const DataType& value2)
{
    KDNode<DataType> targetNode(value1,value2);
    if(this->sum==0) return nullptr;
    return this->nearestNeighborSearchHelper(this->root,targetNode,nullptr,0);
}

template<class DataType>
KDNode<DataType>* KDTree<DataType>::nearestNeighborSearchHelper(const KDNode<DataType>* rootPtr,const KDNode<DataType>& targetNode,const KDNode<DataType>* rootPtrParent,int currDi)
{
    if(rootPtr==nullptr)
    {
        return rootPtrParent;
    }
    //判断递归方向
    int nextDi=(currDi+1)%2;
    KDNode<DataType>* bestNode=nullptr;
    if(targetNode.data[currDi]<rootPtr->data[currDi])
    {
        bestNode=nearestNeighborSearchHelper(rootPtr->left,targetNode,rootPtr,nextDi);
    }
    else
    {
        bestNode=nearestNeighborSearchHelper(rootPtr->right,targetNode,rootPtr,nextDi);
    }
    //回溯到当前节点时再次判断
    //1.当前节点是否是最近节点
    double bestDist=std::sqrt(std::pow(bestNode->data[0]-targetNode.data[0],2)+std::pow(bestNode->data[1]-targetNode.data[1],2));
    double currDist=std::sqrt(std::pow(rootPtr->data[0]-targetNode.data[0],2)+std::pow(rootPtr->data[1]-targetNode.data[1],2));
    if(currDist<bestDist)
    {
        bestNode=rootPtr;
    }
    //2.当前节点另一侧是否有更近的节点
    double splitDist=std::abs(rootPtr->data[currDi]-targetNode.data[currDi]);
    if(splitDist<bestDist)
    {
        KDNode<DataType>* otherSideNode=nullptr;
        if(targetNode.data[currDi]<rootPtr->data[currDi])
        {
            otherSideNode=nearestNeighborSearchHelper(rootPtr->right,targetNode,bestNode,nextDi);
        }
        else
        {
            otherSideNode=nearestNeighborSearchHelper(rootPtr->left,targetNode,bestNode,nextDi);
        }
        double otherSideDist=std::sqrt(std::pow(otherSideNode->data[0]-targetNode.data[0],2)+std::pow(otherSideNode->data[1]-targetNode.data[1],2));
        if(otherSideDist<bestDist)
        {
            bestNode=otherSideNode;
        }
    }
    return bestNode;
}
