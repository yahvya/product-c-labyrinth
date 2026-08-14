#include "./home-rendering.h"
#include "./map-rendering.h"
#include "../game/game.h"

bool renderGameHome(RenderingConfig* renderingConfig)
{
    static bool resetWindowSize = true;

    const GameRenderingConfig* gameRenderingConfig = (GameRenderingConfig*)renderingConfig->data;
    const GameConfig*          gameConfig          = gameRenderingConfig->gameConfig;

    // Gestion de la taille associé à l'accueil

    if (resetWindowSize)
    {
        SetWindowSize(WINDOW_HOME_WIDTH * gameConfig->homeMapConfig->scale, WINDOW_HOME_HEIGHT * gameConfig->homeMapConfig->scale);
        centerWindow();

        resetWindowSize = false;
    }

    // Définition du mouvement des ennemis comme regardant sur en face par défaut

    for (int enemyIndex = 0; enemyIndex < gameConfig->enemiesConfig->countOfEnemies; enemyIndex++)
    {
        EnemyConfig* enemyConfig = gameConfig->enemiesConfig->map + enemyIndex;

        enemyConfig->currentAction = ENEMY_FRONT_WALK;
    }

    renderMapFromConfig(gameConfig, gameConfig->homeMapConfig);

    return true;
}
