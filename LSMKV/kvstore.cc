#include "kvstore.h"
#include <string>
#include <algorithm>
KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
}

KVStore::~KVStore()
{
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    if(m_table.insert(key, s))
        keySet.push_back(key);
    return;
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	Node* getNode = m_table.search(key);
    if(getNode == nullptr)
        return "";
    return getNode->value;
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	if(m_table.remove(key))
        return true;
    return false;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    m_table.clear();
}


/**
 * Return a list including all the key-value pair between key1 and key2.
 * keys in the list should be in an ascending order.
 * An empty string indicates not found.
 */
void KVStore::scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list)
{
    list.clear();
    vector <uint64_t> scanSet = keySet;
    std::sort(scanSet.begin(), scanSet.end());

    for (auto it: scanSet) {
        if (it >= key1 && it <= key2) {
            std::pair <uint64_t, std::string> p(it, get(it));
            list.push_back(p);
        }
    }
}