#include <cstdlib>
#include <Windows.h>

#define WALK 180
#define BREAK_TREE 250
#define BREAK_ROCK 400
#define PUSH_BUTTON 700
#define ERROR_SOUND 200
#define BREAK_WOOD 250
#define BREAK_STONE 400
#define DURATION 80

#define WALK_CODE 1
#define BREAK_TREE_CODE 2
#define BREAK_ROCK_CODE 3
#define PUSH_BUTTON_CODE 4
#define ERROR_CODE 5
#define BREAK_WOOD_CODE 6
#define BREAK_STONE_CODE 7

void playSong () {
    Beep(329,150); //E
    Beep(493,150); //B
    Beep(698,150); //F^
    Beep(659,300); //E^

    Beep(783,150); //G^
    Beep(698,150); //F^
    Beep(659,300); //E^

    Beep(329,100); 
    Beep(493,150); 
    Beep(698,150); 
    Beep(659,300);

    Beep(392,125); 
    Beep(440,100); 
    Beep(587,150); 

    Beep(349,125);
    Beep(587,250); 

    Beep(329,150); 
    Beep(493,150); 
    Beep(698,150); 
    Beep(659,300); 

    Beep(783,150); 
    Beep(698,150); 
    Beep(659,300); 

    Beep(329,100); 
    Beep(493,150); 
    Beep(698,150); 
    Beep(659,300);

    Beep(392,125); 
    Beep(440,100); 
    Beep(587,150); 

    Beep(349,125);
    Beep(587,200); 

}

void soundEffect (int condition) {
    switch (condition) {
        case WALK_CODE:
        Beep(WALK, DURATION);
        Beep(WALK, DURATION);break;
        case BREAK_TREE_CODE:
        Beep(BREAK_TREE, DURATION);
        Beep(BREAK_TREE + 50, DURATION / 3);
        Beep(BREAK_TREE + 100, DURATION / 3);
        Beep(BREAK_TREE + 150, DURATION / 3);break;
        case BREAK_ROCK_CODE:
        Beep(BREAK_ROCK, DURATION);
        Beep(BREAK_ROCK + 50, DURATION / 3);
        Beep(BREAK_ROCK + 100, DURATION / 3);
        Beep(BREAK_ROCK + 150, DURATION / 3);break;
        case PUSH_BUTTON_CODE:Beep(PUSH_BUTTON, DURATION);break;
        case ERROR_CODE:Beep(ERROR_SOUND, DURATION * 2);break;
        case BREAK_WOOD_CODE:
        Beep(BREAK_WOOD, DURATION);break;
        case BREAK_STONE_CODE:
        Beep(BREAK_STONE, DURATION);break;
    }
}