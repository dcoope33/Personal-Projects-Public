#include "levels1_3.hpp"

int main(int argc, char* args[]) {

    // Initialize gamewindow object and textures
    GameWindow game("Escape the Classroom", 800, 600);
    SDL_Texture *LevelCompleteTex = TextureManager::LoadTexture("Screens/LevelComplete.png", game.getRenderer());
    SDL_Texture *LevelFailedTex = TextureManager::LoadTexture("Screens/LevelFailed.png", game.getRenderer());
    SDL_Texture *GameCompleteTex = TextureManager::LoadTexture("Screens/GameComplete.png", game.getRenderer());
    SDL_Texture *StartTex = TextureManager::LoadTexture("Screens/StartingScreen.png", game.getRenderer());
    SDL_Texture *LevelSelectTex = TextureManager::LoadTexture("Screens/LevelSelect4.png", game.getRenderer());

    // level number is gonna be chosen in the handlestart menu
    int level = game.handleStart(StartTex, LevelSelectTex);

    Level *CurrentLevel = nullptr;
    while(level && game.isRunning()) {
        switch (level) {
            case TUTORIAL:
                CurrentLevel = new Tutorial(&game, LevelCompleteTex, LevelFailedTex);
                // currentLevel->run returns the next level to be run
                level = CurrentLevel->run();
                delete CurrentLevel;
                break;

            case LEVEL_1:
                CurrentLevel = new Level1(&game, LevelCompleteTex, LevelFailedTex);
                level = CurrentLevel->run();
                delete CurrentLevel;
                break;

            case LEVEL_2:
                CurrentLevel = new Level2(&game, LevelCompleteTex, LevelFailedTex);
                level = CurrentLevel->run();
                delete CurrentLevel;
                break;
                
            case LEVEL_3:
                CurrentLevel = new Level3(&game, LevelCompleteTex, LevelFailedTex);
                level = CurrentLevel->run();
                delete CurrentLevel;
                break;

            case LEVEL_4:
                CurrentLevel = new Level4(&game, GameCompleteTex, LevelFailedTex);
                level = CurrentLevel->run();
                delete CurrentLevel;
                break;
        }
    }
}