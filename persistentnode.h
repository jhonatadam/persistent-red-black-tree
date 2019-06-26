#ifndef PERSISTENTNODE_H
#define PERSISTENTNODE_H

#include <utility>

using namespace std;

class PersistentNode
{
    enum PointerStatus {None, Right, Left};

    const int key;
    PersistentNode* right;
    PersistentNode* left;
    PersistentNode* extraPointer;
    unsigned extraPointerVersion;
    PointerStatus extraPointerStatus;

public:
    PersistentNode(const int & key,
                   PersistentNode *right,
                   PersistentNode *left);

    PersistentNode * getRight();
    PersistentNode * getLeft();


};

#endif // PERSISTENTNODE_H
