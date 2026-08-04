#if !defined(ASSETS_MANAGER_CONFIG_MANAGER_CONFIG_MANAGER_H)
#define ASSETS_MANAGER_CONFIG_MANAGER_CONFIG_MANAGER_H
    #include <yaml.h>
    #include "./enemies-manager.h"
    #include "./heroes-manager.h"
    #include "./items-manager.h"
    #include "./tills-manager.h"
    #include "./game-map-manager.h"

    /**
     * @brief Charge la configuration de jeux
     */
    typedef struct{
        /**
         * @brief Configuration des ennemies
         */
        EnemiesConfig* enemiesConfig;

        /**
         * @brief Configuration des héros
         */
        HeroesConfig* heroesConfig;

        /**
         * @brief Configuration des items
         */
        ItemsConfig* itemsConfig;

        /**
         * @brief Configuration des tills
         */
        TillsConfig* tillsConfig;

        /**
         * @brief Configuration de la map d'affichage de la page d'accueil
         */
        GameMapConfig* homeMapConfig;
    }GameConfig;

    /**
     * @brief Charge un fichier de configuration avec comme racine le dossier ressource
     * @param path chemin complet du fichier à partir du dossier ressource comme racine
     * @param treatmentFunction fonction de traitement renvoyant la structure générée ou null en cas d'échec. Prend en argument le parser initialisé (ne doit pas libérer la ressource), le chemin du fichier de configuration parent
     * @return void* La structure chargée ou null en cas d'échec
     */
    void* loadConfig(char* path,void* (*treatmentFunction)(yaml_parser_t*,char*) );

    /**
     * @brief Charge la configuration du jeux
     * @return la configuration ou null
     */
    GameConfig* loadGameConfig();

    /**
     * @brief Charge les images raylib dans la configuration des items
     * @param gameConfig configuration de jeux
     * @return si le chargement réussi
     */
    bool loadItemsImages(GameConfig* gameConfig);

    /**
     * @brief Charge les images raylib dans la configuration des tills
     * @param gameConfig configuration de jeux
     * @return si le chargement réussi
     */
    bool loadTillsImages(GameConfig* gameConfig);

    /**
     * @brief Charge les images raylib dans la configuration des ennemies
     * @param gameConfig configuration de jeux
     * @return si le chargement réussi
     */
    bool loadEnemiesImages(GameConfig* gameConfig);

    /**
    * @brief Charge les images raylib dans la configuration des héros
    * @param gameConfig configuration de jeux
    * @return si le chargement réussi
    */
    bool loadHeroesImages(GameConfig* gameConfig);

    /**
     * @brief Libère les ressources de la configuration de jeux
     * @param config la configuration
     * @param freeContainer si true libère le conteneur fourni
     */
    void freeGameConfig(GameConfig* config,bool freeContainer);
#endif
