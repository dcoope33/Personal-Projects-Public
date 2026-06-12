#include "levels.hpp"

class Level1 : public Level {
private:
    CollisionMap *class_map;
    CollisionMap *hall_map;
    
    std::vector<Entity*> *Classroom_Entities;
    std::vector<Entity*> *Hallway_Entities;
    
public:
    Level1(GameWindow *gameWindow, SDL_Texture *winTex, SDL_Texture *loseTex) 
        : Level(gameWindow, winTex, loseTex) {
            levelID = LEVEL_1;
            Classroom_Entities = new std::vector<Entity*>;
            Hallway_Entities = new std::vector<Entity*>;
        }

    ~Level1() {}

    void init() override {
        class_map = new CollisionMap();
        class_map->loadFromMask(TextureManager::LoadSurface("Images/Classroom_mask.png"));
        
        hall_map = new CollisionMap();
        hall_map->loadFromMask(TextureManager::LoadSurface("Images/Hallways_mask.png"));

        Maps.push_back(class_map);
        Maps.push_back(hall_map);

        // need to pass the renderer created in GameWindow
        SDL_Texture *classroomTex = TextureManager::LoadTexture("Images/ClassroomKids.png", game->getRenderer());
        SDL_Texture *hallwayTex = TextureManager::LoadTexture("Images/Hallways.png", game->getRenderer());
        Background *classroom = new Background(classroomTex, 800, 600, Classroom);
        Background *hallway = new Background(hallwayTex, 800, 600, Hallway3);

        SDL_Texture *playerSheet = TextureManager::LoadTexture("Images/sprite_sheet.png", game->getRenderer());
        player = new Player(playerSheet, NULL, 650, 300, 64, 64);
        
        Classroom_Entities->push_back(classroom);
        Hallway_Entities->push_back(hallway);
        Classroom_Entities->push_back(player);
        Hallway_Entities->push_back(player);


        SDL_Texture *Sheet = TextureManager::LoadTexture("Images/Teacher.png", game->getRenderer());
        Teacher *T = new Teacher(Sheet, 550, 122, 64, 64);
        Classroom_Entities->push_back(T);

        SDL_Texture *OSheet = TextureManager::LoadTexture("Images/Officer.png", game->getRenderer());
        Officer *O1 = new Officer(OSheet, 600, 150, path1, 64, 64);
        Officer *O2 = new Officer(OSheet, 620, 455, path2, 64, 64);
        Hallway_Entities->push_back(O1);
        Hallway_Entities->push_back(O2);

        Rooms.push_back(Classroom_Entities);
        Rooms.push_back(Hallway_Entities);

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
                    newroom = Hallway1;
                }
                break;

            case Hallway1:
                if(p_x > 165 && p_x < 235-SPRITE_W && p_y <= 400 && p_y >= 370) {
                    player->setPosition(650.0f, 142.0f);
                    player->setState(WALK_DOWN);
                    newroom = Classroom;

                } else if(p_x > 585 && p_x < 690-SPRITE_W && p_y <= 135) 
                    newroom = Outside;

                 else if(p_x > 570 && p_x < 675-SPRITE_W && p_y <= 450 && p_y >= 300)
                    player->setPosition(p_x, 300.0f);

                break;
        }
        return newroom;
    }

};


class Level2 : public Level {
private:
    CollisionMap *class_map;
    CollisionMap *hall1_map;
    CollisionMap *hall2_map;
    
    std::vector<Entity*> *Classroom_Entities;
    std::vector<Entity*> *Hallway1_Entities;
    std::vector<Entity*> *Hallway2_Entities;
    
public:
    Level2(GameWindow *gameWindow, SDL_Texture *winTex, SDL_Texture *loseTex) 
        : Level(gameWindow, winTex, loseTex) {
            levelID = LEVEL_2;
            Classroom_Entities = new std::vector<Entity*>;
            Hallway1_Entities = new std::vector<Entity*>;
            Hallway2_Entities = new std::vector<Entity*>;
        }

    ~Level2() {}

    void init() override {
        class_map = new CollisionMap();
        class_map->loadFromMask(TextureManager::LoadSurface("Images/Classroom_mask.png"));
        
        hall1_map = new CollisionMap();
        hall1_map->loadFromMask(TextureManager::LoadSurface("Images/Hallway1_mask.png"));

        hall2_map = new CollisionMap();
        hall2_map->loadFromMask(TextureManager::LoadSurface("Images/Hallway3_mask.png"));

        Maps.push_back(class_map);
        Maps.push_back(hall1_map);
        Maps.push_back(hall2_map);

        // need to pass the renderer created in GameWindow
        SDL_Texture *classroomTex = TextureManager::LoadTexture("Images/ClassroomKids.png", game->getRenderer());
        SDL_Texture *hallway1Tex = TextureManager::LoadTexture("Images/Hallway1.png", game->getRenderer());
        SDL_Texture *hallway2Tex = TextureManager::LoadTexture("Images/Hallway3.png", game->getRenderer());
        Background *classroom = new Background(classroomTex, 800, 600, Classroom);
        Background *hallway1 = new Background(hallway1Tex, 800, 600, Hallway1);
        Background *hallway2 = new Background(hallway2Tex, 800, 600, Hallway3);

        SDL_Texture *playerSheet = TextureManager::LoadTexture("Images/sprite_sheet.png", game->getRenderer());
        player = new Player(playerSheet, NULL, 650, 300, 64, 64);
        
        Classroom_Entities->push_back(classroom);
        Hallway1_Entities->push_back(hallway1);
        Hallway2_Entities->push_back(hallway2);
        Classroom_Entities->push_back(player);
        Hallway1_Entities->push_back(player);
        Hallway2_Entities->push_back(player);

        Rooms.push_back(Classroom_Entities);
        Rooms.push_back(Hallway1_Entities);
        Rooms.push_back(Hallway2_Entities);


        SDL_Texture *Sheet = TextureManager::LoadTexture("Images/Teacher.png", game->getRenderer());
        Teacher *T = new Teacher(Sheet, 550, 122, 64, 64);
        Classroom_Entities->push_back(T);

        SDL_Texture *OSheet = TextureManager::LoadTexture("Images/Officer.png", game->getRenderer());
        Officer *O1 = new Officer(OSheet, 600, 160, path1, 64, 64);
        Officer *O2 = new Officer(OSheet, 620, 460, path3, 64, 64);
        Hallway1_Entities->push_back(O1);
        Hallway1_Entities->push_back(O2);

        Officer *O3 = new Officer(OSheet, 600, 165, path4, 64, 64);
        Officer *O4 = new Officer(OSheet, 620, 455, path2, 64, 64);
        Hallway2_Entities->push_back(O3);
        Hallway2_Entities->push_back(O4);

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
                    newroom = Hallway1;
                }
                break;
            case Hallway1:
                if(p_x > 165 && p_x < 235-SPRITE_W && p_y <= 400 && p_y >= 370) {
                    player->setPosition(650.0f, 142.0f);
                    player->setState(WALK_DOWN);
                    newroom = Classroom;

                } else if(p_x > 570 && p_x < 675-SPRITE_W && p_y <= 430 && p_y >= 300) {
                    player->setPosition(550.0f, 300.0f);
                    player->setState(WALK_DOWN);

                } else if(p_x >= 530 && p_x <= 580 && p_y <= 292 && p_y >= 280) {
                    player->setPosition(600, 450);
                    player->setState(WALK_DOWN);
                }
                break;
            
            case Hallway2:
                if(p_x > 165 && p_x < 235-SPRITE_W && p_y <= 400 && p_y >= 370) {
                    player->setPosition(650.0f, 142.0f);
                    player->setState(WALK_DOWN);
                    newroom = Classroom;

                } else if(p_x > 585 && p_x < 690-SPRITE_W && p_y <= 135) 
                    newroom = Outside;

                 else if(p_x > 570 && p_x < 675-SPRITE_W && p_y <= 450 && p_y >= 300)
                    player->setPosition(p_x, 300.0f);

                break;
        }
        return newroom;
    }

};


class Level3 : public Level {
private:
    CollisionMap *class_map;
    CollisionMap *hall1_map;
    CollisionMap *hall2_map;
    CollisionMap *hall3_map;
    
    std::vector<Entity*> *Classroom_Entities;
    std::vector<Entity*> *Hallway1_Entities;
    std::vector<Entity*> *Hallway2_Entities;
    std::vector<Entity*> *Hallway3_Entities;

    
public:
    Level3(GameWindow *gameWindow, SDL_Texture *winTex, SDL_Texture *loseTex) 
        : Level(gameWindow, winTex, loseTex) {
            levelID = LEVEL_3;
            Classroom_Entities = new std::vector<Entity*>;
            Hallway1_Entities = new std::vector<Entity*>;
            Hallway2_Entities = new std::vector<Entity*>;
            Hallway3_Entities = new std::vector<Entity*>;
        }

    ~Level3() {}

    void init() override {
        class_map = new CollisionMap();
        class_map->loadFromMask(TextureManager::LoadSurface("Images/Classroom_mask.png"));
        
        hall1_map = new CollisionMap();
        hall1_map->loadFromMask(TextureManager::LoadSurface("Images/Hallway1_mask.png"));

        hall2_map = new CollisionMap();
        hall2_map->loadFromMask(TextureManager::LoadSurface("Images/Hallway2_mask.png"));

        hall3_map = new CollisionMap();
        hall3_map->loadFromMask(TextureManager::LoadSurface("Images/Hallway3_mask.png"));

        Maps.push_back(class_map);
        Maps.push_back(hall1_map);
        Maps.push_back(hall2_map);
        Maps.push_back(hall3_map);

        // need to pass the renderer created in GameWindow
        SDL_Texture *classroomTex = TextureManager::LoadTexture("Images/ClassroomKids.png", game->getRenderer());
        SDL_Texture *hallway1Tex = TextureManager::LoadTexture("Images/Hallway1.png", game->getRenderer());
        SDL_Texture *hallway2Tex = TextureManager::LoadTexture("Images/Hallway2.png", game->getRenderer());
        SDL_Texture *hallway3Tex = TextureManager::LoadTexture("Images/Hallway3.png", game->getRenderer());
        Background *classroom = new Background(classroomTex, 800, 600, Classroom);
        Background *hallway1 = new Background(hallway1Tex, 800, 600, Hallway1);
        Background *hallway2 = new Background(hallway2Tex, 800, 600, Hallway2);
        Background *hallway3 = new Background(hallway3Tex, 800, 600, Hallway3);

        SDL_Texture *playerSheet = TextureManager::LoadTexture("Images/sprite_sheet.png", game->getRenderer());
        player = new Player(playerSheet, NULL, 650, 300, 64, 64);
        
        Classroom_Entities->push_back(classroom);
        Hallway1_Entities->push_back(hallway1);
        Hallway2_Entities->push_back(hallway2);
        Hallway3_Entities->push_back(hallway3);
        Classroom_Entities->push_back(player);
        Hallway1_Entities->push_back(player);
        Hallway2_Entities->push_back(player);
        Hallway3_Entities->push_back(player);

        Rooms.push_back(Classroom_Entities);
        Rooms.push_back(Hallway1_Entities);
        Rooms.push_back(Hallway2_Entities);
        Rooms.push_back(Hallway3_Entities);

        SDL_Texture *Sheet = TextureManager::LoadTexture("Images/Teacher.png", game->getRenderer());
        Teacher *T = new Teacher(Sheet, 550, 122, 64, 64);
        Classroom_Entities->push_back(T);

        SDL_Texture *OSheet = TextureManager::LoadTexture("Images/Officer.png", game->getRenderer());
        Officer *O1 = new Officer(OSheet, 600, 160, path1, 64, 64);
        Officer *O2 = new Officer(OSheet, 620, 460, path3, 64, 64);
        Hallway1_Entities->push_back(O1);
        Hallway1_Entities->push_back(O2);

        Officer *O3 = new Officer(OSheet, 600, 155, path1, 64, 64);
        Officer *O4 = new Officer(OSheet, 620, 460, path2, 64, 64);
        Hallway2_Entities->push_back(O3);
        Hallway2_Entities->push_back(O4);

        Officer *O5 = new Officer(OSheet, 600, 165, path4, 64, 64);
        Officer *O6 = new Officer(OSheet, 620, 455, path2, 64, 64);
        Hallway3_Entities->push_back(O5);
        Hallway3_Entities->push_back(O6);

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
                    newroom = Hallway1;
                }
                break;
            case Hallway1:
                if(p_x > 165 && p_x < 235-SPRITE_W && p_y <= 400 && p_y >= 370) {
                    player->setPosition(650.0f, 142.0f);
                    player->setState(WALK_DOWN);
                    newroom = Classroom;

                } else if(p_x > 570 && p_x < 675-SPRITE_W && p_y <= 430 && p_y >= 300) {
                    player->setPosition(550.0f, 300.0f);
                    player->setState(WALK_DOWN);

                } else if(p_x >= 530 && p_x <= 580 && p_y <= 292 && p_y >= 280) {
                    player->setPosition(600, 450);
                    player->setState(WALK_DOWN);
                }
                break;

            case Hallway2:
                if(p_x > 570 && p_x < 675-SPRITE_W && p_y <= 450 && p_y >= 300) {
                    player->setPosition(550.0f, 300.0f);
                    player->setState(WALK_DOWN);

                } else if(p_x >= 530 && p_x <= 580 && p_y <= 292 && p_y >= 280) {
                    player->setPosition(600, 450);
                    player->setState(WALK_DOWN);
                }
                break;
            
            case Hallway3:
                if(p_x > 165 && p_x < 235-SPRITE_W && p_y <= 400 && p_y >= 370) {
                    player->setPosition(650.0f, 142.0f);
                    player->setState(WALK_DOWN);
                    newroom = Classroom;

                } else if(p_x > 585 && p_x < 690-SPRITE_W && p_y <= 135) 
                    newroom = Outside;

                 else if(p_x > 570 && p_x < 675-SPRITE_W && p_y <= 450 && p_y >= 300)
                    player->setPosition(p_x, 300.0f);

                break;
        }
        return newroom;
    }

};


class Level4 : public Level {
private:
    CollisionMap *class_map;
    CollisionMap *hall_map;
    CollisionMap *boss_map;
    
    std::vector<Entity*> *Classroom_Entities;
    std::vector<Entity*> *Hallway_Entities;
    std::vector<Entity*> *BossFight_Entities;
    
public:
    Level4(GameWindow *gameWindow, SDL_Texture *winTex, SDL_Texture *loseTex) 
        : Level(gameWindow, winTex, loseTex) {
            levelID = LEVEL_4;
            Classroom_Entities = new std::vector<Entity*>;
            Hallway_Entities = new std::vector<Entity*>;
            BossFight_Entities = new std::vector<Entity*>;

            srand(time(NULL));
        }

    ~Level4() {}

    void init() override {
        class_map = new CollisionMap();
        class_map->loadFromMask(TextureManager::LoadSurface("Images/Classroom_mask.png"));
        
        hall_map = new CollisionMap();
        hall_map->loadFromMask(TextureManager::LoadSurface("Images/Hallways_mask.png"));

        boss_map = new CollisionMap();
        boss_map->loadFromMask(TextureManager::LoadSurface("Images/BossFight_mask.png"));

        Maps.push_back(class_map);
        Maps.push_back(hall_map);
        Maps.push_back(boss_map);

        // need to pass the renderer created in GameWindow
        SDL_Texture *classroomTex = TextureManager::LoadTexture("Images/ClassroomKids.png", game->getRenderer());
        SDL_Texture *hallwayTex = TextureManager::LoadTexture("Images/Hallways.png", game->getRenderer());
        SDL_Texture *bossfightTex = TextureManager::LoadTexture("Images/BossFight.png", game->getRenderer());
        Background *classroom = new Background(classroomTex, 800, 600, Classroom);
        Background *hallway = new Background(hallwayTex, 800, 600, Hallway3);
        Background *bossfight = new Background(bossfightTex, 800, 600, BossFight);


        SDL_Texture *playerSheet = TextureManager::LoadTexture("Images/sprite_sheet.png", game->getRenderer());
        player = new Player(playerSheet, TextureManager::LoadTexture("Images/HeathBar.png", game->getRenderer()), 650, 300, 64, 64);
        
        Classroom_Entities->push_back(classroom);
        Hallway_Entities->push_back(hallway);
        BossFight_Entities->push_back(bossfight);
        Classroom_Entities->push_back(player);
        Hallway_Entities->push_back(player);
        BossFight_Entities->push_back(player);

        SDL_Texture *Sheet = TextureManager::LoadTexture("Images/Teacher.png", game->getRenderer());
        Teacher *T = new Teacher(Sheet, 550, 122, 64, 64);
        Classroom_Entities->push_back(T);

        SDL_Texture *PSheet = TextureManager::LoadTexture("Images/Principle.png", game->getRenderer());
        Principle *P = new Principle(PSheet, 350, 400, 120, 120);
        BossFight_Entities->push_back(P);

        Rooms.push_back(Classroom_Entities);
        Rooms.push_back(Hallway_Entities);
        Rooms.push_back(BossFight_Entities);

        gameObjects = Classroom_Entities;
        active_map = class_map;
        player->setMap(class_map);
        P->setMap(boss_map);
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
                    newroom = Hallway1;
                }
                break;
            
            case Hallway1:
                if(p_x > 165 && p_x < 235-SPRITE_W && p_y <= 400 && p_y >= 370) {
                    player->setPosition(650.0f, 142.0f);
                    player->setState(WALK_DOWN);
                    newroom = Classroom;

                } else if(p_x > 585 && p_x < 690-SPRITE_W && p_y <= 135) { 
                    player->setPosition(370, 80);
                    player->setState(WALK_DOWN);
                    newroom = Hallway2;
                    showHealth = true;

                } else if(p_x > 570 && p_x < 675-SPRITE_W && p_y <= 450 && p_y >= 300)
                    player->setPosition(p_x, 300.0f);

                break;
        }
        return newroom;
    }

};