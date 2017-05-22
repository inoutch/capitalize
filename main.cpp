#include <iostream>
#include "capthread.h"
#include "capfork.h"

using namespace std;

void strcpy(const string src, char *dst, size_t size) {
    if (size + 1 < src.size()) {
        return;
    }
    memcpy(dst, src.c_str(), src.size());
    dst[src.size()] = '\0';
}

int main() {
    string input;
    int type;

    cout << "type [0=q2.1, 1=q2.2, 2=q2.3]?";
    cin >> type;

    cout << "input \t= ";
    cin >> input;

    size_t size = input.size() + 1;
    char *str = new char[size];
    strcpy(input, str, size);

    if (type == 0) {
        cout << "convert by capthread" << endl;
        pthread_t threadId;
        pthread_create(&threadId, NULL, capthread, str);
        pthread_join(threadId, NULL);

    } else if (type == 1) {
        cout << "convert by capfork" << endl;
        capfork(str, size);

    } else if (type == 2) {
        cout << "convert by capfork (read & write)" << endl;
        auto ret = capfork2(str, size);
        memcpy(str, &ret[0], ret.size());

    }

    cout << "output \t= " + string(str) << endl;

    delete[] str;
    return 0;
}