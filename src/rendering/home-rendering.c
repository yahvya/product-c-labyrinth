#include "./home-rendering.h"
#include "./map-rendering.h"
#include "../game/game.h"

bool renderGameHome(RenderingConfig* renderingConfig)
{
    const GameRenderingConfig* gameRenderingConfig = (GameRenderingConfig*)renderingConfig->data;
    const GameConfig*          gameConfig          = gameRenderingConfig->gameConfig;

    // Définition du mouvement des ennemis comme regardant sur en face par défaut

    for (int enemyIndex = 0; enemyIndex < gameConfig->enemiesConfig->countOfEnemies; enemyIndex++)
    {
        EnemyConfig* enemyConfig = gameConfig->enemiesConfig->map + enemyIndex;
        
        enemyConfig->currentAction = ENEMY_FRONT_WALK;
    }

    renderMapFromConfig(gameConfig, gameConfig->homeMapConfig);

    return true;
}
