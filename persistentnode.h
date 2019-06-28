#ifndef PERSISTENTNODE_H
#define PERSISTENTNODE_H

#include <utility>

using namespace std;

class PersistentNode
{
public:
    const int key;

    PersistentNode(const int &key, unsigned version);

    PersistentNode(const int & key,
                   PersistentNode *right,
                   PersistentNode *left);

    bool update(PersistentNode *node, unsigned version);
    PersistentNode * getLeft(unsigned version);
    PersistentNode * getRight(unsigned version);

private:
    enum PointerStatus {None, Right, Left};

    PersistentNode* left;
    PersistentNode* right;
    PersistentNode* extraPointer;
    PointerStatus extraPointerStatus;
    unsigned lastUpdateVersion;
};

#endif // PERSISTENTNODE_H
