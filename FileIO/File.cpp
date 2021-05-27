#include <iostream>
#include <fstream>
#include "myheader.h"
#include <string>

using namespace std;

int main(){
    int act;
    while (true) {
        printOptions(INTRO);
        cin >> act;
        if(act == REGSITER || act == LOGIN)
            break;
        else
            printMessage(INVALID_INPUT);
    }
    if (act == REGSITER)
        registerUser();
    else if (act == LOGIN) {

    }

    int tmp;
    cin >> tmp;

    return 0;
}

