#include "mygame.h"

llist *enemies;
unlocked ship_options;

unsigned int score = 0;
unsigned int ECOLS = 0;
unsigned int Highscore;
unsigned int speed;
unsigned int max_speed;
unsigned int difficulty;
char drawing[4];
char Bull_Drawing[4];

void Init_Game() {
    SRAND;
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    FILE *HS = fopen("Highscore", "rb");
    fread(&Highscore, 4, 1, HS);
    fclose(HS);
    attron(A_BOLD | A_REVERSE);
    char s[20];
    char *start = " PRESS ENTER TO START "; 
    sprintf(s, " HIGH SCORE: %d ", Highscore);
    mvprintw(LINES/2-LINES/10, (COLS - strlen(s)) / 2, "%s", s);  
    attroff(A_BOLD | A_REVERSE);
    mvprintw(LINES / 2, (COLS - strlen(start)) / 2, "%s", start); 
    refresh();
    int switt = 0;
    char g = getch();
    while(g != '\n' && g != ' ') {
        erase();
        attron(A_BOLD | A_REVERSE);
        mvprintw(LINES/2-LINES/10, (COLS - strlen(s)) / 2, "%s", s);  
        attroff(A_BOLD | A_REVERSE);
        mvprintw(0, 0, "Press t to view tutorial");           
        if(switt) mvprintw((LINES+LINES/4) / 2, (COLS - strlen(start)) / 2, "%s", start);
        else mvprintw(LINES / 2, (COLS - strlen(start)) / 2, " ");
        refresh();
        switt = !switt;
        usleep(800000);
        g = getch();
        if(g == 'q') { endwin(); exit(0); }
        else if(g == 't') Tutorial();
    }
    Difficulty_Menu();
    Ship_Select();
}

void End_Game() {
    clear();
    attron(A_BOLD | A_REVERSE);
    if(score < Highscore) {
        char *msg = "  GAME OVER  ";
        char smsg[20];
        sprintf(smsg, "Score: %d", score);
        mvprintw(LINES / 2, (COLS - strlen(msg)) / 2, "%s", msg);
        attroff(A_BOLD | A_REVERSE);
        mvprintw(0, COLS-strlen(smsg), "%s", smsg);
        refresh();
    } else if(score >= 500 && !(ship_options.fast)) {
        char msg[20];
        sprintf(msg, " NEW HIGH SCORE: %d ", score);
        char *new = "NEW SHIP UNLOCKED";
        char *new_ship = "_|_";
        mvprintw(LINES / 2 - 6, (COLS - strlen(msg)) / 2, "%s", msg); 
        attroff(A_BOLD | A_REVERSE);
        mvprintw(LINES / 2 - 2, (COLS - strlen(new)) / 2, "%s", new); 
        mvprintw(LINES / 2 + 2, (COLS - strlen(new_ship)) / 2, "%s", new_ship); 
        refresh();
        FILE *HS = fopen("Highscore", "wb");
        fwrite(&score, 4, 1, HS);
        fseek(HS, 4, 0);
        int fast = 1;
        fwrite(&fast, 4, 1, HS);
        fclose(HS);
        getch();
    } else if(score >= 1000 && !(ship_options.cool)) {
        char msg[20];
        sprintf(msg, " NEW HIGH SCORE: %d ", score);
        char *new = "NEW SHIP UNLOCKED";
        char *new_ship = "\\|/";
        mvprintw(LINES / 2 - 6, (COLS - strlen(msg)) / 2, "%s", msg); 
        attroff(A_BOLD | A_REVERSE);
        mvprintw(LINES / 2 - 2, (COLS - strlen(new)) / 2, "%s", new); 
        mvprintw(LINES / 2 + 2, (COLS - strlen(new_ship)) / 2, "%s", new_ship); 
        refresh();
        FILE *HS = fopen("Highscore", "wb");
        fwrite(&score, 4, 1, HS);
        fseek(HS, 4, 0);
        fwrite(&(ship_options.fast), 4, 1, HS);
        fseek(HS, 8, 0);
        int cool = 1;
        fwrite(&cool, 4, 1, HS);
        fclose(HS);
        getch();
    } else {
        char msg[20];
        sprintf(msg, " NEW HIGH SCORE: %d ", score);
        mvprintw(LINES / 2, (COLS - strlen(msg)) / 2, "%s", msg); 
        refresh();
        FILE *HS = fopen("Highscore", "wb");
        fwrite(&score, 4, 1, HS);
        fseek(HS, 4, 0);
        fwrite(&(ship_options.fast), 4, 1, HS);
        fseek(HS, 8, 0);
        fwrite(&(ship_options.cool), 4, 1, HS);
        fclose(HS);
        attroff(A_BOLD | A_REVERSE);
    }
    sleep(3);
    endwin();
    execl("./space", "c", NULL);
}

int update_and_draw(ship *s, int tick) {
    int destroyed = 0;
    Node_t *rover = enemies->Head;
    FORI(enemies->size) {
        int *curr = (int*)rover->data;
        destroyed = 0;
        FORJ(ECOLS) {
            if(!i && !(tick % speed) && RAND(0, 4) == 0) curr[j] = DESTROYED;
            if(s->bull.size > 0) {
                Node_t *rover2 = s->bull.Head;
                FORK(s->bull.size) {
                    bullet *curr_b = (bullet*)rover2->data;
                    
                    if(curr[j] != DESTROYED && curr_b->y == i*2 && curr_b->x == j*SPACING) { 
                        curr[j] = DESTROYED;
                        REMOVE(&s->bull, k);
                        score++;
                        mvprintw(i*2, j*SPACING, " 1 ");
                        break;
                    } else if(s->y+1 == i*2 && s->x == i*SPACING) return 1;

                    if(i == enemies->size-1 && j == ECOLS-SPACING) {
                        mvprintw(curr_b->y, curr_b->x, "%s", Bull_Drawing);
                        // Move bullet every few ticks so it's not too fast
                        if (tick % 75 == 0) curr_b->y--; 
                        if (curr_b->y < 0) { REMOVE(&s->bull, k); break; }
                    }
                    rover2 = rover2->next;
                }
            }
            
            if(curr[j] != DESTROYED) mvprintw(i*2, j*SPACING, "'|'");
            else destroyed++;
        }
        if(destroyed == ECOLS && i == enemies->size-1) { REMOVE(enemies, i); break; }
        rover = rover->next;
    }
    // Draw Ship 
    mvprintw(s->y, s->x, "%s", drawing);
    char msg[20];
    sprintf(msg, "Score: %d", score);
    mvprintw(0, COLS-strlen(msg), "%s", msg);
    refresh();
    return 0;
}

int main() {

    // initialize game
    enemies = LLIST(1);
    Init_Game();
        
    // set buffer
    int BUFF;
    if(LINES < 10*2) BUFF = 4;
    else if(LINES < 15*2) BUFF = 6;
    else BUFF = 8;
    int PREV = LINES;

    // initialize ship and tick
    ship character = {0, LINES-BUFF, {NULL, 0, NULL}};
    int tick = 0;
    int last_inc = 0;

    while (1) {

        // resizing
        if(WECOLS < 8) ECOLS = 8;
        else if(WECOLS > 10) ECOLS = 10;
        else ECOLS = WECOLS;
        erase();
        if(update_and_draw(&character, tick)) break;

        // handle inputs
        int g = getch();
        if(g == 'q') { endwin(); exit(0); }
        else if(g == 'p') Pause();
        else if (g == KEY_LEFT && character.x > 0) character.x -= MOVE_SPEED;
        else if (g == KEY_RIGHT && character.x < (ECOLS-1)*SPACING) character.x += MOVE_SPEED;
        else if (g == KEY_UP) APPEND_BULL(&character.bull, character.x, character.y-1);

        // consistent timing 
        usleep(1000); 
        tick++;

        // resizing features
        if(PREV != LINES) {
            character.y = LINES-BUFF;
            if(LINES < 12*2) BUFF = 4;
            else if(LINES < 15*2) BUFF = 6;
            else BUFF = 8;
            if(LINES < 12*2 && PREV >=12*2) { PREV = LINES; if(enemies->Head) REMOVE(enemies, 0); }
            else if(LINES < 10*2 && PREV >=10*2) { PREV = LINES; if(enemies->Head) REMOVE(enemies, 0); }
            if(LINES >= 10*2 && PREV < 10*2) { PREV = LINES; ADD_FRONT(enemies); }
            else if(LINES >= 12*2 && PREV < 12*2) { PREV = LINES; ADD_FRONT(enemies);  }
        }

        // handles speedup, adding rows, and game over
        if(enemies->size*2-2 >= LINES-BUFF) break;
        if(score != last_inc && !(score % 25) && speed > max_speed) { speed -= difficulty; last_inc = score; }
        if(!(tick % speed)) ADD_FRONT(enemies); 
    }

    // cleanup 
    DESTRUCT(enemies);
    End_Game();
}