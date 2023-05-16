//
// Created by cxt2004 on 2023/5/16.
//

#ifndef PROJECT_LSMKV_HANDOUT_2023_MEMTABLE_H
#define PROJECT_LSMKV_HANDOUT_2023_MEMTABLE_H

#include <climits>
#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

using namespace std;

#define MAXLEVEL 32
using namespace std;
class Node {
public:
    uint64_t key;//键
    string value;//值
    int level;//层级
    Node **forward;// 指针数组，存储每一层的下一个节点

    Node() {
        key = INT_MIN;
        value = "";
        level = MAXLEVEL;
        this->forward = new Node *[level + 1];
        for (int i = 0; i <= level; i++) {
            this->forward[i] = nullptr;
        }
    }

    Node(uint64_t k, string v, int l) {
        this->key = k;
        this->value = v;
        this->level = l;
        this->forward = new Node *[level + 1];
        for (int i = 0; i <= level; i++) {
            this->forward[i] = nullptr;
        }
    }

    ~Node() {
        delete[]forward;
    }
};


class MemTable {
public:
    Node *head;
    int maxLevel;
    float prob;
//    memTable_status status;

//    vector<uint64_t> keySet;
    MemTable(int maxL, float p);

    MemTable();

    ~MemTable();

    bool insert(const uint64_t k, const string &v);

    int randomLevel();

    Node *search(const uint64_t k);

    bool find(const uint64_t k);

    bool remove(const uint64_t k);
    void clear();
};


#endif //PROJECT_LSMKV_HANDOUT_2023_MEMTABLE_H
