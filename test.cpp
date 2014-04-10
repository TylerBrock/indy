#include <iostream>
#include <cstdlib>

using namespace std;

bool less_than_five(int);

int main(int argc, char** argv) {
    int i;

    if (argc == 1) {
        cout << "usage: test n" << endl;
        return EXIT_FAILURE;
    } else {
        i = atoi(argv[1]);
    }

    if (less_than_five(i)) {
        cout << "less than 5" << endl;
    } else {
        cout << "greater than or equal to 5" << endl;
    }
}

bool less_than_five(int n) {
    if (n < 5)
        return true;
    else
        return false;
}

