#include "persistentnode.h"

PersistentNode::PersistentNode(const int &key, unsigned version) :
    key(key)
{
    color = Red;
    right = nullptr;
    left = nullptr;
    extraPointer = nullptr;
    extraPointerStatus = None;
    lastUpdateVersion = version;
}

PersistentNode::PersistentNode(PersistentNode *other, unsigned version):
    key(other->key)
{
    left = other->getLeft(version);
    right = other->getRight(version);
    color = other->getColor();
    extraPointer = nullptr;
    extraPointerStatus = None;
    lastUpdateVersion = version;
}

bool PersistentNode::update(PersistentNode *node, unsigned version, PointerStatus childToBeUpdated)
{
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

PersistentNode *PersistentNode::updateOrCopy(PersistentNode *node, unsigned version)
{
    if (node == nullptr)
        throw "ahhhh!";

    PointerStatus childToBeUpdated = (node->key < this->key ? Left : Right);

    // pode bugar com ponteiros nulos?
    if (node != nullptr)
        if (node == getLeft(version) || node  == getRight(version))
            return this;

    if (update(node, version, childToBeUpdated))
        return this;

    PersistentNode* thisCopy = new PersistentNode(this, version);
    thisCopy->update(node, version, childToBeUpdated);
    return thisCopy;
}

PersistentNode *PersistentNode::updateOrCopy(PersistentNode *node, unsigned version, PointerStatus childToBeUpdated)
{
    // pode bugar com ponteiros nulos?
    if (node != nullptr)
        if (node == getLeft(version) || node  == getRight(version))
            return this;

    if (update(node, version, childToBeUpdated))
        return this;

    PersistentNode* thisCopy = new PersistentNode(this, version);
    thisCopy->update(node, version, childToBeUpdated);
    return thisCopy;
}

PersistentNode *PersistentNode::getLeft(const unsigned &version)
{
    if ((extraPointerStatus == Left) && (version >= lastUpdateVersion))
        return extraPointer;
    return left;
}

PersistentNode *PersistentNode::getRight(const unsigned &version)
{
    if ((extraPointerStatus == Right) && (version >= lastUpdateVersion))
        return extraPointer;
    return right;
}

bool PersistentNode::isRed()
{
    return this->color == Red;
}

NodeColor PersistentNode::getColor() const
{
    return color;
}

void PersistentNode::setColor(const NodeColor &value)
{
    color = value;
}


