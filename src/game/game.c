#include "./game.h"
#include "../rendering/home-rendering.h"
#include <stddef.h>
#include "raylib.h"

/**
 * @brief Chargement d'image ou libération des ressources du jeux
 */
#define LOAD_OR_QUIT(loadFunction,gameConfig)if(!loadFunction(gameConfig)){\
    freeGameConfig(gameConfig,true);\
    return false;\
}

// ressources du jeu
static GameConfig* gameConfig = NULL;

bool initializeGame(){
    InitWindow(0,0,"Initialize raylib");

    if(!IsWindowReady())
        return false;

    gameConfig = loadGameConfig();

    if(gameConfig == NULL)
        return false;

    printMapConfig(gameConfig->homeMapConfig);

    LOAD_OR_QUIT(loadItemsImages,gameConfig)
    LOAD_OR_QUIT(loadTillsImages,gameConfig)
    LOAD_OR_QUIT(loadEnemiesImages,gameConfig)
    LOAD_OR_QUIT(loadHeroesImages,gameConfig)

    SetWindowTitle("Labyrinth");
    SetWindowSize(WINDOW_DEFAULT_WIDTH,WINDOW_DEFAULT_HEIGHT);
    SetWindowFocused();
    centerWindow();

    return true;
}

void startGame(){
    GameRenderingConfig gameRenderingConfig = {
        .gameConfig = gameConfig,
        .currentRenderingFunction = renderGameHome
    };

    RenderingConfig renderingConfig = {
        .data = &gameRenderingConfig
    };

    // rendu du jeu
    while(!WindowShouldClose()){
        if(!renderWindow(&renderingConfig,gameRenderingConfig.currentRenderingFunction)){
            fputs("Echec de rendu de fenêtre",stderr);
            break;
        }
    }
}

void closeGame(){
    CloseWindow();
    freeGameConfig(gameConfig,true);
}