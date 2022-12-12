#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <queue>
#include <stack>
#include <memory>
#include <functional>
#include "node.hpp"
#include "help.hpp"

#define ITERATION
#define TIMING

using std::vector, std::queue, std::stack, std::pair, std::function, std::bind;
using std::make_pair;
template <class T> using iNode = std::shared_ptr<node<T>>;
template <class T> using vNodes = vector<iNode<T>>;


template <class T>
class binarytree
{
private:
    void _showTraverse(vNodes<T>&);
    vNodes<T> _preOrderByRecursion(iNode<T>&);
    vNodes<T> _inOrderByRecursion(iNode<T>&);
    vNodes<T> _postOrderByRecursion(iNode<T>&);
    iNode<T> _buildByPreIn(vNodes<T>&, const uint, const uint, vNodes<T>&, const uint, const uint);
    iNode<T> _buildByInPost(vNodes<T>&, const uint, const uint, vNodes<T>&, const uint, const uint);
    iNode<T> _buildMirrorByRecursion(iNode<T>&);
    void _resetDatas(iNode<T>&, vNodes<T>&, uint&);
public:
    binarytree(const int, function<bool(const iNode<T>&, const iNode<T>&)>);
    ~binarytree();
    void createTree(const uint, bool=false);
    uint getLayers(iNode<T>&);
    iNode<T> copyTree(iNode<T>&);
    bool isBalanced(iNode<T>&, uint&);
    bool isSearched(iNode<T>&, iNode<T>&, iNode<T>&);
    bool compareTree(iNode<T>, iNode<T>, bool=false);
    void showTree(iNode<T>&);
    vNodes<T> level(bool=false);
    vNodes<T> preOrder(bool=false);
    vNodes<T> inOrder(bool=false);
    vNodes<T> postOrder(bool=false);
    iNode<T> buildByPreIn(vNodes<T>&, vNodes<T>&, bool=false);
    iNode<T> buildByInPost(vNodes<T>&, vNodes<T>&, bool=false);
    iNode<T> buildMirror(bool=false);
    iNode<T> buildBiSearchTree(bool=false);
    void allAlgo();
private:
    iNode<T> head = nullptr;
    function<bool(const iNode<T>&, const iNode<T>&)> compareNode;
private:
    const uint DEFAULTSIZE = 1000;
    const uint MAXDATA = 10000;
};

template <class T>
binarytree<T>::binarytree(const int size, function<bool(const iNode<T>&, const iNode<T>&)> cmp)
    : compareNode(cmp)
{
    createTree(size <= 10 ? DEFAULTSIZE : size, true);
    // showTree(head);
}

template <class T>
binarytree<T>::~binarytree()
{
}

template <class T>
void binarytree<T>::createTree(const uint size, bool needTiming)
{
    if(needTiming) {timing(__FUNCTION__, bind(&binarytree::createTree, this, size, false));}
    else
    {
        srand((int)time(0));
        uint data = rand() % MAXDATA;
        head.reset(new node<T>(data));
        vNodes<T> vEnable;
        vEnable.emplace_back(head);

        for(uint i = 2; i <= size; i++)
        {
            data = rand() % MAXDATA;
            iNode<T> nextNode(new node<T>(data));
            uint idx = rand() % vEnable.size();
            auto pNode = vEnable[idx];
            if(pNode->getLeft())
            {
                pNode->setRight(nextNode);
            }
            else if(pNode->getRight())
            {
                pNode->setLeft(nextNode);
            }
            else
            {
                uint direction = rand() % 2;
                direction == 0 ? pNode->setLeft(nextNode) : pNode->setRight(nextNode);
            }
            if(pNode->getLeft() && pNode->getRight())
            {
                vEnable.erase(vEnable.begin() + idx);
            }
            vEnable.emplace_back(nextNode);
        }
    }

}

template <class T>
uint binarytree<T>::getLayers(iNode<T>& n)
{
#ifdef ITERATION

    if(!n)
    {
        return 0;
    }
    queue<iNode<T>> q;
    q.push(n);
    uint layer = 0;
    while(!q.empty())
    {
        layer++;
        uint cnt = q.size();
        for(uint i = 0; i < cnt; i++)
        {
            auto curNode = q.front();
            q.pop();
            if(curNode->getLeft())
            {
                q.push(curNode->getLeft());
            }
            if(curNode->getRight())
            {
                q.push(curNode->getRight());
            }
        }
    }
    return layer;

#else
    return n ? (1 + std::max(getLayers(n->getLeft()), getLayers(n->getRight()))) : 0;
#endif
}

template <class T>
iNode<T> binarytree<T>::copyTree(iNode<T>& n)
{
    if(!n)
    {
        return nullptr;
    }
    iNode<T> curNode(new node<T>(n));
    curNode->setLeft(n->getLeft());
    curNode->setRight(n->getRight());

    return curNode;
}

template <class T>
bool binarytree<T>::isBalanced(iNode<T>& n, uint& layer)
{
    if(!n)
    {
        layer = 0;
        return true;
    }
    uint leftLayer, rightLayer;
    if(isBalanced(n->getLeft(), leftLayer) && isBalanced(n->getRight(), rightLayer))
    {
        uint diffLayer = abs(leftLayer - rightLayer);
        if(diffLayer == 1)
        {
            layer = 1 + max(leftLayer, rightLayer);
            return true;
        }
    }
    return false;
}

template <class T>
bool binarytree<T>::isSearched(iNode<T>& curNode, iNode<T>& min, iNode<T>& max)
{
    if(curNode->getLeft())
    {
        if(isSearched(curNode->getLeft(), min, max))
        {

        }
    }
}

template <class T>
bool binarytree<T>::compareTree(iNode<T> newNode, iNode<T> oriNode, bool isMirror)
{
#ifdef ITERATION

    queue<iNode<T>> qNew;
    queue<iNode<T>> qOri;
    qNew.push(newNode);
    qOri.push(oriNode);

    while(!qNew.empty() && !qOri.empty())
    {
        newNode = qNew.front();
        oriNode = qOri.front();
        qNew.pop();
        qOri.pop();
#endif
        if(!newNode && !oriNode)
        {
        #ifdef ITERATION
            continue;
        #else
            return true;
        #endif
        }
        else if(!newNode || !oriNode)
        {
            return false;
        }
        else
        {
            if(newNode->getOrigin() == oriNode)
            {
            #ifdef ITERATION
                qNew.push(newNode->getLeft());
                qNew.push(newNode->getRight());
                qOri.push(isMirror ? oriNode->getRight() : oriNode->getLeft());
                qOri.push(isMirror ? oriNode->getLeft() : oriNode->getRight());
            #else
                return compareTree(newNode->getLeft(), isMirror ? oriNode->getRight() : oriNode->getLeft()) && compareTree(newNode->getRight(), isMirror ? oriNode->getLeft() : oriNode->getRight());
            #endif
            }
            else
            {
                return false;
            }
        }
#ifdef ITERATION
    }
    return qNew.size() == qOri.size();
#endif
}

template <class T>
void binarytree<T>::showTree(iNode<T>& n)
{
    uint layer = getLayers(n);
    uint rows = (uint)std::pow(2, layer) - 1;
    uint curLayer = 1;

    vector<vNodes<T>> vMat(rows, vNodes<T>(layer, nullptr));
    iNode<T> none(new node<T>(1));
    queue<std::pair<uint, iNode<T>>> q;
    q.push(make_pair(0, n));
    while(!q.empty() && curLayer <= layer)
    {
        uint cnt = q.size();
        for(uint i = 0; i < cnt; i++)
        {
            auto R_N = q.front();
            auto row = R_N.first;
            auto curNode = R_N.second;
            q.pop();

            uint curRow = row + ((i % 2 == 0) ? (pow(2, layer - curLayer)) : (-pow(2, layer - curLayer)));
            if(!curNode)
            {
                q.push(make_pair(curRow, nullptr));
                q.push(make_pair(curRow, nullptr));
                curNode = none;
            }
            else
            {
                q.push(make_pair(curRow, curNode->getLeft()));
                q.push(make_pair(curRow, curNode->getRight()));
            }
            vMat[curRow - 1][curLayer - 1] = curNode;
        }
        curLayer++;
    }
    for(auto& row : vMat)
    {
        for(auto& n : row)
        {
            if(!n)
            {
                printf("     ");
            }
            else if(n == none)
            {
                printf("none ");
            }
            else
            {
                printf("%4u ", n->getData());
            }
        }
        printf("\n");
    }
}

template <class T>
void binarytree<T>::_showTraverse(vNodes<T>& res)
{
    for(uint i = 0; i < res.size(); i++)
    {
        printf("%u%s", res[i]->getData(), i == res.size() - 1 ? "\n" : ", ");
    }
}

template <class T>
vNodes<T> binarytree<T>::level(bool needTiming)
{
    vNodes<T> res;
    if(needTiming) 
    {
        res = timingWithRet(__FUNCTION__, bind(&binarytree::level, this, false));
    }
    else
    {
        queue<iNode<T>> q;
        q.push(head);
        while(!q.empty())
        {
            auto n = q.front();
            q.pop();
            if(n)
            {
                res.emplace_back(n);
                q.push(n->getLeft());
                q.push(n->getRight());
            }
        }

    }
    return res;
}

template <class T>
vNodes<T> binarytree<T>::preOrder(bool needTiming)
{
    vNodes<T> res;
    if(needTiming)
    {
        res = timingWithRet(__FUNCTION__, bind(&binarytree::preOrder, this, false));
    }
    else
    {
    #ifdef ITERATION

        stack<iNode<T>> st;
        auto curNode = head;
        
        while(curNode || !st.empty())
        {
            if(curNode)
            {
                res.emplace_back(curNode);
                st.push(curNode);
                curNode = curNode->getLeft();
            }
            else
            {
                curNode = st.top()->getRight();
                st.pop();
            }
        }

    #else
        res = _preOrderByRecursion(head);
    #endif
    }

    return res;
}

template <class T>
vNodes<T> binarytree<T>::_preOrderByRecursion(iNode<T>& n)
{
    vNodes<T> res;
    if(n)
    {
        res.emplace_back(n);
        auto leftRes = _preOrderByRecursion(n->getLeft());
        auto rightRes = _preOrderByRecursion(n->getRight());
        res.insert(res.end(), leftRes.begin(), leftRes.end());
        res.insert(res.end(), rightRes.begin(), rightRes.end());
    }
    return res;
}

template <class T>
vNodes<T> binarytree<T>::inOrder(bool needTiming)
{
    vNodes<T> res;
    if(needTiming)
    {
        res = timingWithRet(__FUNCTION__, bind(&binarytree::inOrder, this, false));
    }
    else
    {
    #ifdef ITERATION

        stack<iNode<T>> st;
        auto curNode = head;
        
        while(curNode || !st.empty())
        {
            if(curNode)
            {
                st.push(curNode);
                curNode = curNode->getLeft();
            }
            else
            {
                res.emplace_back(st.top());
                curNode = st.top()->getRight();
                st.pop();
            }
        }

    #else
        res = _inOrderByRecursion(head);
    #endif
    }
    
    return res;
}

template <class T>
vNodes<T> binarytree<T>::_inOrderByRecursion(iNode<T>& n)
{
    vNodes<T> res;
    if(n)
    {
        auto leftRes = _inOrderByRecursion(n->getLeft());
        auto rightRes = _inOrderByRecursion(n->getRight());
        res.insert(res.end(), leftRes.begin(), leftRes.end());
        res.emplace_back(n);
        res.insert(res.end(), rightRes.begin(), rightRes.end());
    }
    return res;
}

template <class T>
vNodes<T> binarytree<T>::postOrder(bool needTiming)
{
    vNodes<T> res;
    if(needTiming)
    {
        res = timingWithRet(__FUNCTION__, bind(&binarytree::postOrder, this, false));
    }
    else
    {
    #ifdef ITERATION

        stack<iNode<T>> st;
        auto curNode = head;
        iNode<T> lastVisit = nullptr;
        
        while(curNode || !st.empty())
        {
            if(curNode)
            {
                st.push(curNode);
                curNode = curNode->getLeft();
            }
            else
            {
                curNode = st.top();
                auto rNode = curNode->getRight();
                if(rNode && rNode != lastVisit)
                {
                    st.push(rNode);
                    curNode = rNode->getLeft();
                }
                else
                {
                    st.pop();
                    res.emplace_back(curNode);
                    lastVisit = curNode;
                    curNode = nullptr;
                }
            }
        }

    #else
        res = _postOrderByRecursion(head);
    #endif
    }

    return res;
}

template <class T>
vNodes<T> binarytree<T>::_postOrderByRecursion(iNode<T>& n)
{
    vNodes<T> res;
    if(n)
    {
        auto leftRes = _postOrderByRecursion(n->getLeft());
        auto rightRes = _postOrderByRecursion(n->getRight());
        res.insert(res.end(), leftRes.begin(), leftRes.end());
        res.insert(res.end(), rightRes.begin(), rightRes.end());
        res.emplace_back(n);
    }
    return res;
}

template <class T>
iNode<T> binarytree<T>::_buildByPreIn(vNodes<T>& vPre, const uint lPre, const uint rPre, vNodes<T>& vIn, const uint lIn, const uint rIn)
{
    iNode<T> retNode(new node<T>(vPre[lPre]));
    uint division;
    for(division = lIn; division <= rIn; division++)
    {
        if(vIn[division] == retNode->getOrigin())
        {
            break;
        }
    }
    retNode->setLeft(division == lIn ? nullptr : _buildByPreIn(vPre, lPre + 1, rPre + (division - lIn), vIn, lIn, division - 1));
    retNode->setRight(division == rIn ? nullptr : _buildByPreIn(vPre, lPre + (division - lIn) + 1, rPre, vIn, division + 1, rIn));

    return retNode;
}

template <class T>
iNode<T> binarytree<T>::buildByPreIn(vNodes<T>& vPre, vNodes<T>& vIn, bool needTiming)
{
    iNode<T> newHead;
    if(needTiming)
    {
        newHead = timingWithRet(__FUNCTION__, bind(&binarytree::buildByPreIn, this, vPre, vIn, false));
    }
    else
    {
        newHead = _buildByPreIn(vPre, 0, vPre.size() - 1, vIn, 0, vIn.size() - 1);
    }

    return newHead;
}

template <class T>
iNode<T> binarytree<T>::_buildByInPost(vNodes<T>& vIn, const uint lIn, const uint rIn, vNodes<T>& vPost, const uint lPost, const uint rPost)
{
    iNode<T> retNode(new node<T>(vPost[rPost]));
    uint division;
    for(division = lIn; division <= rIn; division++)
    {
        if(vIn[division] == retNode->getOrigin())
        {
            break;
        }
    }
    retNode->setLeft(division == lIn ? nullptr : _buildByInPost(vIn, lIn, division - 1, vPost, lPost, lPost + (division - lIn) - 1));
    retNode->setRight(division == rIn ? nullptr : _buildByInPost(vIn, division + 1, rIn, vPost, lPost + (division - lIn), rPost - 1));

    return retNode;
}

template <class T>
iNode<T> binarytree<T>::buildByInPost(vNodes<T>& vIn, vNodes<T>& vPost, bool needTiming)
{
    iNode<T> newHead;
    if(needTiming)
    {
        newHead = timingWithRet(__FUNCTION__, bind(&binarytree::buildByInPost, this, vIn, vPost, false));
    }
    else
    {
        newHead = _buildByInPost(vIn, 0, vIn.size() - 1, vPost, 0, vPost.size() - 1);
    }

    return newHead;
}

template <class T>
iNode<T> binarytree<T>::buildMirror(bool needTiming)
{
    iNode<T> retNode;
    if(needTiming)
    {
        retNode = timingWithRet(__FUNCTION__, bind(&binarytree::buildMirror, this, false));
    }
    else
    {
    #ifdef ITERATION

        queue<iNode<T>> q;
        retNode.reset(new node<T>(head));
        q.push(retNode);
        while(!q.empty())
        {
            auto curNode = q.front();
            q.pop();
            if(!curNode->getOrigin())
            {
                continue;
            }
            if(curNode->getOrigin()->getRight())
            {
                iNode<T> left(new node<T>(curNode->getOrigin()->getRight()));
                curNode->setLeft(left);
                q.push(left);
            }
            if(curNode->getOrigin()->getLeft())
            {
                iNode<T> right(new node<T>(curNode->getOrigin()->getLeft()));
                curNode->setRight(right);
                q.push(right);
            }
        }

    #else
        retNode = _buildMirrorByRecursion(head);
    #endif
    }

    return retNode;
}

template <class T>
iNode<T> binarytree<T>::_buildMirrorByRecursion(iNode<T>& n)
{
    if(!n)
    {
        return nullptr;
    }
    auto curNode = new node<T>(n);
    curNode->setLeft(n->getRight());
    curNode->setRight(n->getLeft());

    return curNode;
}

template <class T>
iNode<T> binarytree<T>::buildBiSearchTree(bool needTiming)
{
    iNode<T> retNode;
    if(needTiming)
    {
        retNode = timingWithRet(__FUNCTION__, bind(&binarytree::buildBiSearchTree, this, false));
    }
    else
    {
        retNode = copyTree(head);
        auto vIn = inOrder();
        std::sort(vIn.begin(), vIn.end(), compareNode);
        uint idx = 0;
        _resetDatas(retNode, vIn, idx);
    }

    return retNode;
}

template <class T>
void binarytree<T>::_resetDatas(iNode<T>& n, vNodes<T>& vIn, uint& idx)
{
    if(!n)
    {
        return ;
    }
    _resetDatas(n->getLeft(), vIn, idx);
    n->setData(vIn[idx]->getData());
    idx++;
    _resetDatas(n->getRight(), vIn, idx);
}

template <class T>
void binarytree<T>::allAlgo()
{
    printf("\033[4;31m%-20s%-20s\n\033[0m", "FUNCTION", "USED_TIME(s)");
    auto vLevel = level(true);
    auto vPre = preOrder(true);
    auto vIn = inOrder(true);
    auto vPost = postOrder(true);
    printf("\033[4;31m%-20s%-20s%-20s\n\033[0m", "FUNCTION", "USED_TIME(s)", "RESULT");
    buildByPreIn(vPre, vIn, true);
    buildByInPost(vIn, vPost, true);
    buildMirror(true);
    buildBiSearchTree(true);
}
