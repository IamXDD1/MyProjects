#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define GAME_NAME "Shabbycraft"

//printMessage(int condition <--- )
#define WELCOME 0
#define REGISTER 1
#define GOOD_NAME 2
#define ENJOY 3
#define MOVEMENT 4
#define END_GAME 5
#define THANKS_FOR_PLAYING 6
#define HOW_TO_PLAY 7
#define INTRO_OPTIONS 8
#define PRESS_ANY_KEY 9

//printMessage(int condition <--- , string username)
#define HELLO 0
#define INTERVAL 1

//loginInterval() decide wut unit of time it returns
#define MIN 60

//intro options
#define OPTION1 1
#define OPTION2 2
#define OPTION3 3

void printMessage(int condition)
{
    switch (condition)
    {
    case REGISTER:
        cout << "Seems like this is the very first time for you playing this game." << endl;
        cout << "So, what should we call you(spaces are illegal):";
        break;
    case GOOD_NAME:
        cout << "That's a really good name!" << endl;
        break;
    case ENJOY:
        cout << "Now, enjoy the game!" << endl;
        break;
    case MOVEMENT:
        cout << "Act:";
        break;
    case END_GAME:
        cout << "Are you sure you want to quit(y/others)?";
        break;
    case THANKS_FOR_PLAYING:
        cout << "Thank you for playing Shabbycraft!" << endl;
        cout << "You're welcomed to play again soon!" << endl;
        break;
    case HOW_TO_PLAY:
        cout << "___________________________________________________________" << endl;
        cout << "|                                                          |" << endl;
        cout << "|       -Controls:                                         |" << endl;
        cout << "|          -> wasd(must be lowerCase) to Move.             |" << endl;
        cout << "|          -> tfgh(must be lowerCase) to Build.            |" << endl;
        cout << "|          -> j(must be lowerCase) to Break Entities.      |" << endl;
        cout << "|           ps. Can only break entities 1 block near.      |" << endl;
        cout << "|          -> c(must be lowerCase) to Craft.               |" << endl;
        cout << "|          -> r(must be lowerCase) to Change Hot Bar.      |" << endl;
        cout << "|          -> e(must be lowerCase) to Open Inventory.      |" << endl;
        cout << "|          -> = to Terminate Game.                         |" << endl;
        cout << "|                                                          |" << endl;
        cout << "|       -Marks On The Map:                                 |" << endl;
        cout << "|          -> #'s on the map represent BORDERS.            |" << endl;
        cout << "|          -> T's on the map represent TREES.              |" << endl;
        cout << "|          -> O's on the map represent ROCKS.              |" << endl;
        cout << "|          -> H's on the map represent PLANKS.             |" << endl;
        cout << "|          -> L's on the map represent LOGS.               |" << endl;
        cout << "|          -> C's on the map represent COBBLES.            |" << endl;
        cout << "|          -> %'s on the map represent CRAFTING_TABLES.    |" << endl;
        cout << "|          -> ='s on the map represent DOORS.              |" << endl;
        cout << "|          -> @ on the map represents YOU.                 |" << endl;
        cout << "|                                                          |" << endl;
        cout << "|       -Tips, Mechanisms:                                 |" << endl;
        cout << "|          -> You can craft extra stuff when standing      |" << endl;
        cout << "|             near a CRAFTING_TABLE!                       |" << endl;
        cout << "|          -> You can walk through a DOOR!                 |" << endl;
        cout << "|          -> Chop down some TREES or Mine some ROCKS to   |" << endl;
        cout << "|             to build your own house!                     |" << endl;
        cout << "|                                                          |" << endl;
        cout << "|__________________________________________________________|" << endl;
        break;

    case INTRO_OPTIONS:
        cout << "___________________________________________________________" << endl;
        cout << "|                 [Welcome To ShabbyCraft]                 |" << endl;
        cout << "|                                                          |" << endl;
        cout << "|                         Options:                         |" << endl;
        cout << "|                                                          |" << endl;
        cout << "|             [--------(1)How to play--------]             |" << endl;
        cout << "|             [------(2)Start New Game-------]             |" << endl;
        cout << "|             [----(3)Load Previous Save-----]             |" << endl;
        cout << "|                                                          |" << endl;
        cout << "|__________________________________________________________|" << endl;
        cout << "|         | Author: James Ku ; Version: Pre_Alpha |        |" << endl;
        cout << "|__________________________________________________________|" << endl;
        break;
    case PRESS_ANY_KEY:
        cout << "Enter any key to go back:";
        break;
    }
}

void printMessage(int condition, string output)
{
    switch (condition)
    {
    case HELLO:
        cout << "Hello, " << output << "!" << endl;
        break;
    case INTERVAL:
        cout << "The previous login was: " << output << " ago" << endl;
        break;
    }
}