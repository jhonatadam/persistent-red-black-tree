#ifndef PERSISTENTREDBLACKTREE_H
#define PERSISTENTREDBLACKTREE_H

#include <persistentnode.h>

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stack>

class PersistentRedBlackTree
{
public:
    PersistentRedBlackTree();

    unsigned insert(const int &key);
    unsigned remove(const int &key);
    int successor(const int &key, const unsigned &version);
    int predecessor(const int &key, const unsigned &version);
    string toString(unsigned version);

private:
    vector<PersistentNode*> accessPointers;
    void treeToString(PersistentNode *node, string &str, unsigned version);
};

#endif // PERSISTENTREDBLACKTREE_H
