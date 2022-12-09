/*
** PROBLEMS:
** radixSort and bucketSort can't pass a lambda which can catch any param
*/

#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <map>
#include <cmath>
using std::vector, std::function, std::map;
using std::swap, std::min, std::max;

using timepoint = std::chrono::_V2::steady_clock::time_point;

template <class T>
class sort
{
private:
    bool check(vector<T>&);
    void _insertSort(vector<T>&, uint = 1, uint = 1);
    void _quickSort(vector<T>&, uint, uint);
    void _mergeSort(vector<T>&, const uint, const uint);
    void _adjustHeap(vector<T>&, uint, uint);
    void _bucketSort(vector<T>&, uint, function<uint(T)>, bool = true);
public:
    sort(const int, function<bool(const T&, const T&)>, function<bool(const T&, const T&)>, function<uint(const T&)>);
    ~sort();
    void createDatas(uint);
    void createDatas(vector<T>&);
    void builtSort();
    void bubbleSort();
    void insertSort();
    void selectSort();
    void shellSort();
    void quickSort();
    void mergeSort();
    void heapSort();
    void countingSort();
    void bucketSort();
    void radixSort();
    void allSort();
private:
    vector<T> vDatas;
    function<bool(const T&, const T&)> cmp;
    function<bool(const T&, const T&)> hash;
    function<uint(const T&)> getMaxBit;
private:
    const uint DEFAULTSIZE = 50000;
    const uint MAXDATA = 1000000;
    std::chrono::nanoseconds BUILT_TIME;
    inline timepoint NOW() {return std::chrono::steady_clock::now();}
    inline std::chrono::nanoseconds USED(timepoint end, timepoint begin) {return std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);}
    inline void PRINT(const char* func, const std::chrono::nanoseconds time, bool res) {printf("\033[33m%-20s%-20.8f%-+20.8f%-20s\n\033[0m", (func), time.count() * 1e-9, (BUILT_TIME - time).count() * 1e-9, res?"TRUE":"FALSE");}
};

template <class T>
sort<T>::sort(const int size, function<bool(const T&, const T&)> cmp, function<bool(const T&, const T&)> hash, function<uint(const T&)> getMaxBit)
    : cmp(cmp), hash(hash), getMaxBit(getMaxBit)
{
    createDatas(size > 5 ? size : DEFAULTSIZE);
}

template <class T>
sort<T>::~sort()
{
}

template <class T>
void sort<T>::createDatas(uint size)
{
    vector<T> temp{};
    vDatas.swap(temp);

    auto begin = NOW();
    srand((int)time(0));
    for(uint i = 0; i < size; i++)
    {
        vDatas.emplace_back(rand() % MAXDATA);
    }
    printf("\033[33m%s\t%.8fs\t%d\n\033[0m", __FUNCTION__, USED(NOW(), begin).count() * 1e-9, size);
}

template <class T>
void sort<T>::createDatas(vector<T>& datas)
{
    this->vDatas = datas;
}

template <class T>
bool sort<T>::check(vector<T>& vDatas)
{
    for(uint i = 0; i < vDatas.size() - 1; i++)
    {
        if(!cmp(vDatas[i], vDatas[i + 1]))
        {
            return false;
        }
    }
    return true;
}

template <class T>
void sort<T>::builtSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();
    std::sort(vDatas.begin(), vDatas.end(), cmp);
    BUILT_TIME = USED(NOW(), begin);
    printf("\033[33m%-20s%-20.8f\n\033[0m", __FUNCTION__, BUILT_TIME.count() * 1e-9);
}

template <class T>
void sort<T>::bubbleSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();

    uint flag = vDatas.size();
    while(flag > 0)
    {
        uint k = flag;
        flag = 0;
        for(uint j = 1; j < k; j++)
        {
            if(!cmp(vDatas[j - 1], vDatas[j]))
            {
                swap(vDatas[j - 1], vDatas[j]);
                flag = j;
            }
        }
    }

    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::_insertSort(vector<T>& vDatas, uint start, uint step)
{
    for(uint i = start; i < vDatas.size(); i++)
    {
        auto curData = vDatas[i];
        int j;
        for(j = i - step; j >= 0 && !cmp(vDatas[j], curData); j -= step)
        {
            vDatas[j + step] = vDatas[j];
        }
        vDatas[j + step] = curData;
    }
}

template <class T>
void sort<T>::insertSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();
    _insertSort(vDatas);
    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::selectSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();

    for(uint i = 0; i < vDatas.size() - 1; i++)
    {
        auto minIdx = i;
        for(uint j = i + 1; j < vDatas.size(); j++)
        {
            if(cmp(vDatas[j], vDatas[minIdx]))
            {
                minIdx = j;
            }
        }
        swap(vDatas[i], vDatas[minIdx]);
    }

    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::shellSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();

    for(uint step = vDatas.size() / 2; step > 0; step /= 2)
    {
        _insertSort(vDatas, step, step);
    }

    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::_quickSort(vector<T>& vDatas, uint left, uint right)
{
    if(left >= right)
    {
        return;
    }

    uint curLeft = left, curRight = right;
    T baseData = vDatas[curLeft];
    
    while(curLeft < curRight)
    {
        while(curLeft < curRight && !cmp(vDatas[curRight], baseData))
        {
            curRight--;
        }
        vDatas[curLeft] = vDatas[curRight];
        while(curLeft < curRight && cmp(vDatas[curLeft], baseData))
        {
            curLeft++;
        }
        vDatas[curRight] = vDatas[curLeft];
    }
    vDatas[curLeft] = baseData;

    if(curLeft != 0)
    {
        _quickSort(vDatas, left, curLeft - 1);
    }
    _quickSort(vDatas, curRight + 1, right);
}

template <class T>
void sort<T>::quickSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();
    _quickSort(vDatas, 0, vDatas.size() - 1);
    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::_mergeSort(vector<T>& vDatas, const uint begin, const uint end)
{
    if(begin >= end)
    {
        return;
    }

    uint cnt = end - begin + 1;
    uint mid = (cnt - 1) / 2 + begin;
    uint begin_1 = begin, end_1 = mid, begin_2 = mid + 1, end_2 = end;

    _mergeSort(vDatas, begin_1, end_1);
    _mergeSort(vDatas, begin_2, end_2);

    vector<T> vTemp(cnt);
    uint curIdx = 0;
    while(begin_1 <= end_1 && begin_2 <= end_2)
    {
        vTemp[curIdx++] = !cmp(vDatas[begin_1], vDatas[begin_2]) ? vDatas[begin_2++] : vDatas[begin_1++];
    }
    while(begin_1 <= end_1)
    {
        vTemp[curIdx++] = vDatas[begin_1++];
    }
    while(begin_2 <= end_2)
    {
        vTemp[curIdx++] = vDatas[begin_2++];
    }
    curIdx = begin;
    for(auto& data : vTemp)
    {
        vDatas[curIdx++] = data;
    }
}

template <class T>
void sort<T>::mergeSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();
    _mergeSort(vDatas, 0, vDatas.size() - 1);
    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::_adjustHeap(vector<T>& vDatas, uint idx, uint len)
{
    auto topData = vDatas[idx];

    for(uint childIdx = idx * 2 + 1; childIdx < len; childIdx = childIdx * 2 + 1)
    {
        if(childIdx + 1 < len && !cmp(vDatas[childIdx + 1], vDatas[childIdx]))
        {
            childIdx++;
        }
        if(!cmp(vDatas[childIdx], topData))
        {
            vDatas[idx] = vDatas[childIdx];
            idx = childIdx;
        }
        else
        {
            break;
        }
    }
    vDatas[idx] = topData;
}

template <class T>
void sort<T>::heapSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();

    for(int idx = vDatas.size() / 2 - 1; idx >= 0; idx--)
    {
        _adjustHeap(vDatas, idx, vDatas.size());
    }

    for(int idx = vDatas.size() - 1; idx > 0; idx--)
    {
        swap(vDatas[0], vDatas[idx]);
        _adjustHeap(vDatas, 0, idx);
    }

    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::countingSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();

    map<T, uint, decltype(hash)> hashData2Cnt(hash);
    for(auto& data : vDatas)
    {
        hashData2Cnt[data]++;
    }

    uint curIdx = 0;
    for(auto& p : hashData2Cnt)
    {
        const T data = p.first;
        uint cnt = p.second;
        while(cnt--)
        {
            vDatas[curIdx++] = data;
        }
    }

    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::_bucketSort(vector<T>& vDatas, uint bucket, function<uint(T)> getBuk, bool sortBuk)
{
    /*if param sortBuk is true, everytime insert a new data after the bucket will call the _insertSort to sort them.*/
    vector<vector<T>> vBuckets(bucket);

    for(auto& data : vDatas)
    {
        uint buk = getBuk(data);
        vBuckets[buk].emplace_back(data);
        if(sortBuk)
        {
            _insertSort(vBuckets[buk], vBuckets[buk].size() - 1, 1);
        }
    }

    uint curIdx = 0;
    for(auto& buk : vBuckets)
    {
        for(auto& data : buk)
        {
            vDatas[curIdx++] = data;
        }
    }
}

template <class T>
void sort<T>::bucketSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();

    uint cnt = vDatas.size();
    T minData = vDatas[0], maxData = vDatas[0];
    for(auto& data : vDatas)
    {
        if(cmp(data, minData))
        {
            minData = data;
        }
        if(!cmp(data, maxData))
        {
            maxData = data;
        }
    }
    uint step = (maxData - minData) / cnt + 1;
    uint bucket = (maxData - minData) / step + 1;

    _bucketSort(vDatas, bucket, [step, minData](T curData){return (curData - minData) / step;});

    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::radixSort()
{
    auto vDatas = this->vDatas;
    auto begin = NOW();

    uint maxBit = 1;
    for(auto& data : vDatas)
    {
        maxBit = max(maxBit, getMaxBit(data));
    }

    for(uint bit = 1; bit <= maxBit; bit++)
    {
        _bucketSort(vDatas, 10, [bit](T curData){return curData % (int)pow(10, bit) / (int)pow(10, bit - 1);}, false);
    }

    auto used = USED(NOW(), begin);
    PRINT(__FUNCTION__, used, check(vDatas));
}

template <class T>
void sort<T>::allSort()
{
    printf("\033[4;31m%-20s%-20s%-20s%-20s\n\033[0m", "FUNCTION", "USED_TIME(s)", "COMPARISON(S)", "RESULT");
    builtSort();
#if 1
    bubbleSort();
    insertSort();
    selectSort();
#endif
    shellSort();
    quickSort();
    mergeSort();
    heapSort();
    countingSort();
    bucketSort();
    radixSort();
}

