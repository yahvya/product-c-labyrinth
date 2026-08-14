#ifndef GAME_RENDERING_H
#define GAME_RENDERING_H
#include "raylib.h"
#include "../assets-manager/config-manager/game-map-manager.h"
#include "../assets-manager/config-manager/image-config-manager.h"

/**
 * Dessine une image à partir de la configuration
 * @param imageConfig  Configuration de l'image
 * @param mapConfig  Configuration de la map
 * @param texturePosition  Configuration de la position de l'image
 * @return
 */
bool renderImageFromConfig(ImageConfig* imageConfig, const GameMapConfig* mapConfig, Vector2 texturePosition);
#endif
