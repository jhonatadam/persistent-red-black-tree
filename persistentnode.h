#ifndef PERSISTENTNODE_H
#define PERSISTENTNODE_H

#include <utility>

using namespace std;

enum PointerStatus {None, Right, Left};
enum NodeColor {Red, Black};

class PersistentNode
{
public:


    const int key;

    PersistentNode(const int &key, unsigned version);

    PersistentNode(PersistentNode *other,
                   unsigned version);

    bool update(PersistentNode *node, unsigned version);
    bool update(PersistentNode *node, unsigned version, PointerStatus childToBeUpdated);
    PersistentNode * updateOrCopy(PersistentNode *node, unsigned version);
    PersistentNode * updateOrCopy(PersistentNode *node, unsigned version, PointerStatus childToBeUpdated);
    PersistentNode * getLeft(const unsigned &version);
    PersistentNode * getRight(const unsigned &version);
    bool isRed();
    void setColor(const NodeColor &value);
    NodeColor getColor() const;

private:
    NodeColor color;
    PersistentNode* left;
    PersistentNode* right;
    PersistentNode* extraPointer;
    PointerStatus extraPointerStatus;
    unsigned lastUpdateVersion;
};

#endif // PERSISTENTNODE_H
