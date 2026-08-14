#if !defined(GAME_MAP_RENDERING_H)
#define GAME_MAP_RENDERING_H
#include "../assets-manager/config-manager/game-map-manager.h"
#include "../assets-manager/config-manager/config-manager.h"

/**
 * @brief Affiche une map avec ses éléments
 * @param gameConfig Configuration du jeux
 * @param mapConfig Configuration de la disposition actuelle de la map
 * @return
 */
bool renderMapFromConfig(GameConfig* gameConfig, GameMapConfig* mapConfig);
#endif
