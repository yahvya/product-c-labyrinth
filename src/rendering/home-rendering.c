#include "./home-rendering.h"
#include "./map-rendering.h"
#include "../game/game.h"

bool renderGameHome(RenderingConfig* renderingConfig)
{
    const GameRenderingConfig* gameRenderingConfig = (GameRenderingConfig*)renderingConfig->data;
    const GameConfig*          gameConfig          = gameRenderingConfig->gameConfig;

    renderMapFromConfig(gameConfig, gameConfig->homeMapConfig);

    return true;
}
