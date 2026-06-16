#include "principle.hpp"

enum Rooms {Classroom, Hallway1, Hallway2, Hallway3, Outside, Tut, BossFight = 2};

class Level {
protected:
    GameWindow *game;
    int levelID;
    bool isLevelRunning;

    SDL_Texture *LevelCompleteTex;
    SDL_Texture *LevelFailedTex;

    CollisionMap *active_map;
    int room;

    std::vector<Entity*> *gameObjects;
    std::vector<CollisionMap*> Maps;
    std::vector<std::vector<Entity*>*> Rooms;
    
    Player *player;

    bool showHealth = false;

public:
    Level(GameWindow *gameWindow, SDL_Texture *winTex, SDL_Texture *loseTex) 
    : game(gameWindow), LevelCompleteTex(winTex), LevelFailedTex(loseTex), isLevelRunning(true) {
        room = Classroom;
    }
    
    // universal destructor for all levels
    virtual ~Level() {
        // Clean up maps 
        for(CollisionMap* map : Maps) {
            delete map;
        }
        Maps.clear();

        for(std::vector<Entity*>* r : Rooms) {
            if(r != nullptr) {
                for(Entity* e : *r) {
                    // Skip the player (deleted separately)
                    if(e != player) delete e;
                }
                delete r;
            }
        }
        Rooms.clear();


        // Delete the player 
        delete player;
    }

    // these need to be implementeddddddd
    virtual void init() = 0;
    virtual int updateRoom(Player *player) = 0;

    // this doesn't need to be level specific because of how I have it setup but if I added a bunch more stuff
    // it would need to become level specific. 
    int updateRoom_static(int update_room) {
        
        static int timer = 0;
        Uint32 currentTime = SDL_GetTicks();

        int newroom = -1;
        if(update_room != -1) return update_room;
        if(currentTime > timer + 400) {
            if(room == Hallway1 || room == Hallway2) {
                auto coords = player->getCoords();
                int p_x = coords.first;
                int p_y = coords.second;
                if(p_x >= 800-50 && p_y < 300) {
                    player->setPosition(-10, p_y);
                    newroom = ++room;
                }
                if(p_x <= -15 && p_y < 300) {
                    player->setPosition(800-55, p_y);
                    newroom = --room;
                } 
            }
            timer = currentTime;
        }
        return newroom;
    } 


    // these last two functions are generic and stay the same;
    int update() {

        bool E_Pressed = game->handleEvents(*gameObjects);
        int new_room = -1;

        if(E_Pressed) new_room = updateRoom(player);
        new_room = updateRoom_static(new_room);
        
        // update room entities and such if a new room is entered
        if(new_room != -1 && new_room != Outside) {
                room = new_room;
                active_map = Maps[new_room];
                player->setMap(Maps[new_room]);
                gameObjects = Rooms[new_room];
                
                // Important to reset entities
                for(Entity *e : *gameObjects) {
                    e->resetState();
                }

        } else if(new_room == Outside)
                return game->handleGameWin(LevelCompleteTex, levelID);

        // clear the screen 
        game->clear();

        // loop through current room entities
        for(Entity *e : *gameObjects) {

            // each entity does its specified action and updates
            e->action(player->getCoords());
            e->update();
            
            // if look returns true then the player was caught
            if(e->look(active_map, player->getCoords(), player->iscrouched())) 
                if(game->handleGameLoss(LevelFailedTex, levelID)) return levelID;


            int damage = 0;
            // check collision against player but skip the player and background
            if(e != player && e != (*gameObjects)[0]) damage = e->collCheck(player->getCoords(), player->iscrouched());
            player->takeDamage(damage);

            // render every entity
            e->render(game->getRenderer());
        }

        if(showHealth) {
            if(player->displayHealth(game, levelID, room)) {
                sleep(1);
                if(game->handleGameLoss(LevelFailedTex, levelID)) return levelID;
            }
        }
                
        game->display();
        return -1;
    }

    int run() {

        // initialize the level
        init();

        int next = END;

        while(game->isRunning() && isLevelRunning) {
            next = update();
            if(next != -1) isLevelRunning = false;
        }
        return next;
    } // Returns the next state/level ID
};







class Tutorial : public Level {
private:
    CollisionMap *class_map;
    
    std::vector<Entity*> *Classroom_Entities;
    
public:
    Tutorial(GameWindow *gameWindow, SDL_Texture *winTex, SDL_Texture *loseTex) 
        : Level(gameWindow, winTex, loseTex) {
            levelID = LEVEL_1;
            Classroom_Entities = new std::vector<Entity*>;
        }

    ~Tutorial() {}

    void init() override {
        class_map = new CollisionMap();
        class_map->loadFromMask(TextureManager::LoadSurface("Images/Classroom_mask.png"));
        
        Maps.push_back(class_map);

        // need to pass the renderer created in GameWindow
        SDL_Texture *classroomTex = TextureManager::LoadTexture("Images/ClassroomKids.png", game->getRenderer());
        Background *classroom = new Background(classroomTex, 800, 600, Tut);

        SDL_Texture *playerSheet = TextureManager::LoadTexture("Images/sprite_sheet.png", game->getRenderer());
        player = new Player(playerSheet, NULL, 650, 300, 64, 64);
        
        Classroom_Entities->push_back(classroom);
        Classroom_Entities->push_back(player);


        SDL_Texture *Sheet = TextureManager::LoadTexture("Images/Teacher.png", game->getRenderer());
        Teacher *T = new Teacher(Sheet, 550, 122, 64, 64);
        Classroom_Entities->push_back(T);

        Rooms.push_back(Classroom_Entities);

        gameObjects = Classroom_Entities;
        active_map = class_map;
        player->setMap(class_map);
    }

    int updateRoom(Player *player) override {
        int newroom = -1;
        auto coords = player->getCoords();
        int p_x = coords.first;
        int p_y = coords.second;
        switch(room) {
            case Classroom:
                if(p_x > 635 && p_x < 715-SPRITE_W && p_y <= 140) {
                    player->setPosition(171.0f, 415.0f);
                    player->setState(WALK_DOWN);
                    newroom = Outside;
                }
                break;
        }
        return newroom;
    }

};


