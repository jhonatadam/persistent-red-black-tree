#include "persistentnode.h"

PersistentNode::PersistentNode(const int &key,
                               PersistentNode *right,
                               PersistentNode *left):
    key(key), right(right), left(left),
    extraPointer(nullptr), extraPointerStatus(None)
{
}

PersistentNode *PersistentNode::getRight()
{
    return extraPointerStatus == Right ? extraPointer : right;
}

PersistentNode *PersistentNode::getLeft()
{
    return extraPointerStatus == Left ? extraPointer : left;
}


