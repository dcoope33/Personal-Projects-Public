#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h> 
#include <stdio.h>

#define DESTROYED -1
#define SPACING 6
#define WECOLS COLS/12
#define MOVE_SPEED 6
#define EROWS 12

#define IS_NOT !=
#define IS ==
#define AND &&
#define OR ||

#define FORI(STOP) for(int i = 0; i<(STOP); i++)
#define FORJ(STOP) for(int j = 0; j<(STOP); j++)
#define FORK(STOP) for(int k = 0; k<(STOP); k++)
#define FORZ(STOP) for(int z = 0; z<(STOP); z++)

#define SRAND srand(time(NULL))
#define RAND(MIN, MAX) ((rand() % (MAX-MIN+1)) + MIN)

struct node {
    struct node *next;
    void *data;
};
typedef struct node Node_t;

struct List {
    Node_t *Head;
    int size;
    Node_t *Tail;
};
typedef struct List llist;

typedef struct {
    int x, y;
} bullet;

typedef struct {
    int x, y;
    llist bull;
} ship;

typedef struct {
    int fast; 
    int cool;
} unlocked;

extern llist *enemies;
extern unlocked ship_options;

extern unsigned int score;
extern unsigned int ECOLS;
extern unsigned int Highscore;
extern unsigned int speed;
extern unsigned int max_speed;
extern unsigned int difficulty;
extern char drawing[4];
extern char Bull_Drawing[4];

#define LIST_INIT() (llist*)calloc(1,sizeof(llist))

void *ACCESS(llist *List, int index);
void ADD_FRONT(llist *L);
void APPEND_BULL(llist *L, int x, int y);
void REMOVE(llist *L, int index);
llist *LLIST(int num);
void DESTRUCT(llist *L);

void Tutorial();
void Difficulty_Menu();
void Ship_Select();
void Pause();