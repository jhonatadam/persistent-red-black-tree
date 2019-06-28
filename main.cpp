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

        if (str == "insert") {
            int key;
            cin >> key;
            rbt.insert(key);
        }
        else if (str == "show") {
            unsigned version;
            cin >> version;
            cout << rbt.toString(version) << endl;
        }
        else if (str == "stop") {
            break;
        }
    }
    return 0;
}
