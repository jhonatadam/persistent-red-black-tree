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

    accessPointers.push_back(insertFixup(path, version));
    accessPointers.back()->setColor(Black);
    return version;
}

unsigned PersistentRedBlackTree::remove(const int &key)
{
    unsigned version = unsigned(accessPointers.size());

    // if there is no version
    if (accessPointers.empty() || (accessPointers.back() == nullptr))
        return version - 1;

    stack<PersistentNode*> path;
    PersistentNode* currentNode = accessPointers.back();

    while (currentNode != nullptr) {
        if (key == currentNode->key)
            break;

        path.push(currentNode);

        if (key < currentNode->key)
            currentNode = currentNode->getLeft(version);
        else
            currentNode = currentNode->getRight(version);
    }

    NodeColor successorColor;
    // a chave não está na árvore
    if (currentNode == nullptr) {
        return version - 1;
    }
    PointerStatus status;//essa variável indica se o ultimo no da pilha é um filho a esquerda ou direita

    // o nó a ser removido não tem filhos
    if (currentNode->getLeft(version) == nullptr && currentNode->getRight(version) == nullptr) {
        if (path.empty()) { // a arvore tem apenas um nó
            accessPointers.push_back(nullptr);
            return version;
        }

        status = (currentNode->key < path.top()->key ? Left : Right);
        PersistentNode * nodeParent = path.top()->updateOrCopy(nullptr, version, status);
        successorColor = currentNode->getColor();
        path.pop();
        path.push(nodeParent);
        path.push(nullptr);//para ser consistente com os demais casos
    } // o nó a ser removido tem apenas filho a direita
    else if (currentNode->getLeft(version) == nullptr) {
        successorColor = currentNode->getColor();
        path.push(currentNode->getRight(version));
        status = Right;
    } // o nó a ser removido tem apenas filho a esquerda
    else if (currentNode->getRight(version) == nullptr) {
        successorColor = currentNode->getColor();
        path.push(currentNode->getLeft(version));
        status = Left;
    } // o nó tem dois filhos
    else {

        queue<PersistentNode*> pathToSucessor;
        PersistentNode* nodeSuccessor = currentNode->getRight(version);
        while (nodeSuccessor->getLeft(version) != nullptr) {
            pathToSucessor.push(nodeSuccessor);
            nodeSuccessor = nodeSuccessor->getLeft(version);
        }
        successorColor = nodeSuccessor->getColor();
        PersistentNode* successorChild = nodeSuccessor->getRight(version);
        status = (nodeSuccessor == currentNode->getRight(version)? Right: Left);// Precisa atualizar o status pois succChild pode ser nulo

        nodeSuccessor = nodeSuccessor->updateOrCopy(currentNode->getLeft(version),version, Left);// atualiza o filho esquerdo do sucessor
        nodeSuccessor->setColor(currentNode->getColor());
        path.push(nodeSuccessor);

        pathToSucessor.push(successorChild);
        while(!pathToSucessor.empty()){
            path.push(pathToSucessor.front());
            pathToSucessor.pop();
        }

    }

    if(successorColor == Black){
        accessPointers.push_back(removeFixup(path, status ,version));
        accessPointers.back()->setColor(Black);
    }
    else{
        currentNode = path.top();
        path.pop();
        while(!path.empty()){
            currentNode = (currentNode == nullptr? path.top()->updateOrCopy(currentNode,version, status) : path.top()->updateOrCopy(currentNode,version));
            path.pop();
        }
        accessPointers.push_back(currentNode);
    }

    //accessPointers.back()->setColor(Black); a arvore pode ter ficado vazia
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
    string str;
    if (version >= accessPointers.size()){
        throw "sucessor(): A versão passada ainda não existe.";
        //treeToString(accessPointers.back(), str, (version>0)? static_cast<unsigned>(accessPointers.size()-1): 0);
    }

    treeToString(accessPointers[version], str, version);
    return str;
}

PersistentNode *PersistentRedBlackTree::insertFixup(stack<PersistentNode *> path,  const unsigned & version)
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

//passando esse statuslNullNode pro caso em que o topo da pilha é null, pra decidir qual lado ir. Qqr outra solução é bem vinda
PersistentNode *PersistentRedBlackTree::removeFixup(stack<PersistentNode *> path, PointerStatus statusNode, const unsigned &version)
{
    bool flagCase4 = false;
    while (path.size() > 1 && !isRed(path.top())) {
        PersistentNode *node = path.top();
        path.pop();
        if(node != nullptr)
            statusNode = (node->key < path.top()->key ? Left : Right);

        if (statusNode == Left) {
            PersistentNode * nodeBrother = path.top()->getRight(version);

            // troca de cores
            if (isRed(nodeBrother)) {
                nodeBrother->setColor(Black);
                PersistentNode* nodeParent = path.top();
                path.pop();

                nodeParent->setColor(Red);
                path.push(rotateLeft(nodeParent, version));
                nodeBrother = nodeParent->getRight(version);// após a rotação na linha anterior node tem um novo irmão
                path.push(nodeParent); //para manter consistência com o topo sendo o pai de node e nodeBrother

            }
            if(!isRed(nodeBrother->getLeft(version)) && !isRed(nodeBrother->getRight(version))){// os dois filhos são pretos
                nodeBrother->setColor(Red);
                //path.pop(); pois demos no início
            } else{
                PersistentNode *nodeParent = path.top();// remove logo pro caso de entrar no próx if
                path.pop();
                if(!isRed(nodeBrother->getRight(version))) {//isso implica que filho esquerdo existe e não é nulo, pois é Red
                    nodeBrother->getLeft(version)->setColor(Black);
                    nodeBrother->setColor(Red);

                    nodeBrother = rotateRight(nodeBrother,version); //filho esquerdo de nodeBrother ser torna irmão de node
                    nodeParent  = nodeParent->updateOrCopy(nodeBrother, version);//a próx rotação assume esse update
                    nodeBrother = path.top();
                }

                nodeBrother->setColor(nodeParent->getColor());
                nodeParent->setColor(Black);
                if (nodeBrother->getRight(version) != nullptr)
                    nodeBrother->getRight(version)->setColor(Black);
                path.push(rotateLeft(nodeParent, version));

                flagCase4 = true;
                break;
            }
        }else { // caso simétrico
            PersistentNode * nodeBrother = path.top()->getLeft(version);

            // troca de cores
            if (isRed(nodeBrother)) {
                nodeBrother->setColor(Black);
                PersistentNode* nodeParent = path.top();
                path.pop();

                nodeParent->setColor(Red);
                path.push(rotateRight(nodeParent, version));
                nodeBrother = nodeParent->getLeft(version);// após a rotação na linha anterior node tem um novo irmão
                path.push(nodeParent); //para manter consistência com o topo sendo o pai de node e nodeBrother

            }
            if(nodeBrother->getRight(version)->getColor() == Black &&
                    nodeBrother->getLeft(version)->getColor() == Black){// os dois filhos são pretos
                nodeBrother->setColor(Red);
                //path.pop(); pois demos no início

            } else{
                PersistentNode *nodeParent = path.top();// remove logo pro caso de entrar no próx if
                path.top();
                if(nodeBrother->getLeft(version)->getColor() == Black) {//isso implica que filho esquerdo existe e não é nulo, pois é Red
                    nodeBrother->getRight(version)->setColor(Black);
                    nodeBrother->setColor(Red);

                    nodeBrother = rotateLeft(nodeBrother,version); //filho esquerdo de nodeBrother ser torna irmão de node
                    nodeParent  = nodeParent->updateOrCopy(nodeBrother, version);//a próx rotação assume esse update
                    nodeBrother = path.top();
                }
                nodeBrother->setColor(nodeParent->getColor());
                nodeParent->setColor(Black);
                if (nodeBrother->getLeft(version) != nullptr)
                    nodeBrother->getLeft(version)->setColor(Black);
                path.push(rotateRight(nodeParent, version));
                flagCase4 = true;
                break;
            }
        }
    }

    if(path.empty()){ //não sei se pode acontecer, mas caso aconteça
        return  nullptr;
    }else{
        PersistentNode *node = path.top();
        path.pop();
        if(!flagCase4)//para o caso em que terminamos por encontrar um nó red
            node->setColor(Black);
        while(!path.empty()) {
            node = path.top()->updateOrCopy(node, version);
            path.pop();
        }
        return node;
    }
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
