OVERVIEW:

    This is an arcade style space shooter game that is in my opintion quite fun. I also had
    a good time making it. It has different difficulty levels and unlockable ships. The game 
    itslelf is made using the Ncurses library meaning it is in the terminal. 


GAME DESIGN:

    ENTITIES:

    I have two "entities", there is a player ship and there are enemy ships.

    1.  The player ship struct that has x and y coordinates along with a pointer to a linked list of bullets.

    2.  The enemy ships do not have a struct. because of how the game is structured. The ships are arranged by rows.
        There is a linked list of rows that grows over time and shrinks if the row closest to the player is fully 
        destroyed. Each row is an array of ints (could be bools in current state but are ints in case of future updates
        where I could add ship health). if the int value is -1 the ship has been destroyed otherwise it is alive. 
        This will make sense later on.

    3. you could technically count the bullets as entities I guess. The bullet struct is just an x and y coord. 


    GAME LOOP:

    The game loop does these things in this order.

    1.  Checks for terminal resizing and adjust things accordingly.
    
    2.  Calls the update_and_draw function and breaks the loop (ends game) if the function returns 1.
    
    3.  Handles inputs.

    4.  usleeps to keep a consistant timing.

    5.  Handles some more resizing features.

    6.  Handles speedup, adding rows, and game over


    UPDATE AND DRAW FUNCTION:

    This funcion is called once every time throught the game loop. and hadles the collision checking as well
    as the drawing to screen. I does the following things in order.

    1.  There is a nested for loop where the outer loops throught the linked list of rows and the inner loops 
        through the rows themselves (int arrays). The position of the ships is based off of the indexing of the loops
        combined with a spacing macro. This makes a sort of "grid" of ships.

    2.  The first time a row is added some of the ships are marked deleted by a random funcion.

    3.  Collision is checked for every position in the "grid". To do this the bullet linked list is looped through.
        If a collision happens the appropriate bullet is removed and the ship is marked deleted. At the same time all
        the bullets are printed to the screen.

    4.  After collision is checked each ship is printed if it is not destroyed. 

    The collision checking and the printing happen all in a one loop (FORJ for ships and FORK for bullets) can slightly
    improve performace by not needing to loop through the same things twice. It's not all sunshine and rainbows through
    because having a triple nested for loop makes the funcion a bit confusing and harder to read.

    
    SAVE FILES: 

    There is one save file called Highscore. The first 4 bytes of the file store the highscore (an int). The next 8 bytes 
    store a struct (two ints) that correspondes to wether or not the two unlockable ships are unlocked. since the struct is 
    just two ints and not like 3 chars or something (with 3 chars the struct size would likely be 4 bytes instead of 3 and could
    cause some padding issues when trying to read in raw bytes from a file). There should theoretically not be any issues with padding 
    as the struct should be consistantly 8 bytes with this configuration. 


    MENUS: 

    I have a couple of different menus that generally all have a dedicated funcion such as 
    Tutorial, Difficuly menu, ship select, ect.


ADDITIONAL NOTES:

1.  The game requires you to have the Ncurses library installed. sudo apt update && sudo apt install libncurses-dev libncursesw-dev
    if on Ubuntu.

2.  If the Highscore file doesn't exist the game will seg fault upon launch so please make sure that if you take the code 
    from this repo and try to run it that you also take the Highscore file aswell.