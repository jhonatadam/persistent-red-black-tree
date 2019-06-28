#include "persistentredblacktree.h"

PersistentRedBlackTree::PersistentRedBlackTree()
{

}

unsigned PersistentRedBlackTree::insert(const int &key)
{
    unsigned version = unsigned(accessPointers.size());

    // if there is no version
    if (accessPointers.empty() || (accessPointers.back() == nullptr)) {
        accessPointers.push_back(new PersistentNode(key, nullptr, nullptr));
        return version;
    }

    stack<PersistentNode*> path;
    PersistentNode* currentNode = accessPointers.back();

    while (currentNode != nullptr) {
        path.push(currentNode);
        if (key < path.top()->key)
            currentNode = currentNode->getLeft(version);
        else
            currentNode = currentNode->getRight(version);
    }

    currentNode = new PersistentNode(key, version);

    while (!path.top()->update(currentNode, version))
    {
        PersistentNode* parentCopy = new PersistentNode(path.top()->key, version);
        parentCopy->update(currentNode, version);
        path.pop();
        path.push(parentCopy);
    }

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

void PersistentRedBlackTree::treeToString(PersistentNode *node, string &str, unsigned version)
{
    if (node == nullptr)
    {
        str = "[]";
        return;
    }

    str = "[" + to_string(node->key) + ", ";
    treeToString(node->getLeft(version), str, version);
    str = ", ";
    treeToString(node->getRight(version), str, version);
    str = "]";
}
