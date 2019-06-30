#include "persistentredblacktree.h"

PersistentRedBlackTree::PersistentRedBlackTree()
{

}

unsigned PersistentRedBlackTree::insert(const int &key)
{
    unsigned version = unsigned(accessPointers.size());

    // if there is no version
    if (accessPointers.empty() || (accessPointers.back() == nullptr)) {
        accessPointers.push_back(new PersistentNode(key, version));
        accessPointers.back()->setColor(Black);
        return version;
    }

    stack<PersistentNode*> path;
    PersistentNode* currentNode = accessPointers.back();

    while (currentNode != nullptr) {
        path.push(currentNode);
        if (key < currentNode->key)
            currentNode = currentNode->getLeft(version);
        else
            currentNode = currentNode->getRight(version);
    }

    path.push(new PersistentNode(key, version));
    // ligando novo nó à arvore
//    currentNode = new PersistentNode(key, version);
//    PersistentNode* currentNodeParent = path.top()->updateOrCopy(currentNode, version);
//    path.pop();
//    path.push(currentNodeParent);
//    path.push(currentNode);

    accessPointers.push_back(insertFixup(path, version));
    accessPointers.back()->setColor(Black);
    return version;
}

int PersistentRedBlackTree::successor(const int &key, const unsigned &version)
{
    if (version >= accessPointers.size())
        throw "sucessor(): A versão passada ainda não existe.";

    // ponteiro para o candidato a sucessor
    PersistentNode *successorNode = nullptr;
    // nó utilizado na busca
    PersistentNode *currentNode = accessPointers[version];
    while (currentNode != nullptr) {
        if (key < currentNode->key) {
            successorNode = currentNode;
            currentNode = currentNode->getLeft(version);
        }
        else {
            currentNode = currentNode->getRight(version);
        }
    }

    if( successorNode != nullptr)
        return successorNode->key;

    // se não tem sucessor, então retorna o menor inteiro (que não pode ser sucessor
    // de ninguém).
    return numeric_limits<int>::min();
}

int PersistentRedBlackTree::predecessor(const int &key, const unsigned &version)
{
    if (version >= accessPointers.size())
        throw "sucessor(): A versão passada ainda não existe.";

    // ponteiro para o candidato a sucessor
    PersistentNode *predecessorNode = nullptr;
    // nó utilizado na busca
    PersistentNode *currentNode = accessPointers[version];
    while (currentNode != nullptr) {
        if (key > currentNode->key) {
            predecessorNode = currentNode;
            currentNode = currentNode->getRight(version);
        }
        else {
            currentNode = currentNode->getLeft(version);
        }
    }

    if( predecessorNode != nullptr)
        return predecessorNode->key;

    // se não tem predecessor, então retorna o maior inteiro (que não pode ser predecessor
    // de ninguém).
    return numeric_limits<int>::max();
}

string PersistentRedBlackTree::toString(unsigned version)
{
    if (version >= accessPointers.size())
        throw "sucessor(): A versão passada ainda não existe.";

    string str;
    treeToString(accessPointers[version], str, version);
    return str;
}

PersistentNode *PersistentRedBlackTree::insertFixup(stack<PersistentNode *> path, const unsigned & version)
{
    PersistentNode *node = path.top();
    path.pop();

    while (path.top()->isRed()) {
        PersistentNode *nodeParent = path.top()->updateOrCopy(node, version);
        path.pop();

        if (nodeParent->key < path.top()->key) {
            PersistentNode * nodeUncle = path.top()->getRight(version);

            // troca de cores
            if (isRed(nodeUncle)) {
                nodeParent->setColor(Black);
                nodeUncle->setColor(Black);
                path.top()->setColor(Red);

                node = path.top()->updateOrCopy(nodeParent, version);
                path.pop();
                if (path.empty())
                    break;
            } else { // rotação
                if (node == nodeParent->getRight(version)) {
                    node = nodeParent;
                    nodeParent = rotateLeft(nodeParent,version);
                }

                nodeParent->setColor(Black);
                path.top()->setColor(Red);

                PersistentNode * nodeGrandParent = path.top()->updateOrCopy(nodeParent, version);
                path.pop();
                path.push(rotateRight(nodeGrandParent, version));
             }

        } else {
            PersistentNode * nodeUncle = path.top()->getLeft(version);

            // troca de cores
            if (isRed(nodeUncle)) {
                nodeParent->setColor(Black);
                nodeUncle->setColor(Black);
                path.top()->setColor(Red);

                node = path.top()->updateOrCopy(nodeParent, version);
                path.pop();
                if (path.empty())
                    break;
            } else { // rotação
                if (node == nodeParent->getLeft(version)) {
                    node = nodeParent;
                    nodeParent = rotateRight(nodeParent,version);
                }

                nodeParent->setColor(Black);
                path.top()->setColor(Red);

                PersistentNode * nodeGrandParent = path.top()->updateOrCopy(nodeParent, version);
                path.pop();
                path.push(rotateLeft(nodeGrandParent, version));
             }
        }
    }

    while(!path.empty()) {
        node = path.top()->updateOrCopy(node, version);
        path.pop();
    }

    return node;
}

PersistentNode *PersistentRedBlackTree::rotateLeft(PersistentNode *node, const unsigned & version)
{
    PersistentNode * aux = node->getRight(version);
    node = node->updateOrCopy(aux->getLeft(version), version, Right);
    aux = aux->updateOrCopy(node, version, Left);
    return aux;
}

PersistentNode *PersistentRedBlackTree::rotateRight(PersistentNode *node, const unsigned & version)
{
    PersistentNode * aux = node->getLeft(version);
    node = node->updateOrCopy(aux->getRight(version), version, Left);
    aux = aux->updateOrCopy(node, version, Right);
    return aux;
}

bool PersistentRedBlackTree::isRed(PersistentNode *node)
{
    if (node == nullptr)
        return false;
    return node->isRed();
}

void PersistentRedBlackTree::treeToString(PersistentNode *node, string &str, unsigned version)
{
    if (node == nullptr)
    {
        str += "[]";
        return;
    }

    str += "[" + to_string(node->key) + ":" + (node->isRed() ? "r" : "b") + ", ";
    treeToString(node->getLeft(version), str, version);
    str += ", ";
    treeToString(node->getRight(version), str, version);
    str += "]";
}
