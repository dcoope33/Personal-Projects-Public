DISCLAIMER:

   This game IS NOT FINISHED YET and I am in the process of working on level 4.

   Most of the assets "art" in this game were made with the help of generative AI this is because 
   I am an engineer not an artist.


OVERVIEW:   
   
    Shool Game (Escape The Classroom) is a game that I have had a lot of fun developing. The basic idea of this game 
is that it is a game where you are a student that doesn't like school I suppose, and due to your dislike of school 
you want to escape. There are 4 levels and in each level you start in a classroom and you must leave the room and 
navigate the hallways in order to escape the school. Each level has increasing difficulty and in the final level 
you must defeat the principle in a boss fight. One of the main reasons for developing this game in C++ specifically
is that as a computer engineering major I write a lot of lower level C code and I wanted to expand my abilities by
becoming relatively efficient in Object Oriented Programing while still being more low level and managing memory. 
I have also recentely done some projects in C# and other OOP laguages as well. C++ like C is very fast as there is no 
garbage collection overhead. There are also many things that you can do with manual pointer manipulation that just 
make C and C++ soooo good. Anyways enough yapping. 


GAME ENGINE DESIGN:

    So for this game I spent a decent amount of time 
planning and designing the "game engine" and making sure that I view everything as an object and follow the 4 pillars
of OOP. Here are some of the things I did to acheive this goal.

1.  I heavily use Inheratance and Polymorphism in my game loop as a store a vector of Entities where
    all of my classes such as player, teacher, and officer are all inherited from the base Entity class. Each time through
    the game loop functions are called on each entity in the vector and obviosly because of polymorphism the specicied 
    fucntion that corellates to whatever inherited class will be properly called. This makes the game loop very simple 
    and scalable. 

2.  Each level itself is also a class that inherites from a base level class. this is important because
    a large portion of the level code (the game loop) is the same for every level. This is achieved through a few neat
    tricks. 

    I.  The base level class has a vector of vectors where each vector in the vector is the vector of entities 
        for a specific room. 
        
    II. There is also a vector of the collision maps for each room. 
    
    because of this, a derived level class can have however many rooms I want and I can also have a universal destructor 
    that cleans up the vectors in the base level class. This works becuase anything that is allocated will be in those vectors.
    now one thing I had to keep in mind is that each entity vector that is specific to a derived level class must be heap 
    allocated. This is because if they are stack allocated they will go out of scope before they get to the base level destructor
    (due to the way c++ destructors execute bottom up). This means that the vectors that actually hold the pointers to the entities are lost,
    and the entities themselves (heap allocated with new) could never be deleted. So by making the level specific vectors heap 
    allocated they do not go out of scope and I can delete all of the entities in the universal destructor. Yippie!!! 
    
3.  The last thing that is important for the "game engine" is the base Enitty class. This class has an update function that handles 
    the sprite sheets and the animations based off of different states (Walking Down, Walking Right, ect). 
    It also has a render method. So now when adding new entities I can just specify their action functions, 
    looking functions, ect so that they act however I want them to. 
    
    
4.  I also wrapped most of the SDL stuff in a gamewindow class that handles the game window and the events 
    (closing, inputs, ect). I also have a texture managing class that handles the textures and such. There is also a class for 
    the collision masks that are just images in black and white where the black is where you cannot go. The class creates an
    vector of bools and has a method that tells if a pixel is solid or not.


5.  In order to make the backgrounds work (not being able to walk through walls and objects) I use a boolian vector that tracks
    wether every pixel is "solid" or walkable. I have a function that takes in a black and white image and turns it into a boolian
    vector that I can then use. This works nicely because it is simple and all I have to do is create a black and white version of 
    my background images. This easy to do and is easily edited and scalable.


    All of these things combnined allows me to easily scale by adding new 
levels or entities as needed without having to modify the game loop and rendering/animations. 


ENTITIES:
    
1.  I have a background entitiy that basically just lobotimizes the entity class
    by overloading the update so that it does nothing as there is no sprite sheet or animations for the static background. 

2.  I then have a player class that has move methods and other things needed for a player. 

3.  I them have a teacher class that is just an FSM that essentially walks a set path. 
    The Teacher could (and maybe should just follow a path vector like the Officer class) but I wrote it before the Officer 
    class and it works fine as is.

4.  Now the most fun class is the Officer class these NCPs are what really make the game. 
    They follow a set path that is specicied at allocation time by passing in a vector to the constructor where the 
    vector is the path they take. This is exremely helpful as it allows me to reuse this class for all of my officers and just 
    specify the path and sprite sheet at allocation time meaning I can have tons of customizability easily. I could easily add more
    entities or levels with this setup.
    
5.  The last entity I made is the Principle. This is the boss that you fight at the end of the final level of the game.
    This entity cycles through different attacks and an idle/walking state. The attack choice is based off of certain details
    such as distance from the player. 


ADDITIONAL NOTES:

   This game depents on the SDL2 library and its image extention. In order for the game to work you must have SDL2 installed. 
sudo apt update && sudo apt install libsdl2-dev libsdl2-image-dev if on Ubuntu.