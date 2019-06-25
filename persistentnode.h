#ifndef PERSISTENTNODE_H
#define PERSISTENTNODE_H

#include <utility>

using namespace std;

class PersistentNode
{
    const int key;
    PersistentNode* right;
    PersistentNode* left;
    PersistentNode* extraPointer;
    unsigned extraPointerVersion;

public:
    PersistentNode(const int & key,
                   PersistentNode *right,
                   PersistentNode *left);


};

#endif // PERSISTENTNODE_H
