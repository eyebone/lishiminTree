//
// Created by cxt2004 on 2023/5/16.
//

#include "MemTable.h"
#include <random>
#include <map>
using namespace std;


MemTable::MemTable() {
    this->status = MEMTABLE;
    this->maxLevel = 32;
    this->prob = 0.5;
    this->head = new Node();
}

MemTable::MemTable(int maxL, float p) {
//    this->status = MEMTABLE;
    this->maxLevel = maxL;
    this->prob = p;
    this->head = new Node(INT_MIN, "",maxL);
}

MemTable::~MemTable() {
    Node* cur = head;
    while(cur != nullptr){
        Node* next = cur->forward[0];
        delete cur;
        cur = next;
    }
}

bool MemTable::insert(const uint64_t k, const string &v) {
    Node *test = search(k);
    if(test != nullptr) {//重复插入则覆盖==
        test->value = v;
        return false;
    }

    Node* cur = head;
    Node** update = new Node*[maxLevel + 1];

    for(int i = maxLevel ; i >= 0; i--){
        while(cur->forward[i] && cur->forward[i]->key < k){
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    int level = randomLevel();
    Node* newNode = new Node(k,v,level);
    for(int i = 0; i <= level; i++){
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
    delete[] update;
    return true;
}

int MemTable::randomLevel() {
    int level = 0;
    std::mt19937 rng;
    rng.seed(std::random_device()());//set seeds
    std::uniform_real_distribution<double> distribution(0, 1);//创建一个均匀分布，随机（等概率）生成在（0， 1）之间的小数；
    while ((float)distribution(rng) / RAND_MAX < prob && level < maxLevel) { // 如果随机数小于p且未达到最大层级，则增加一层
        level++;
    }
    return level;
}

Node* MemTable::search(const uint64_t k) {
    Node* cur = head;
    for(int i = maxLevel; i >= 0; i--){
        while (cur->forward[i] != nullptr && cur->forward[i]->key < k){
            cur = cur->forward[i];
        }
        if(cur->forward[i] != nullptr && cur->forward[i]->key == k){
            return cur->forward[i];
        }
    }
    return nullptr;
}


bool MemTable::find(const uint64_t k) {
    Node* cur = head;
    for(int i = maxLevel; i>= 0; i--){
        while (cur->forward[i] && cur->forward[i]->key < k){
            cur = cur->forward[i];
        }
        if(cur->forward[i] && cur->forward[i]->key == k){
            return true;
        }
    }
    return false;
}

bool MemTable::remove(const uint64_t k) {
    Node *cur = head;
    Node *toDelete = nullptr;

    for (int i = maxLevel; i >= 0; i--) {
        while (cur->forward[i] && cur->forward[i]->key < k) {
            cur = cur->forward[i];
        }
        if (cur->forward[i] && cur->forward[i]->key == k) {
            toDelete = cur->forward[i];
            cur->forward[i] = toDelete->forward[i];

            while (i == 0 && cur->forward[i] && cur->forward[i]->key == k) {
                toDelete = cur->forward[i];
                cur->forward[i] = toDelete->forward[i];
                delete toDelete;
            }
        }
    }
    if (toDelete) {
        delete toDelete;
        return true;
    }
    return false;
}

void MemTable::clear() {
    Node* p = head->forward[0];
    while(p != nullptr){
        Node* q = p;
        p = p->forward[0];
        delete q;
    }
    for(int i = 0; i <= maxLevel; i++){
        head->forward[i] = nullptr;
    }
}