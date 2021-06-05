#include "soundEffects.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <iomanip>
#include <conio.h>
#include <windows.h>


using namespace std;

#define MAX_ENTITY_DROP 4
#define MAX_MOB_DROP 3

#define TREE_ENTITY 0
#define ROCK_ENTITY 1

//ENTITIES
#define TYPES 2

//Item
#define LOG 0
#define PLANK 1
#define COBBLE 2
#define CRAFTING_TABLE 3
#define WOODEN_SWORD 4
#define DOOR 5
#define PORK 6

//Player

//Layout
#define MAP_HEIGHT 100
#define MAP_WIDTH 200

#define SIGHT_HEIGHT 25
#define SIGHT_WIDTH 49
//should be odd

#define VOID_REF 0
#define TREE_REF 1
#define ROCK_REF 2
#define PLAYER_REF 3
#define PLANK_REF 4
#define LOG_REF 5
#define COBBLE_REF 6
#define CRAFTING_TABLE_REF 7
#define DOOR_REF 8
#define PIG_REF 9

#define VOID_ ' '
#define BORDER '#'
#define TREE 'T'
#define ROCK 'O'
#define PLAYER '@'
#define PLANK_BLOCK 'H'
#define LOG_BLOCK 'L'
#define COBBLE_BLOCK 'C'
#define CRAFTING_TABLE_BLOCK '%'
#define DOOR_BLOCK '='
#define PIG_CHAR 'P'

const static int FOREGROUND_WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
const static int FOREGROUND_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;
const static int FOREGROUND_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE;
const static int FOREGROUND_LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const static int FOREGROUND_LIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const static int FOREGROUND_LIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
const static int FOREGROUND_LIGHT_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const static int FOREGROUND_LIGHT_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;


HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

//System
static int MAX_ENTITIES = 200;
static int MAX_MOBS = 10;
void loadArguments() {
    ifstream inFile_maxEntities("arguments/maxEntities.txt");
    if (inFile_maxEntities)
        inFile_maxEntities >> MAX_ENTITIES;
    inFile_maxEntities.close(); 
    
    ifstream inFile_maxMobs("arguments/maxMobs.txt");
    if (inFile_maxMobs)
        inFile_maxMobs >> MAX_MOBS;
    inFile_maxMobs.close();
}

//controls
#define UP 'w'
#define DOWN 's'
#define RIGHT 'd'
#define LEFT 'a'
#define BREAK 'j'
#define USE 'k'
#define END '='
#define CHECK_INVENTORY 'e'
#define PLACE_LEFT 'f'
#define PLACE_RIGHT 'h'
#define PLACE_UP 't'
#define PLACE_DOWN 'g'
#define CHANGE_HOT_BAR 'r'
#define CRAFT 'c'

#define UP_DIR 0
#define DOWN_DIR 1
#define RIGHT_DIR 2
#define LEFT_DIR 3

#define NOT_FOUND -1


struct Entity {

    int type;
    int x;
    int y;
    int drop;

    Entity(int type, int x, int y, int drop){
        this->type = type;
        this->x = x;
        this->y = y;
        this->drop = drop;
    }
};

struct Block {

    int type;
    int x;
    int y;

    Block (int type, int x, int y) {
        this->type = type;
        this->x = x;
        this->y = y;
    }
};

struct Item {
    
    int type;
    int quantity;

    Item (int type, int quantity) {
        this->type = type;
        this->quantity = quantity;
    }
};

struct Player {

    int x;
    int y;
    double health;
    int balance;
    vector <Item> inventory;

};

struct Layout {

    int ref[MAP_HEIGHT][MAP_WIDTH];
    int sightLeftBnd;
    int sightUpBnd;

};

#define MOB_PIG 0
struct Mob {
    double health;
    int x;
    int y;
    int type;
    Item drop = Item(0, 0);
    
    Mob (int type, double health, int x, int y, int drop_type, int drop_quan) {
        this->type = type;
        this->health = health;
        this->x = x;
        this->y = y;
        drop.type = drop_type;
        drop.quantity = drop_quan;
    }
};

#define GAME_DAY 720
#define GAME_HOUR 30
#define GAME_2_MINUTE 1//sec
struct Time {

    long long int startTime;
    long long int currTime;
    int gameDay;
    int gameHour;
    int gameMinute;
    //1 gameDay = 12mins, 1 gameHour = 30sec, 2 gameMinute = 1sec
};

class System {
    private:

    Player player;
    Layout layout;
    vector <Entity> entity;
    vector <Block> block;
    vector <Mob> mob;
    int hotBarIndex;
    Time time;

    public:

    void setTime(bool loadSave);
    void updateTime();
    void setMap();
    void setSight();
    void generateEntity();
    void checkEntities();
    void generateMob();
    void checkMobs();
    void updateMobs();
    void setPlayer(bool loadSave);
    void printLayout();
    void movePlayer(char input);
    void triggerBnd(int direction);
    void breakBlock();
    void breakEntity();
    void collectDrop(int index, int form);
    void interact(char input);
    Player getPlayer(){return player;}
    Layout getLayout(){return layout;}
    vector <Block> getBlock () {return block;}
    void openInventory();
    void placeBlock(char input);
    void setBlock(int item, int x, int y);
    void changeHotBar();
    int indexOfItem(int item_type);
    bool isBlockNear(int blockRef);
    bool isEntity(int ref);
    bool isBlock(int ref);
    void craft();
    void refreshInventory();
    
};

long long getPrevLoginTime () {
    long long int prevLogin;
    ifstream inFile_time("saves/time.txt");
    inFile_time >> prevLogin;
    inFile_time.close();
    return prevLogin;
}

long long getCurrentTime () {
    return time(nullptr);
}

void printColoredWord (string output, int color) {
    SetConsoleTextAttribute(console, color);
    cout << output;
    SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
}

void alignAndOutput(int input, int span) {
    if (span == 2) {
        if (input < 10)
            cout << "0";
        cout << input;
    }
    else if (span == 3) {
        if (input < 10)
            cout << "00";
        else if (input < 100)
            cout << "0";
        cout << input;
    }
    else if (span == 4) {
        if (input < 10)
            cout << "000";
        else if (input < 100)
            cout << "00";
        else if (input < 1000)
            cout << "0";
        cout << input;
    }
}

void printHealthBar(int health) {
    if (health >= 87.5)
        cout << "[]";
    else
        cout << "__";
    if (health >= 75)
        cout << "[]";
    else
        cout << "__";
    if (health >= 62.5)
        cout << "[]";
    else
        cout << "__";
    if (health >= 50)
        cout << "[]";
    else
        cout << "__";
    if (health >= 37.5)
        cout << "[]";
    else
        cout << "__";
    if (health >= 25)
        cout << "[]";
    else
        cout << "__";
    if (health >= 12.5)
        cout << "[]";
    else
        cout << "__";
    if (health > 0)
        cout << "[]";
    else
        cout << "__";
}

void System::setTime (bool loadSave) {
    if (loadSave) {
        ifstream inFile_time("saves/time.txt");
        inFile_time >> time.startTime;
        inFile_time.close();
        time.currTime = getCurrentTime();
    }
    else {
        time.startTime = getCurrentTime();
        time.currTime = getCurrentTime();
        ofstream outFile_time("saves/time.txt");
        outFile_time << time.currTime;
        outFile_time.close();
    }

    int interval = time.currTime - time.startTime;

    time.gameDay = interval / GAME_DAY;interval %= GAME_DAY;
    time.gameHour = interval / GAME_HOUR;interval %= GAME_HOUR;
    time.gameMinute = (interval / GAME_2_MINUTE) * 2;
}

void System::updateTime () {
    time.currTime = getCurrentTime();
    int interval = time.currTime - time.startTime;

    time.gameDay = interval / GAME_DAY;interval %= GAME_DAY;
    time.gameHour = interval / GAME_HOUR;interval %= GAME_HOUR;
    time.gameMinute = (interval / GAME_2_MINUTE) * 2;
    return;
}

void System::generateEntity() {
    int rand_x = rand() % (MAP_WIDTH - 1) + 1;
    int rand_y = rand() % (MAP_HEIGHT - 1) + 1;
    int rand_type = rand() % TYPES;
    int rand_drop = rand() % MAX_ENTITY_DROP + 2;

    while (layout.ref[rand_y][rand_x] != VOID_REF) {
        int random = rand() % 2;
        if (random == 0)
            rand_x++;
        else
            rand_y++;

        if (rand_x >= MAP_WIDTH - 1)
            rand_x -= (MAP_WIDTH - 2);
        if (rand_y >= MAP_HEIGHT - 1)
            rand_y -= (MAP_HEIGHT - 2); 
    }

    entity.push_back(Entity(rand_type, rand_x, rand_y, rand_drop));
    setMap();
}

void System::checkEntities () {
    while (entity.size() < MAX_ENTITIES) {
        generateEntity();
    }
}

void System::generateMob () {

    int generateChance = rand() % 100 + 1;

    if (generateChance > 1)
        return;

    int rand_x = rand() % (MAP_WIDTH - 1) + 1;
    int rand_y = rand() % (MAP_HEIGHT - 1) + 1;
    int rand_type = rand() % TYPES;
    int rand_quan = rand() % MAX_MOB_DROP + 1;

    while (layout.ref[rand_y][rand_x] != VOID_REF) {
        int random = rand() % 2;
        if (random == 0)
            rand_x++;
        else
            rand_y++;

        if (rand_x >= MAP_WIDTH - 1)
            rand_x -= (MAP_WIDTH - 2);
        if (rand_y >= MAP_HEIGHT - 1)
            rand_y -= (MAP_HEIGHT - 2); 
    }
    mob.push_back(Mob(MOB_PIG, 100, rand_x, rand_y, PORK, rand_quan));
    setMap();
}

void System::checkMobs () {
    while (mob.size() < MAX_MOBS) {
        generateMob();
    }
}

void System::updateMobs () {
    for (int i = 0; i < mob.size(); i++) {
        if (mob[i].type == MOB_PIG) {
            int random = rand() % 8 + 1;
            if (random > 4)
                continue;
            else {
                switch (random) {
                    case 1:
                    if (layout.ref[mob[i].y - 1][mob[i].x] == VOID_REF) {
                        mob[i].y--;
                        setMap();
                    }
                    break;
                    case 2:
                    if (layout.ref[mob[i].y + 1][mob[i].x] == VOID_REF) {
                        mob[i].y++;
                        setMap();
                    }
                    break;
                    case 3:
                    if (layout.ref[mob[i].y][mob[i].x + 1] == VOID_REF) {
                        mob[i].x++;
                        setMap();
                    }
                    break;
                    case 4:
                    if (layout.ref[mob[i].y][mob[i].x - 1] == VOID_REF) {
                        mob[i].x--;
                        setMap();
                    }
                    break;
                }
            }
        }
    }
}

void System::setPlayer (bool loadSave) {
    if (loadSave) {
        player.inventory.push_back(Item(LOG, 0));
        player.inventory.push_back(Item(PLANK, 0));
        player.inventory.push_back(Item(COBBLE, 0));
        
        player.balance = 0;

        ifstream inFile_pos("saves/coordinate.txt");
        int x = 0;
        int y = 0;
        inFile_pos >> x >> y;
        inFile_pos.close();

        if (x != 0 && y != 0) {
            player.y = y;
            player.x = x;
        }
        else {
            player.x = 1;
            player.y = 1;

        }

        ifstream inFile_inv("saves/inventory.txt");
        for (int i = 0; i < 3; i++) {
            int tmp;
            inFile_inv >> tmp;
            inFile_inv >> player.inventory[i].quantity;
        }
        int type;
        do {
            inFile_inv >> type;
            if (type != NOT_FOUND) {
                int quan;
                inFile_inv >> quan;
                player.inventory.push_back(Item(type, quan));
            }
        }while (type != NOT_FOUND);
        inFile_inv.close();

        ifstream inFile_sightBorder("saves/sightBorder.txt");
        inFile_sightBorder >> layout.sightLeftBnd >> layout.sightUpBnd;
        inFile_sightBorder.close();

        ifstream inFile_health("saves/health.txt");
        inFile_health >> player.health;
        inFile_health.close();

        ifstream inFile_block("saves/block.txt");
        int blockType;
        do {
            inFile_block >> blockType;
            if (blockType != NOT_FOUND) {
                int x, y;
                inFile_block >> x >> y;
                block.push_back(Block(blockType, x, y));
            }
        }while(blockType != NOT_FOUND);
        inFile_block.close();

        //hotbarindex to be contiue
    }
    else {
        player.inventory.push_back(Item(LOG, 0));
        player.inventory.push_back(Item(PLANK, 0));
        player.inventory.push_back(Item(COBBLE, 0));
        player.health = 100;
        player.balance = 0;

        player.x = 1;
        player.y = 1;
        layout.sightLeftBnd = 0;
        layout.sightUpBnd = 0;
        hotBarIndex = 0;
    }
}

void gotoXY(int x, int y)
{
    CursorPosition.X = x;                              // Locates column
    CursorPosition.Y = y;                              // Locates Row
    SetConsoleCursorPosition(console, CursorPosition); // Sets position for next thing to be printed
}

void System::printLayout () {
    gotoXY(0, 0);
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {

            if ((i >= layout.sightUpBnd && i <= layout.sightUpBnd + SIGHT_HEIGHT - 1) && (j >= layout.sightLeftBnd && j <= layout.sightLeftBnd + SIGHT_WIDTH - 1)) {

                if (i == layout.sightUpBnd || j == layout.sightLeftBnd || i == layout.sightUpBnd + SIGHT_HEIGHT - 1 || j == layout.sightLeftBnd + SIGHT_WIDTH - 1)
                    cout << BORDER;
                else {
                    switch (layout.ref[i][j]) {
                        case VOID_REF:SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);cout << VOID_;break;
                        case TREE_REF:SetConsoleTextAttribute(console, FOREGROUND_LIGHT_GREEN);cout << TREE;break;
                        case ROCK_REF:SetConsoleTextAttribute(console, FOREGROUND_LIGHT_BLUE);cout << ROCK;break;
                        case PLAYER_REF:SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);cout << PLAYER;break;
                        case PLANK_REF:SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);cout << PLANK_BLOCK;break;
                        case LOG_REF:SetConsoleTextAttribute(console, FOREGROUND_MAGENTA);cout << LOG_BLOCK;break;
                        case COBBLE_REF:SetConsoleTextAttribute(console, FOREGROUND_LIGHT_BLUE);cout << COBBLE_BLOCK;break;
                        case CRAFTING_TABLE_REF:SetConsoleTextAttribute(console, FOREGROUND_RED);cout << CRAFTING_TABLE_BLOCK;break;
                        case DOOR_REF:SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);cout << DOOR_BLOCK;break;
                        case PIG_REF:SetConsoleTextAttribute(console, FOREGROUND_LIGHT_RED);cout << PIG_CHAR;break;
                    }
                }
            }
        }
        const int tmp = layout.sightUpBnd;
        
        if (i == tmp)
            cout << "____________________";
        if (i == tmp + 1) {
            cout << "|     ";
            printColoredWord("x", FOREGROUND_WHITE);
            cout << "        ";
            printColoredWord("y", FOREGROUND_WHITE);
            cout << "    |";
        }
        if (i == tmp + 2) {
            cout << "|    ";
            SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
            alignAndOutput(player.x, 3);
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
            cout << "      ";
            SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
            alignAndOutput(player.y, 3);
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
            cout << "   |";

        }
        if (i == tmp + 3)
            cout << "|___________________|";
        if (i == tmp + 4) {
            cout << "|  ";
            printColoredWord("HP:", FOREGROUND_WHITE);
            cout << " ";
            SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
            alignAndOutput(player.health, 3);
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
            cout << "          |";
        }
        if (i == tmp + 5) {
            cout << "|  ";
            printHealthBar(player.health);
            cout << " |";
        }
        if (i == tmp + 6)
            cout << "|___________________|";
        if (i == tmp + 7) {
            cout << "|  ";
            printColoredWord("Hot Bar:", FOREGROUND_WHITE);
            cout << "         |";
        }
        if (i == tmp + 8) {
            switch (player.inventory[hotBarIndex].type) {
                case LOG:
                cout << "|     ";
                printColoredWord("Log", FOREGROUND_LIGHT_YELLOW);
                cout << "    - ";
                SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
                alignAndOutput(player.inventory[hotBarIndex].quantity, 3);
                SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
                cout << "  |";
                break;
                case PLANK:
                //cout << "|    Plank   - ";
                cout << "|    ";
                printColoredWord("Plank", FOREGROUND_LIGHT_YELLOW);
                cout << "   - ";
                SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
                alignAndOutput(player.inventory[hotBarIndex].quantity, 3);
                SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
                cout << "  |";
                break;
                case COBBLE:
                cout << "|    ";
                printColoredWord("Cobble", FOREGROUND_LIGHT_YELLOW);
                cout << "  - ";
                SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
                alignAndOutput(player.inventory[hotBarIndex].quantity, 3);
                SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
                cout << "  |";
                break;
                case CRAFTING_TABLE:
                cout << "|  ";
                printColoredWord("Crt_Table", FOREGROUND_LIGHT_YELLOW);
                cout << " - ";
                SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
                alignAndOutput(player.inventory[hotBarIndex].quantity,3 );
                SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
                cout << "  |";
                break;
                case DOOR:
                cout << "|    ";
                printColoredWord("Door", FOREGROUND_LIGHT_YELLOW);
                cout << "    - ";
                SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
                alignAndOutput(player.inventory[hotBarIndex].quantity, 3);
                SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
                cout << "  |";
                break;
                case WOODEN_SWORD:
                cout << "|";
                printColoredWord("Wooden_Sword", FOREGROUND_LIGHT_YELLOW);
                cout << "- ";
                SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
                alignAndOutput(player.inventory[hotBarIndex].quantity, 3);
                SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
                cout << "  |";
                break;
                default:
                cout << "|------- N/A -------|";
            }
        }
        if (i == tmp + 9)
            cout << "|___________________|";
        if (i == tmp + 10) {
            cout << "|  ";
            printColoredWord("Game Time:", FOREGROUND_WHITE);
            cout << "       |";
        }
        if (i == tmp + 11) {
            cout << "|  ";
            printColoredWord("Day:", FOREGROUND_WHITE);
            SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
            alignAndOutput(time.gameDay, 4);
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
            cout << ", ";
            SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
            alignAndOutput(time.gameHour, 2);
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
            cout << ":";
            SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
            alignAndOutput(time.gameMinute, 2);
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
            cout << "  |";
        }
        if (i == tmp + 12)
            cout << "|___________________|";

        //----------DONT TOUCH THE CODE BELOW----------
        if ((i >= layout.sightUpBnd) && (i <= layout.sightUpBnd + SIGHT_HEIGHT))
            cout << endl;
        //----------DONT TOUCH THE CODE ABOVE----------
    }
}

void System::setMap () {

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            layout.ref[i][j] = VOID_REF;
        }
    }
    //set entities to ref
    for (int i = 0; i < entity.size(); i++) {
        int x = entity[i].x;
        int y = entity[i].y;
        switch (entity[i].type) {
            case TREE_ENTITY:layout.ref[y][x] = TREE_REF;break;
            case ROCK_ENTITY:layout.ref[y][x] = ROCK_REF;break;
        }
    }
    //set blocks to ref
    for (int i = 0; i < block.size(); i++) {
        int x = block[i].x;
        int y = block[i].y;
        switch (block[i].type) {
            case PLANK:         layout.ref[y][x] = PLANK_REF;           break;
            case LOG:           layout.ref[y][x] = LOG_REF;             break;
            case COBBLE:        layout.ref[y][x] = COBBLE_REF;          break;
            case CRAFTING_TABLE:layout.ref[y][x] = CRAFTING_TABLE_REF;  break;
            case DOOR:          layout.ref[y][x] = DOOR_REF;            break;
        }
    }
    //set mobs to ref
    for (int i = 0; i < mob.size(); i++) {
        int x = mob[i].x;
        int y = mob[i].y;
        switch (mob[i].type) {
            case MOB_PIG:layout.ref[y][x] = PIG_REF;break;
        }
    }
    //set player to ref
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (i == player.y && j == player.x){
                layout.ref[i][j] = PLAYER_REF;
                break;
            }
        }
    }
}

void System::setSight () {
    if (player.x > (layout.sightLeftBnd + ( (SIGHT_WIDTH - 1) / 2) ) ) {
        if (layout.sightLeftBnd + SIGHT_WIDTH + 1 < MAP_WIDTH - 1) {
            layout.sightLeftBnd++;
        }
    }
    if (player.x < (layout.sightLeftBnd + ( (SIGHT_WIDTH - 1) / 2) ) ) {
        if (layout.sightLeftBnd > 0) {
            layout.sightLeftBnd--;
        }
    }
    if (player.y > (layout.sightUpBnd + ( (SIGHT_HEIGHT - 1) / 2) ) ) {
        if (layout.sightUpBnd + SIGHT_HEIGHT + 1 < MAP_HEIGHT - 1) {
            layout.sightUpBnd++;
        }
    }
    if (player.y < (layout.sightUpBnd + ( (SIGHT_HEIGHT - 1) / 2) ) ) {
        if (layout.sightUpBnd - 1 > 0) {
            layout.sightUpBnd--;
        }
    }
}

#define ENTITY 1
#define BLOCK 2
#define MOB 3
void System::collectDrop (int index, int form) {
    if (form == ENTITY) {
        switch (entity[index].type) {
            case TREE_ENTITY:
            if (player.inventory[LOG].quantity <= 999)
                player.inventory[LOG].quantity += entity[index].drop;
            soundEffect(BREAK_TREE_CODE);
            break;
            case ROCK_ENTITY:
            if (player.inventory[COBBLE].quantity <= 999)
                player.inventory[COBBLE].quantity += entity[index].drop;
            soundEffect(BREAK_ROCK_CODE);
            break;
        }
        entity.erase(entity.begin() + index);
    }
    else if (form == BLOCK) {
        switch (block[index].type) {
            case LOG:
            if (player.inventory[LOG].quantity <= 999)
                player.inventory[LOG].quantity ++;
            soundEffect(BREAK_WOOD_CODE);
            break;
            case PLANK:
            if (player.inventory[PLANK].quantity <= 999)
                player.inventory[PLANK].quantity ++;
            soundEffect(BREAK_WOOD_CODE);
            break;
            case COBBLE:
            if (player.inventory[COBBLE].quantity <= 999)
                player.inventory[COBBLE].quantity ++;
            soundEffect(BREAK_STONE_CODE);
            case CRAFTING_TABLE:
            if (indexOfItem(CRAFTING_TABLE) != NOT_FOUND){
                if (player.inventory[indexOfItem(CRAFTING_TABLE)].quantity <= 999)
                    player.inventory[indexOfItem(CRAFTING_TABLE)].quantity++;
            }
            else
                player.inventory.push_back(Item(CRAFTING_TABLE, 1));
            soundEffect(BREAK_WOOD_CODE);
            break;
            case DOOR:
            if (indexOfItem(DOOR) != NOT_FOUND) {
                if (player.inventory[indexOfItem(DOOR)].quantity <= 999)
                    player.inventory[indexOfItem(DOOR)].quantity++;
            }
            else
                player.inventory.push_back(Item(DOOR, 1));
            soundEffect(BREAK_WOOD_CODE);
            break;
        }
        block.erase(block.begin() + index);
    }
}

bool System::isEntity (int ref) {
    switch (ref) {
        case TREE_REF:
        case ROCK_REF:
        return true;break;
        
        default:
        return false;
    }
}

bool System::isBlock (int ref) {
    switch (ref) {
        case LOG_REF:
        case PLANK_REF:
        case COBBLE_REF:
        case CRAFTING_TABLE_REF:
        case DOOR_REF:
        return true;break;

        default:
        return false;
    }
}

void System::breakBlock () {

    bool isB_down = isBlock(layout.ref[player.y + 1][player.x]);
    bool isB_up = isBlock(layout.ref[player.y - 1][player.x]);
    bool isB_right = isBlock(layout.ref[player.y][player.x + 1]);
    bool isB_left = isBlock(layout.ref[player.y][player.x - 1]);

    for (int i = 0; i < block.size(); i++) {
        if ((block[i].x == player.x && block[i].y == player.y + 1) && isB_down)
            collectDrop(i, BLOCK);
        else if ((block[i].x == player.x && block[i].y == player.y - 1) && isB_up)
            collectDrop(i, BLOCK);
        else if ((block[i].x == player.x + 1 && block[i].y == player.y) && isB_right)
            collectDrop(i, BLOCK);
        else if ((block[i].x == player.x - 1 && block[i].y == player.y) && isB_left)
            collectDrop(i, BLOCK);
    }
}

void System::breakEntity () {

    bool isE_down = isEntity(layout.ref[player.y + 1][player.x]);
    bool isE_up = isEntity(layout.ref[player.y - 1][player.x]);
    bool isE_right = isEntity(layout.ref[player.y][player.x + 1]);
    bool isE_left = isEntity(layout.ref[player.y][player.x - 1]);

    for (int i = 0; i < entity.size(); i++) {
        if ((entity[i].x == player.x && entity[i].y == player.y + 1) && isE_down)
            collectDrop(i, ENTITY);
        else if ((entity[i].x == player.x && entity[i].y == player.y - 1) && isE_up)
            collectDrop(i, ENTITY);
        else if ((entity[i].x == player.x + 1 && entity[i].y == player.y) && isE_right)
            collectDrop(i, ENTITY);
        else if ((entity[i].x == player.x - 1 && entity[i].y == player.y) && isE_left)
            collectDrop(i, ENTITY);
    }
    checkEntities();
}

void System::triggerBnd (int direction) {
    switch (direction) {
        case UP_DIR:
        if (layout.sightUpBnd > 0)
            layout.sightUpBnd -= 1;
        else {
            soundEffect(ERROR_CODE); 
            player.y++;
        }
        break;
        case DOWN_DIR:
        if (layout.sightUpBnd < MAP_HEIGHT - SIGHT_HEIGHT)
            layout.sightUpBnd += 1;
        else {
            soundEffect(ERROR_CODE); 
            player.y--;
        }
        break;
        case LEFT_DIR:
        if (layout.sightLeftBnd > 0)
            layout.sightLeftBnd -= 1;
        else {
            soundEffect(ERROR_CODE); 
            player.x++;
        }
        break;
        case RIGHT_DIR:
        if (layout.sightLeftBnd < MAP_WIDTH - SIGHT_WIDTH)
            layout.sightLeftBnd += 1;
        else {
            soundEffect(ERROR_CODE); 
            player.x--;
        }
        break;
    }
}

void System::movePlayer (char input) {
    int intended_x;
    int intended_y;
    int direction;
    switch (input) {
        case UP:
        intended_x = player.x;
        intended_y = player.y - 1;
        direction = UP_DIR;
        break;
        case DOWN:
        intended_x = player.x;
        intended_y = player.y + 1;
        direction = DOWN_DIR;
        break;
        case LEFT:
        intended_x = player.x - 1;
        intended_y = player.y;
        direction = LEFT_DIR;
        break;
        case RIGHT:
        intended_x = player.x + 1;
        intended_y = player.y;
        direction = RIGHT_DIR;
        break;
    }

    if (layout.ref[intended_y][intended_x] == VOID_REF || layout.ref[intended_y][intended_x] == DOOR_REF) {
        soundEffect(WALK_CODE);
        player.x = intended_x;
        player.y = intended_y;
    }
    else {
        soundEffect(ERROR_CODE);
    }

    if (intended_x == layout.sightLeftBnd || intended_x == layout.sightLeftBnd + SIGHT_WIDTH - 1 || intended_y == layout.sightUpBnd || intended_y == layout.sightUpBnd + SIGHT_HEIGHT - 1) {
        triggerBnd(direction);
    }

}

void System::interact (char input) {
    return;
}

void System::openInventory () {
    gotoXY(2, 2);
    cout << "____________________________________________";
    gotoXY(2, 3);
    cout << "|  Index  |     Type     | Amount |  Block  |";
    gotoXY(2, 4);
    cout << "|_________|______________|________|_________|";

    for (int i = 0; i < player.inventory.size(); i++) {
        gotoXY(2, 5 + 2 * i);
        cout << "|  -";
        SetConsoleTextAttribute(console, FOREGROUND_WHITE);
        alignAndOutput(i + 1, 3);
        SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
        cout << "-  |   ";
        switch (player.inventory[i].type) {
            case LOG:printColoredWord("  LOG    ", FOREGROUND_WHITE);break;
            case PLANK:printColoredWord("  PLANK  ", FOREGROUND_WHITE);break;
            case COBBLE:printColoredWord(" COBBLE  ", FOREGROUND_WHITE);break;
            case CRAFTING_TABLE:printColoredWord("CRT_TABLE", FOREGROUND_WHITE);break;
            case DOOR:printColoredWord("  DOOR   ", FOREGROUND_WHITE);break;
            case WOODEN_SWORD:printColoredWord("WDEN_SWRD", FOREGROUND_WHITE);break;
        }
        cout << "  |  -";
        SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
        alignAndOutput(player.inventory[i].quantity, 3);
        SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
        cout << "- |   ";
        SetConsoleTextAttribute(console, FOREGROUND_LIGHT_YELLOW);
        switch (player.inventory[i].type) {
            case LOG:           cout << "\'" << LOG_BLOCK << "\'";break;
            case PLANK:         cout << "\'" << PLANK_BLOCK << "\'";break;
            case COBBLE:        cout << "\'" << COBBLE_BLOCK << "\'";break;
            case CRAFTING_TABLE:cout << "\'" << CRAFTING_TABLE_BLOCK << "\'";break;
            case DOOR:          cout << "\'" << DOOR_BLOCK << "\'";break;
            default:            cout << "N/A";
        }
        SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
        cout << "   |";
        gotoXY(2, 6 + 2 * i);
        cout << "|_________|______________|________|_________|";
    }
    cout << endl;
}

void System::setBlock (int item, int x, int y) {
    soundEffect(BREAK_WOOD_CODE);
    block.push_back(Block(item, x, y));
    player.inventory[hotBarIndex].quantity--;
}

void System::placeBlock (char input) {
    if (player.inventory[hotBarIndex].quantity > 0) {
        switch (input) {
            case PLACE_UP:
            if (layout.ref[player.y - 1][player.x] == VOID_REF)
                setBlock(player.inventory[hotBarIndex].type, player.x, player.y - 1);
            break;
            case PLACE_DOWN:
            if (layout.ref[player.y + 1][player.x] == VOID_REF) 
                setBlock(player.inventory[hotBarIndex].type, player.x, player.y + 1);
            break;
            case PLACE_RIGHT:
            if (layout.ref[player.y][player.x + 1] == VOID_REF)
                setBlock(player.inventory[hotBarIndex].type, player.x + 1, player.y);
            break;
            case PLACE_LEFT:
            if (layout.ref[player.y][player.x - 1] == VOID_REF) 
                setBlock(player.inventory[hotBarIndex].type, player.x - 1, player.y);
            break;
        }
    }
    refreshInventory();
}

void System::changeHotBar () {
    openInventory();
    while (!kbhit());
    char input = getch();
    int index = input - '0';
    hotBarIndex = index - 1;
}

int System::indexOfItem (int item_type) {

    for (int i = 0; i < player.inventory.size(); i++) {
        if (player.inventory[i].type == item_type) {
            return i;
        }
    }
    return NOT_FOUND;
}

bool System::isBlockNear (int blockRef) {
    bool isBlockNear = false;

    if (layout.ref[player.y + 1][player.x] == blockRef)
        isBlockNear = true;
    if (layout.ref[player.y - 1][player.x] == blockRef)
        isBlockNear = true;
    if (layout.ref[player.y][player.x + 1] == blockRef)
        isBlockNear = true;
    if (layout.ref[player.y][player.x - 1] == blockRef)
        isBlockNear = true;

    return isBlockNear;
}

void System::craft () {
    gotoXY(2, 2);
    cout << "____________________________________________";
    gotoXY(2, 3);
    cout << "|  Index  |     Type     |      Recipe      |";
    gotoXY(2, 4);
    cout << "|_________|______________|__________________|";
    gotoXY(2, 5);
    cout << "|  -";
    printColoredWord("001", FOREGROUND_WHITE);
    cout << "-  |    ";
    printColoredWord("Planks", FOREGROUND_WHITE);
    cout << "    |";
    printColoredWord("1", FOREGROUND_LIGHT_YELLOW);
    printColoredWord(" Log ", FOREGROUND_WHITE);
    cout << "-> ";
    printColoredWord("4", FOREGROUND_LIGHT_YELLOW);
    printColoredWord(" Planks ", FOREGROUND_WHITE);
    cout << "|";
    gotoXY(2, 6);
    cout << "|_________|______________|__________________|";
    gotoXY(2, 7);
    cout << "|  -";
    printColoredWord("002", FOREGROUND_WHITE);
    cout << "-  |";
    printColoredWord("Crafting_Table", FOREGROUND_WHITE);
    cout << "| ";
    printColoredWord("4", FOREGROUND_LIGHT_YELLOW);
    printColoredWord(" Planks ", FOREGROUND_WHITE);
    cout << "-> ";
    printColoredWord("1", FOREGROUND_LIGHT_YELLOW);
    printColoredWord(" CT ", FOREGROUND_WHITE);
    cout << "|";
    gotoXY(2, 8);
    cout << "|_________|______________|__________________|";
    int input;
    if (isBlockNear(CRAFTING_TABLE_REF)) {
        gotoXY(2, 9);
        cout << "|  -";
        printColoredWord("003", FOREGROUND_WHITE);
        cout << "-  | ";
        printColoredWord("Wooden_Sword", FOREGROUND_WHITE);
        cout << " |";
        printColoredWord("12", FOREGROUND_LIGHT_YELLOW);
        printColoredWord(" Planks", FOREGROUND_WHITE);
        cout << "->";
        printColoredWord("1", FOREGROUND_LIGHT_YELLOW);
        printColoredWord(" Sword", FOREGROUND_WHITE);
        cout << "|";
        gotoXY(2, 10);
        cout << "|_________|______________|__________________|";
        gotoXY(2,11);
        cout << "|  -";
        printColoredWord("004", FOREGROUND_WHITE);
        cout << "-  |     ";
        printColoredWord("Door", FOREGROUND_WHITE);
        cout << "     |";
        printColoredWord("6", FOREGROUND_LIGHT_YELLOW);
        printColoredWord(" Planks ", FOREGROUND_WHITE);
        cout << "-> ";
        printColoredWord("1", FOREGROUND_LIGHT_YELLOW);
        printColoredWord(" Door", FOREGROUND_WHITE);
        cout << "|";
        gotoXY(2, 12);
        cout << "|_________|______________|__________________|";
        gotoXY(2, 13);
        cout << "Act(0 to exit):";
        while (!kbhit());
        char in = getch();
        input = in - '0';
    }
    else {
        gotoXY(2, 9);
        cout << "Act(0 to exit):";
        while (!kbhit());
        char in = getch();
        input = in - '0';
    }
    
    if (input == 0)
        return;
    switch (input) {
        case 1:
        if (player.inventory[LOG].quantity >= 1) {
            soundEffect(PUSH_BUTTON_CODE);
            player.inventory[LOG].quantity--;
            player.inventory[PLANK].quantity += 4;
        }
        break;
        case 2:
        if (player.inventory[PLANK].quantity >= 4) {
            soundEffect(PUSH_BUTTON_CODE);
            player.inventory[PLANK].quantity -= 4;
            if (indexOfItem(CRAFTING_TABLE) != NOT_FOUND) {
                player.inventory[indexOfItem(CRAFTING_TABLE)].quantity++; 
            }
            else {
                player.inventory.push_back(Item(CRAFTING_TABLE, 1));
            }
        }
        break;
        case 3:
        if (isBlockNear(CRAFTING_TABLE_REF)) {
            if (player.inventory[PLANK].quantity >= 12) {
                soundEffect(PUSH_BUTTON_CODE);
                player.inventory[PLANK].quantity -= 12;
                if (indexOfItem(WOODEN_SWORD) != NOT_FOUND)
                    player.inventory[indexOfItem(WOODEN_SWORD)].quantity++;
                else
                    player.inventory.push_back(Item(WOODEN_SWORD, 1));
            }
        }
        else 
            cout << "You need a Crafting Table near you to craft extra stuff." << endl;
        break;
        case 4:
        if (isBlockNear(CRAFTING_TABLE_REF)) {
            if (player.inventory[PLANK].quantity >= 6) {
                soundEffect(PUSH_BUTTON_CODE);
                player.inventory[PLANK].quantity -= 6;
                if (indexOfItem(DOOR) != NOT_FOUND)
                    player.inventory[indexOfItem(DOOR)].quantity++;
                else
                    player.inventory.push_back(Item(DOOR, 1));
            }
        }
        else 
            cout << "You need a Crafting Table near you to craft extra stuff." << endl;
        break;
    }

    


    return;
}

void System::refreshInventory () {
    for (int i = 3; i < player.inventory.size(); i++) {
        if (player.inventory[i].quantity <= 0)
            player.inventory.erase(player.inventory.begin() + i);
    }
}