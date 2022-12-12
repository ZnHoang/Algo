#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include "sort.hpp"
#include "maze.h"
#include "binarytree.hpp"

#define START(func) printf("\033[5;35m%s START!\n\033[0m", func);
#define DONE(func) printf("\033[5;35m%s DONE!\n\033[0m", func);

void _sort(const int size)
{
    START(__FUNCTION__);

    auto cmp = [](const uint& a, const uint& b) -> bool {return a <= b;};
    auto hash = [](const uint& a, const uint& b) -> bool {return a < b;};
    auto getMaxBit = [](const uint& a) -> uint {uint maxBit = 1; while(a / (int)pow(10, maxBit)){maxBit++;} return maxBit;};
    sort<uint>* my_sort = new sort<uint>(cmp, hash, getMaxBit);

    my_sort->allSort();
    DONE(__FUNCTION__);

    delete my_sort;
    my_sort = nullptr;
}

void _maze(const int size)
{
    START(__FUNCTION__);
    maze* my_maze = new maze(size);
    my_maze->find();
    DONE(__FUNCTION__);

    delete my_maze;
    my_maze = nullptr;
}

void _binarytree(const int size)
{
    START(__FUNCTION__);
    auto cmp = [](const iNode<uint>& a, const iNode<uint>& b) -> bool {return a->getData() <= b->getData();};
    binarytree<uint>* my_binarytree = new binarytree<uint>(size, cmp);
    my_binarytree->allAlgo();
    DONE(__FUNCTION__);

    delete my_binarytree;
    my_binarytree = nullptr;
}

int main(int argc, char** argvs)
{
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argvs[i], "-sort") == 0)
        {
            int size = (i + 1 < argc) ? atoi(argvs[i + 1]) : 0;
            _sort(size);
        }
        else if(strcmp(argvs[i], "-maze") == 0)
        {
            int size = (i + 1 < argc) ? atoi(argvs[i + 1]) : 0;
            _maze(size);
        }
        else if(strcmp(argvs[i], "-binarytree") == 0 || strcmp(argvs[i], "-bt") == 0)
        {
            int size = (i + 1 < argc) ? atoi(argvs[i + 1]) : 0;
            _binarytree(size);
        }
    }

    return 0;
}