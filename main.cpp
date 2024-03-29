#include <iostream>
#include <persistentredblacktree.h>

using namespace std;

int main()
{
    PersistentRedBlackTree rbt;

    while (true) {
        cout << ">> ";

        string str;
        cin >> str;

        int key;
        unsigned version;

        if (str == "in") {
            cin >> key;
            rbt.insert(key);
        }
        else if (str == "sh") {
            cin >> version;
            cout << rbt.toString(version) << endl;
        }
        else if (str == "su") {
            cin >> key >> version;
            cout << rbt.successor(key, version) << endl;
        }
        else if (str == "pr") {
            cin >> key >> version;
            cout << rbt.predecessor(key, version) << endl;
        }
        else if (str == "rm"){
            cin >> key;
            rbt.remove(key);
        }
        else if (str == "stop") {
            break;
        }
    }
    return 0;
}
