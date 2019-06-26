#ifndef PERSISTENTREDBLACKTREE_H
#define PERSISTENTREDBLACKTREE_H

#include <persistentnode.h>

#include <vector>
#include <limits>

class PersistentRedBlackTree
{
    vector<PersistentNode*> accessPointers;

public:
    PersistentRedBlackTree();

    unsigned insert(const int &key);
    unsigned remove(const int &key);
    int successor(const int &key, const unsigned &version);
    int predecessor(const int &key, const unsigned &version);
};

#endif // PERSISTENTREDBLACKTREE_H
