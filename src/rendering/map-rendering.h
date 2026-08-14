#if !defined(GAME_MAP_RENDERING_H)
#define GAME_MAP_RENDERING_H
#include "../assets-manager/config-manager/config-manager.h"
#include "../assets-manager/config-manager/game-map-manager.h"

/**
 * @brief Affiche une map avec ses éléments
 * @param gameConfig Configuration du jeux
 * @param mapConfig Configuration de la disposition actuelle de la map
 * @return
 */
bool renderMapFromConfig(const GameConfig* gameConfig, const GameMapConfig* mapConfig);

/**
 * @brief Dessine les tills de la map
 * @param gameConfig Configuration du jeux
 * @param mapConfig Configuration de la disposition actuelle de la map
 * @return Etat de succès des dessins
 */
bool drawTills(const GameConfig* gameConfig, const GameMapConfig* mapConfig);

/**
 * Dessine les items de la map
 * @param gameConfig Configuration du jeux
 * @param mapConfig Configuration de la disposition actuelle de la map
 * @return Etat de succès des dessins
 */
bool drawItems(const GameConfig* gameConfig, const GameMapConfig* mapConfig);

/**
 * Dessine les enemies de la map
 * @param gameConfig Configuration du jeux
 * @param mapConfig Configuration de la disposition actuelle de la map
 * @return Etat de succès des dessins
 */
bool drawEnemies(const GameConfig* gameConfig, const GameMapConfig* mapConfig);
#endif
