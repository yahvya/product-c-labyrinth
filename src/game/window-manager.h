#if !defined(GAME_WINDOW_MANAGER_H)
#define GAME_WINDOW_MANAGER_H

#include <stdbool.h>

/**
 * @brief Largeur par défaut de la fenêtre
 */
#define WINDOW_DEFAULT_WIDTH 0
/**
 * @brief Hauteur par défaut de la fenêtre
 */
#define WINDOW_DEFAULT_HEIGHT 0

/**
 * @brief Largeur de la fenêtre d'accueil
 */
#define WINDOW_HOME_WIDTH 800

/**
 * @brief Hauteur de la fenêtre d'accueil
 */
#define WINDOW_HOME_HEIGHT 600

/**
 * @brief Largeur de la fenêtre de jeu
 */
#define WINDOW_LEVELS_WIDTH 800

/**
 * Hauteur de la fenêtre de jeu
 */
#define WINDOW_LEVELS_HEIGHT 600

/**
 * @brief Configuration de rendu
 */
typedef struct
{
    /**
     * @brief Données à transmettre
     */
    void* data;
} RenderingConfig;

/**
 * @brief Centre la fenêtre
 */
void centerWindow();

/**
 * @brief Gère le rendu de fenêtre
 * @param renderingConfig configuration de rendu (transmise à la fonction de rendu)
 * @param customRenderingFunction fonction de rendu customisé, retourne la réussite des actions internes
 * @return si le rendu à réussi
 */
bool renderWindow(RenderingConfig* renderingConfig, bool (*customRenderingFunction)(RenderingConfig*));
#endif
