#include "persistentnode.h"

PersistentNode::PersistentNode(const int &key, unsigned version) :
    key(key)
{
    right = nullptr;
    left = nullptr;
    extraPointer = nullptr;
    extraPointerStatus = None;
    lastUpdateVersion = version;
}

PersistentNode::PersistentNode(const int &key,
                               PersistentNode *right,
                               PersistentNode *left):
    key(key), left(left), right(right)
{
    extraPointer = nullptr;
    extraPointerStatus = None;
    lastUpdateVersion = 0;
}

bool PersistentNode::update(PersistentNode *node, unsigned version)
{
    PointerStatus childToBeUpdated  = (node->key < key ? Left : Right);

    if (extraPointerStatus != None)
    {
       if (lastUpdateVersion < version || childToBeUpdated != extraPointerStatus)
            return false;

        extraPointer = node;
    }
    else
    {
        if (lastUpdateVersion == version)
        {
            if (childToBeUpdated == Left)
                left = node;
            else
                right = node;
        }
        else
        {
            extraPointerStatus = childToBeUpdated;
            lastUpdateVersion  =          version;
            extraPointer       =             node;
        }
    }

    return true;
}

PersistentNode *PersistentNode::getLeft(unsigned version)
{
    if ((extraPointerStatus == Left) && (version >= lastUpdateVersion))
        return extraPointer;
    return left;
}

PersistentNode *PersistentNode::getRight(unsigned version)
{
    if ((extraPointerStatus == Right) && (version >= lastUpdateVersion))
        return extraPointer;
    return right;
}


