#include "persistentredblacktree.h"

PersistentRedBlackTree::PersistentRedBlackTree()
{

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
        if (key < currentNode->getKey()) {
            successorNode = currentNode;
            currentNode = currentNode->getLeft(version);
        }
        else {
            currentNode = currentNode->getRight(version);
        }
    }

    if( successorNode != nullptr)
        return successorNode->getKey();

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
        if (key > currentNode->getKey()) {
            predecessorNode = currentNode;
            currentNode = currentNode->getRight(version);
        }
        else {
            currentNode = currentNode->getLeft(version);
        }
    }

    if( predecessorNode != nullptr)
        return predecessorNode->getKey();

    // se não tem predecessor, então retorna o maior inteiro (que não pode ser predecessor
    // de ninguém).
    return numeric_limits<int>::max();
}
