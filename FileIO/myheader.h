#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define INTRO 0
#define REGSITER 1
#define LOGIN 2
#define ENTER_USERNAME 1
#define ENTER_PASSWORD 2
#define INVALID_INPUT 3
#define REGISTERED 4
#define USER_EXIST 5

void printOptions(int situation){
    switch(situation){
        case INTRO:
        cout << "What do you want to do?" << endl;
        cout << "(1) Register" << endl;
        cout << "(2) Login" << endl;
        break;
    }
    cout << endl << "Your decision: ";
}

void printLine () {
    for(int i = 0; i < 30; i++)
        cout << "=";
    cout << endl; 
}

void printLine (int count) {
    for(int i = 0; i < count; i++)
        cout << "=";
    cout << endl; 
}

void printMessage (int situation) {
    switch (situation) {
        case ENTER_USERNAME:
        cout << "Enter your username: ";
        break;
        case ENTER_PASSWORD:
        cout << "Enter your password (spaces are illegal): ";
        break;
        case INVALID_INPUT:
        cout << "Invalid input!" << endl;
        break;
        case REGISTERED:
        cout << "Registered successfully!" << endl;
        cout << "Welcome: ";
        break;
        case USER_EXIST:
        cout << "The username already exists!" << endl;
        cout << "Try another one!" << endl;
        break;
    }
}

void registerUser () {

    ofstream outFile;

    bool userExist;
    string username;

    do {

        ifstream inFile;
        userExist = false;

        printMessage(ENTER_USERNAME);
        cin >> username;

        inFile.open("users.txt");
        while (!inFile.eof()) { 
            string tempUsername;
            int tempPassword;

            inFile >> tempUsername >> tempPassword;
            if (username == tempUsername){
                printMessage(USER_EXIST);
                userExist = true;
                break;
            }
        }
        inFile.close();

    } while (userExist);

    printMessage(ENTER_PASSWORD);
    string password;
    cin >> password;
    outFile.open("users.txt", ios::app);
    outFile << username << " " << password << "\n";
    printMessage(REGISTERED);
    cout << username << "!" << endl;

    outFile.close();
}