#include "mygame.h"


// These are just some menues nothing too important is going on here


void Tutorial() {
    erase();
    mvprintw(0, 0, "USE THE ARROW KEYS TO MOVE LEFT AND RIGHT");
    mvprintw(2, 0, "USE THE UP ARROW TO SHOOT");
    mvprintw(4, 0, "PRESS q TO QUIT, p TO PAUSE, AND b TO CLOSE TUTORIAL");
    mvprintw(6, 0, "IF THE ENEMY SHIPS REACH YOUR LOCATION THE GAME ENDS");
    mvprintw(8, 0, "HOLD THEM BACK AND HAVE FUN");
    char g = getch();
    while(g != 'b') {
        if(g == 'q') { endwin(); exit(0); }
        g = getch();
    }
}

void Difficulty_Menu() {
    erase();
    char *select = "SELECT A DIFFICULTY";
    char *nums = "1      2      3        4     ";
    char *options = "EASY  MEDIUM  HARD  IMPOSSIBLE";
    mvprintw(LINES / 2 - 4 , (COLS - strlen(select)) / 2, "%s", select);
    mvprintw(LINES / 2 - 2 , (COLS - strlen(nums)) / 2, "%s", nums);
    mvprintw(LINES / 2 , (COLS - strlen(options)) / 2, "%s", options);
    refresh();
    while(1) {
        char g = getch();
        if(g == 'q') { endwin(); exit(0); }
        else if(g == '1') { speed = 1500; difficulty = 10; max_speed = 1000; return; }
        else if(g == '2' || g == '\n') { speed = 1400; difficulty = 20; max_speed = 900; return; }
        else if(g == '3') { speed = 1300; difficulty = 25; max_speed = 800; return; }
        else if(g == '4') { 
            speed = 900; 
            difficulty = 40;
            max_speed = 500; 
            ADD_FRONT(enemies);
            ADD_FRONT(enemies);  
            return; 
        }
        erase();
        mvprintw(LINES / 2 - 4 , (COLS - strlen(select)) / 2, "%s", select);
        mvprintw(LINES / 2 - 2 , (COLS - strlen(nums)) / 2, "%s", nums);
        mvprintw(LINES / 2 , (COLS - strlen(options)) / 2, "%s", options);
        refresh();
    }
}

void Ship_Select() {
    erase();
    FILE *fp = fopen("Highscore", "rb");
    fseek(fp, 4, 0);
    fread(&ship_options, 4, 2, fp);

    char *select = "SELECT A SHIP";
    char *nums = " 1       2       3 ";
    char *options = "|.|     _|_     \\|/";
    char available[30];

    strcpy(available, "unlocked  ");
    if(ship_options.fast) strcat(available, "unlocked  ");
    else strcat(available, "locked  ");
    if(ship_options.cool) strcat(available, "unlocked");
    else strcat(available, "locked");

    mvprintw(LINES / 2 - 4, (COLS - strlen(select)) / 2, "%s\n", select);
    mvprintw(LINES / 2 - 2, (COLS - strlen(nums)) / 2, "%s\n", nums);
    mvprintw(LINES / 2, (COLS - strlen(options)) / 2, "%s", options);
    mvprintw(LINES / 2 + 2, (COLS - strlen(available)) / 2, "%s", available);
    refresh();
    while(1) {
        char g = getch();
        if(g == 'q') { endwin(); exit(0); }
        else if(g == '1' || g == '\n') {
            strcpy(drawing, "|.|");
            strcpy(Bull_Drawing, "' '");
            return;
        } else if(g == '2' && ship_options.fast) {   
            strcpy(drawing, "_|_");
            strcpy(Bull_Drawing, " * ");
            return;
        } else if(g == '3' && ship_options.cool) {
            strcpy(drawing, "\\|/");
            strcpy(Bull_Drawing, "'''");
            return;
        }
        erase();
        mvprintw(LINES / 2 - 4, (COLS - strlen(select)) / 2, "%s\n", select);
        mvprintw(LINES / 2 - 2, (COLS - strlen(nums)) / 2, "%s\n", nums);
        mvprintw(LINES / 2, (COLS - strlen(options)) / 2, "%s", options);
        mvprintw(LINES / 2 + 2, (COLS - strlen(available)) / 2, "%s", available);
        refresh();
    }
}

void Pause() {
    char *msg = "  PRESS SPACE OR ENTER TO RESUME  ";
    mvprintw(LINES / 2, (COLS - strlen(msg)) / 2, "%s", msg);
    refresh();
    while(1) {
        char g = getch();
        if(g == ' ' || g == '\n') return;
        else if(g == 'q') { endwin(); exit(0); }
    }
}