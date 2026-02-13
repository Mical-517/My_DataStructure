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
    void nearestNeighborSearchHelper(const KDNode<DataType>* rootPtr, const KDNode<DataType>& targetNode, KDNode<DataType>*& bestNode, double& minDistanceSq, int currDi);
    //计算两点距离的平方
    double distanceSquared(const KDNode<DataType>* n1, const KDNode<DataType>* n2);
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
double KDTree<DataType>::distanceSquared(const KDNode<DataType>* n1, const KDNode<DataType>* n2)
{
    double dx = n1->data[0] - n2->data[0];
    double dy = n1->data[1] - n2->data[1];
    return dx * dx + dy * dy;
}

template<class DataType>
KDNode<DataType>* KDTree<DataType>::nearestNeighborSearch(const DataType& value1, const DataType& value2)
{
    if (this->root == nullptr) {
        return nullptr;
    }
    KDNode<DataType> targetNode(value1, value2);
    KDNode<DataType>* bestNode = nullptr;
    double minDistanceSq = -1.0; // 使用-1.0表示无限大

    nearestNeighborSearchHelper(this->root, targetNode, bestNode, minDistanceSq, 0);
    return bestNode;
}


template<class DataType>
void KDTree<DataType>::nearestNeighborSearchHelper(const KDNode<DataType>* rootPtr, const KDNode<DataType>& targetNode, KDNode<DataType>*& bestNode, double& minDistanceSq, int currDi)
{
    if (rootPtr == nullptr) {
        return;
    }

    // 1. 沿着路径向下搜索
    KDNode<DataType>* searchPath = nullptr;
    KDNode<DataType>* otherPath = nullptr;

    if (targetNode.data[currDi] < rootPtr->data[currDi]) {
        searchPath = rootPtr->left;
        otherPath = rootPtr->right;
    } else {
        searchPath = rootPtr->right;
        otherPath = rootPtr->left;
    }

    // 递归进入更可能包含最近邻的子树
    nearestNeighborSearchHelper(searchPath, targetNode, bestNode, minDistanceSq, (currDi + 1) % 2);

    // 2. 回溯
    // (a) 检查当前节点是否是更近的点
    double currentDistSq = distanceSquared(rootPtr, &targetNode);
    if (bestNode == nullptr || currentDistSq < minDistanceSq) {
        minDistanceSq = currentDistSq;
        bestNode = const_cast<KDNode<DataType>*>(rootPtr);
    }

    // (b) 检查是否需要搜索另一侧的子树
    // 计算目标点到分割超平面的距离的平方
    double distToSplitPlaneSq = targetNode.data[currDi] - rootPtr->data[currDi];
    distToSplitPlaneSq *= distToSplitPlaneSq;

    // 如果以目标点为圆心，minDistance为半径的圆与分割平面相交，则另一侧子树可能存在更近的点
    if (distToSplitPlaneSq < minDistanceSq) {
        nearestNeighborSearchHelper(otherPath, targetNode, bestNode, minDistanceSq, (currDi + 1) % 2);
    }
}


