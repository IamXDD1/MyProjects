#include "classes.h"
#include "functions.h"
#include <vector>

using namespace std;

System game;
bool isGameRunning = true;

void setGame (bool startNewGame) {
    game.setTime(!startNewGame);
    game.setPlayer(!startNewGame);
    game.checkEntities();
    game.setMap();
    game.printLayout();
}

bool endGame () {
    printMessage(END_GAME);
    char decision;
    cin >> decision;
    if (decision == 'y')
        return true;
    else
        return false;
}

void updateGame () {
    game.checkEntities();
    game.setMap();
}

void controlFlow () {
    game.updateTime();
    game.checkMobs();
    game.updateMobs();
    char movement = getch();

    switch (movement) {
        case UP:
        case DOWN:
        case RIGHT:
        case LEFT:
        game.movePlayer(movement);
        game.setSight();
        updateGame();
        break;
        case BREAK:
        game.breakEntity();
        game.breakBlock();
        updateGame();
        break;
        case USE:
        game.interact(movement);
        break;
        case CHECK_INVENTORY:
        game.openInventory();
        cout << "# ";
        printMessage(PRESS_ANY_KEY);
        while(!kbhit());
        getch();
        break;
        case PLACE_UP:
        case PLACE_DOWN:
        case PLACE_LEFT:
        case PLACE_RIGHT:
        game.placeBlock(movement);
        updateGame();
        break;
        case CHANGE_HOT_BAR:
        game.changeHotBar();
        break;
        case CRAFT:
        game.craft();
        break;
        case END:
        isGameRunning = !endGame();
        break;
    }
}

void saveGame () {
    //saves coordinate
    ofstream outFile_pos("saves/coordinate.txt");
    outFile_pos << game.getPlayer().x << " " << game.getPlayer().y;
    outFile_pos.close();

    //saves inventory
    ofstream outFile_inv("saves/inventory.txt");
    for (int i = 0; i < game.getPlayer().inventory.size(); i++)
        outFile_inv << game.getPlayer().inventory[i].type << " " << game.getPlayer().inventory[i].quantity << endl;
    outFile_inv << "-1";
    outFile_inv.close();

    ofstream outFile_sightBorder("saves/sightBorder.txt");
    outFile_sightBorder << game.getLayout().sightLeftBnd << " " << game.getLayout().sightUpBnd;
    outFile_sightBorder.close(); 

    ofstream outFile_health("saves/health.txt");
    outFile_health << game.getPlayer().health;
    outFile_health.close();

    ofstream outFile_block("saves/block.txt");
    for (int i = 0; i < game.getBlock().size(); i++) {
        outFile_block << game.getBlock()[i].type << " " << game.getBlock()[i].x << " " << game.getBlock()[i].y << "\n";
    }
    outFile_block << "-1";
    outFile_block.close();
}

void endGame (System game) {
    saveGame();
    printMessage(THANKS_FOR_PLAYING);
}

int main () {

    srand(time(nullptr));

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;
    GetConsoleCursorInfo(console, &cursorInfo);

    bool startNewGame = false;
    bool start = false;

    loadArguments();

    while (!start) {
        printMessage(INTRO_OPTIONS);
        playSong();
        printMessage(MOVEMENT);
        int input;
        while (!kbhit());
        char in = getch();
        input = in - '0';
        soundEffect(PUSH_BUTTON_CODE);
        switch (input) {
        case OPTION1:
            system("CLS");
            printMessage(HOW_TO_PLAY);
            printMessage(PRESS_ANY_KEY);
            while (!kbhit()) {}
            getch();
            system("CLS");
            break;
            case OPTION2:
            startNewGame = true;
            start = true;
            break;
            case OPTION3:
            start = true;
            break;
        }
        soundEffect(PUSH_BUTTON_CODE);
    }
    setGame(startNewGame);
    
    while (isGameRunning) {
        while (!kbhit()) {
            //no movement update
            game.updateTime();
            game.checkMobs();
            game.updateMobs();
            game.printLayout();
        }
        controlFlow();
        game.checkEntities();
        if(isGameRunning)
            game.printLayout();
        saveGame();
    }

    endGame(game);
    return 0;
}