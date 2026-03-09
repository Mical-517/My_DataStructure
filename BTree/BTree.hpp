#include <iostream>
#include <memory>
#include <vector>
using namespace std;

constexpr int M_COUNT = 5;

template <class KeyType>
class BTree;

template <class KeyType>
class BTreeNode
{
public:
    BTreeNode() = default;

private:
    bool leaf{true};                                  // 记录当前节点是否是叶子节点,因为是自底向上构建，所以是true
    int keyTally{0};                                  // 记录当前节点键值数量
    KeyType keys[M_COUNT - 1];                        // 记录当前节点的键值
    unique_ptr<BTreeNode<KeyType>> pointers[M_COUNT]; // 记录当前节点指向的子节点

    friend class BTree<KeyType>;
};

template <class KeyType>
class BTree
{
public:
    BTree() = default;
    // 插入键值
    void insert(const KeyType &data);
    // 查询键值
    KeyType &search(const KeyType &data);

private:
    unique_ptr<BTreeNode<KeyType>> node;
    int size{0};

    // 查询键值辅助
    BTreeNode<KeyType> *searchHelper(const KeyType &data, BTreeNode<KeyType> *currNode, int &index);
    // 插入查询
    BTreeNode<KeyType> *insertSearch(const KeyType &data, vector<BTreeNode<KeyType> *> &path);
    // 排序函数，将keys排序
    void mySort(KeyType *ptr, int size);
};

// 查询键值
template <class KeyType>
BTreeNode<KeyType> *BTree<KeyType>::searchHelper(const KeyType &data, BTreeNode<KeyType> *currNode, int &index)
{
    // 修复 1：必须判断当前传入的节点 currNode 是否为空，而不是根节点
    if (currNode == nullptr)
    {
        return nullptr;
    }

    int i = 0; // 对应查询键值的下标
               // 在当前节点进行查询
    for (i = 0; i < currNode->keyTally && currNode->keys[i] < data; i++)
    {
        // 空循环
    }
    // 如果当前节点没有，此时第i个键值是比data大的那个键值物理序号
    if (i >= currNode->keyTally || currNode->keys[i] > data)
    {
        return searchHelper(data, currNode->pointers[i].get(), index);
    }
    else
    {
        index = i;
        return currNode;
    }
}

template <class KeyType>
KeyType &BTree<KeyType>::search(const KeyType &data)
{
    int index = 0;
    auto ptr = searchHelper(data, this->node.get(), index);

    if (ptr == nullptr)
    {
        // 修复 2：如果没找到，必须中断执行抛出异常，否则无法返回合法的引用
        throw runtime_error("查找失败：数据不存在");
    }
    else
    {
        return ptr->keys[index];
    }
}

// 插入查询,要适配向上构建，所以但是插入是非递归的，所以要记录从上到下的路径
template <class KeyType>
BTreeNode<KeyType> *BTree<KeyType>::insertSearch(const KeyType &data, vector<BTreeNode<KeyType> *> &path)
{
    BTreeNode<KeyType> *currNode = this->node.get();
    if (currNode == nullptr)
        return nullptr;
    while (true)
    {
        path.push_back(currNode);
        // 先找到第一个大于等于data的键值的下标
        int i = 0;
        while (i < currNode->keyTally && data > currNode->keys[i])
            i++;

        // 判断如果键值重复
        if (currNode->keys[i] == data)
        {
            cout << "Key" << data << "already exists." << endl;
            return nullptr;
        }
        /*
        //如果还有子节点就往下探索
        if(currNode->pointers[i]!=nullptr)
        {
            insertSearch(data,path);
        }
        */
        // 如果是叶子结点，就表明找到了
        if (currNode->leaf)
        {
            return currNode;
        }
        // 不是就继续向下探索
        else
        {
            currNode = currNode->pointers[i].get();
        }
    }
}

// 排序函数，将keys排序(插入排序)
template <class KeyType>
void BTree<KeyType>::mySort(KeyType *ptr, int size)
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

template <class KeyType>
void BTree<KeyType>::insert(const KeyType &data)
{
    if (this->node == nullptr || this->node->keyTally == 0)
    {
        // 创建堆内存插入
        if (this->node == nullptr)
        {
            this->node = make_unique<BTreeNode<KeyType>>();
        }
        this->node->keys[0] = data;
        this->node->keyTally = 1;
        return;
    }

    // 定义变量记录路径，以便后续向上构建
    vector<BTreeNode<KeyType> *> path;
    // 找到要插入的节点
    BTreeNode<KeyType> *targetNode = insertSearch(data, path);

    // 如果为空就表明，键值重复
    if (targetNode == nullptr)
        return;

    // 开始插入，定义变量表示可能产生的新节点
    unique_ptr<BTreeNode<KeyType>> newChildNode = nullptr; // 分裂产生的新子节点
    KeyType keyToInsert = data;

    while (true)
    {
        // 如果当前节点的键值数组还有空
        if (targetNode->keyTally < M_COUNT - 1)
        {
            // 找到插入位置然后重新排序
            int i = targetNode->keyTally;
            while (i > 0 && keyToInsert < targetNode->keys[i - 1])
            {
                targetNode->keys[i] = targetNode->keys[i - 1];
                // 如果他不是叶子节点的话，要把ptr[i]向后移动
                if (!targetNode->leaf)
                {
                    targetNode->pointers[i + 1] = move(targetNode->pointers[i]);
                }
                i--;
            }
            targetNode->keys[i] = keyToInsert;
            // 如果此时有新子节点,当前节点的智能指针就要管理他,并且更新是否是叶子节点
            if (newChildNode != nullptr)
            {
                targetNode->pointers[i + 1] = move(newChildNode);
                targetNode->leaf = false;
            }
            targetNode->keyTally++;
            return;
        }
        else
        {
            // 定义一个新的临时存储所有键值的keys和pointers;
            vector<KeyType> tempKeys;
            vector<unique_ptr<BTreeNode<KeyType>>> tempPointers;

            // 将当前节点所有的键值与指针转移
            for (int i = 0; i < targetNode->keyTally; i++)
            {
                tempKeys.push_back(targetNode->keys[i]);
            }
            for (int i = 0; i <= targetNode->keyTally; i++) // 注意这里是 <=，循环次数是 tally + 1
            {
                tempPointers.push_back(move(targetNode->pointers[i]));
            }

            // 找到新节点的插入位置并更新指针
            auto it_key = lower_bound(tempKeys.begin(), tempKeys.end(), keyToInsert);
            int insert_pos = distance(tempKeys.begin(), it_key); // 修正：tempKeys.begin()

            // 插入键值以及带来的新的子节点
            tempKeys.insert(tempKeys.begin() + insert_pos, keyToInsert);
            tempPointers.insert(tempPointers.begin() + insert_pos + 1, move(newChildNode));

            // 定义分裂点,确定要提升到父节点的键值
            int splitIndex = M_COUNT / 2;
            KeyType keyToParent = tempKeys.at(splitIndex);

            // 创建新的右侧兄弟节点
            auto rightNode = make_unique<BTreeNode<KeyType>>();
            rightNode->leaf = targetNode->leaf;

            // 填充旧节点（作为左节点）
            targetNode->keyTally = splitIndex;
            for (int i = 0; i < splitIndex; i++) // 修正：大小写统一
            {
                targetNode->keys[i] = tempKeys[i];
                targetNode->pointers[i] = move(tempPointers[i]);
            }
            targetNode->pointers[splitIndex] = move(tempPointers[splitIndex]);

            // 填充新的右节点 (修正核心计算逻辑：减去提升的1个键)
            rightNode->keyTally = M_COUNT - 1 - splitIndex;
            for (int i = 0; i < rightNode->keyTally; i++) // 修正：分号
            {
                rightNode->keys[i] = tempKeys[splitIndex + 1 + i];
                rightNode->pointers[i] = move(tempPointers[splitIndex + 1 + i]);
            }
            rightNode->pointers[rightNode->keyTally] = move(tempPointers[M_COUNT]);

            // 准备向上传递
            keyToInsert = keyToParent;
            newChildNode = move(rightNode);

            // 从路径中移除当前节点
            path.pop_back(); // 修正：vector用pop_back()

            // 如果当前节点是根节点 (通过路径是否为空来判断最稳妥)
            if (path.empty())
            {
                auto newRoot = make_unique<BTreeNode<KeyType>>(); // 修正：KeyType
                newRoot->keyTally = 1;
                newRoot->leaf = false;
                newRoot->keys[0] = keyToInsert; // 修正：提取上来的应当直接赋值
                newRoot->pointers[0] = move(this->node);
                newRoot->pointers[1] = move(newChildNode); // 修正：pointers拼写
                this->node = move(newRoot);
                return;
            }
            else
            {
                // 处理targetNode的父节点
                targetNode = path.back();
            }
        }
    }
}
