#include "./home-rendering.h"
#include "../game/game.h"
#include "raylib.h"

bool renderGameHome(RenderingConfig* renderingConfig){
    GameRenderingConfig* gameRenderingConfig = (GameRenderingConfig*) renderingConfig->data;
    GameConfig* gameConfig = gameRenderingConfig->gameConfig;

    return true;
}