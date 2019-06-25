#include "persistentnode.h"

PersistentNode::PersistentNode(const int &key,
                               PersistentNode *right,
                               PersistentNode *left):
    key(key), right(right), left(left), extraPointer(nullptr)
{
}

