#if !defined(GAME_GAME_H)
#define GAME_GAME_H
    /**
     * @brief Défini si le jeux est en mode de test / développement
     */
    #define GAME_IS_IN_TEST_MODE 1

    #include <stdbool.h>
    #include "./window-manager.h"
    #include "../assets-manager/config-manager/config-manager.h"

    /**
     * @brief Configuration de rendu du jeux
     */
    typedef struct{
        /**
         * @brief Configuration de jeux
         */
        GameConfig* gameConfig;

        /**
         * @brief Fonction de rendu
         */
        bool (*currentRenderingFunction)(RenderingConfig*);
    }GameRenderingConfig;

    /**
     * @brief Initialise les ressources du jeux
     * @return si l'initialisation réussi
     */
    bool initializeGame();

    /**
     * @brief Lance le jeux
     * @return si la lancement du jeux réussi
     */
    void startGame();

    /**
     * @brief Ferme et libère les ressources du jeux
     */
    void closeGame();
#endif