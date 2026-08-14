#include "./game.h"
#include <stddef.h>
#include "raylib.h"
#include "../rendering/home-rendering.h"

/**
 * @brief Chargement d'image ou libération des ressources du jeux
 */
#define LOAD_OR_QUIT(loadFunction,gameConfig)if(!loadFunction(gameConfig)){\
    freeGameConfig(gameConfig,true);\
    return false;\
}

// ressources du jeu
static GameConfig* gameConfig = NULL;

bool initializeGame()
{
    SetTraceLogLevel(GAME_IS_IN_TEST_MODE == 1 ? LOG_ALL : LOG_ERROR | LOG_WARNING | LOG_FATAL);

    InitWindow(0, 0, "Initialize raylib");

    if (!IsWindowReady())
        return false;

    gameConfig = loadGameConfig();

    if (gameConfig == NULL)
        return false;

    LOAD_OR_QUIT(loadItemsImages, gameConfig)
    LOAD_OR_QUIT(loadTillsImages, gameConfig)
    LOAD_OR_QUIT(loadEnemiesImages, gameConfig)
    LOAD_OR_QUIT(loadHeroesImages, gameConfig)

    SetWindowTitle(WINDOW_TITLE);
    SetWindowIcon(gameConfig->windowIcon);
    SetWindowSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    SetWindowFocused();
    SetTargetFPS(30);
    centerWindow();

    TraceLog(LOG_INFO, "Game initialized");

    return true;
}

void startGame()
{
    TraceLog(LOG_INFO, "Game started");

    GameRenderingConfig gameRenderingConfig = {
        .gameConfig               = gameConfig,
        .currentRenderingFunction = renderGameHome
    };

    RenderingConfig renderingConfig = {
        .data = &gameRenderingConfig
    };

    // rendu du jeu
    while (!WindowShouldClose())
    {
        if (!renderWindow(&renderingConfig, gameRenderingConfig.currentRenderingFunction))
        {
            TraceLog(LOG_ERROR, "Echec de rendu de fenêtre");
            break;
        }
    }
}

void closeGame()
{
    freeGameConfig(gameConfig, true);
    CloseWindow();

    TraceLog(LOG_INFO, "Game closed");
}
