#include "persistentnode.h"

int PersistentNode::getKey() const
{
    return key;
}

PersistentNode::PersistentNode(const int &key,
                               PersistentNode *right,
                               PersistentNode *left):
    key(key), right(right), left(left),
    extraPointer(nullptr), extraPointerStatus(None)
{
}

PersistentNode *PersistentNode::getRight(unsigned version)
{
    if ((extraPointerStatus == Right) && (version >= extraPointerVersion))
        return extraPointer;
    return right;
}

PersistentNode *PersistentNode::getLeft(unsigned version)
{
    if ((extraPointerStatus == Left) && (version >= extraPointerVersion))
        return extraPointer;
    return left;
}


