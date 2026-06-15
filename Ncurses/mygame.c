#include "mygame.h"

// finds whatever index you are looking for if it exists (will seg fault if index is not in list)
void *ACCESS(llist *List, int index) { 
    Node_t *rover = List->Head; 
    FORZ(index) { rover = rover->next; } 
    return (rover->data); 
}

// adds directely to the front of the list O(1)
void ADD_FRONT(llist *L) { 
    Node_t *Node = calloc(1,sizeof(Node_t)); 
    Node->next = NULL; 
    int *data = calloc(25,sizeof(int)); 
    Node->data = data; 
    if(L->Head IS NULL) { 
        L->Head = Node; 
        L->Tail = Node; 
    } else { 
        Node->next = L->Head;
        L->Head = Node; 
    } 
    L->size++; 
}

// adds directely to the end of list O(1)
void APPEND_BULL(llist *L, int x, int y) { 
    if(L->size > 8) return;
    Node_t *Node = calloc(1,sizeof(Node_t)); 
    Node->next = NULL; 
    bullet *data = calloc(1,sizeof(bullet));
    if(!data) exit(0);
    data->x = x;
    data->y = y; 
    Node->data = data; 
    if(L->Head IS NULL) { 
        L->Head = Node; 
        L->Tail = Node; 
    } else { 
        L->Tail->next = Node; 
        L->Tail = Node; 
    } 
    L->size++; 
}

// removes specified index O(n)
 void REMOVE(llist *L, int index) { 
    if(L->Head IS NULL) return;
    Node_t *rover = L->Head; 
    Node_t *prev = NULL; 
    FORZ(index) { 
        if(rover->next IS NULL) break; 
        prev = rover; 
        rover = rover->next; 
    } 
    if(prev IS NULL) { 
        // Removing the Head
        L->Head = rover->next; 
        // If the list is now empty, Tail must be NULL too
        if (L->Head IS NULL) L->Tail = NULL; 
    } else { 
        // Removing Middle or Tail
        prev->next = rover->next; 
        // If just removed the Tail, update L->Tail to the previous node
        if (rover IS L->Tail) L->Tail = prev; 
    } 
    if(rover IS_NOT NULL) { 
        L->size--; 
        free(rover->data); 
        free(rover); 
    } 
}

// initializes list and adds specified num of items to it
llist *LLIST(int num) { 
    llist *LIST = LIST_INIT(); 
    FORZ(num) { 
        ADD_FRONT(LIST); 
    } 
    return LIST; 
}

// destroys the list
void DESTRUCT(llist *L) { 
    Node_t *rover = L->Head;
    Node_t *next_node;
    while(rover IS_NOT NULL) { 
        next_node = rover->next; // Save next before freeing current
        free(rover->data); 
        free(rover); 
        rover = next_node;
    } 
    free(L); 
}