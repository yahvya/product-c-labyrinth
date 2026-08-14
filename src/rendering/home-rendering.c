#include "./home-rendering.h"
#include "../game/game.h"
#include "./map-rendering.h"

bool renderGameHome(RenderingConfig* renderingConfig)
{
    GameRenderingConfig* gameRenderingConfig = (GameRenderingConfig*)renderingConfig->data;
    GameConfig*          gameConfig          = gameRenderingConfig->gameConfig;

    renderMapFromConfig(gameConfig, gameConfig->homeMapConfig);

    return true;
}