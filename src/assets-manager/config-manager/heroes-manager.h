#if !defined(ASSETS_MANAGER_CONFIG_MANAGER_HEROES_MANAGER_H)
#define ASSETS_MANAGER_CONFIG_MANAGER_HEROES_MANAGER_H
    #include "./image-config-manager.h"
    #include <yaml.h>

    /**
     * @brief Longueur max supposée de l'id d'un héro
     */
    #define SUPPOSED_HERO_ID_MAX_LEN 15

    /**
     * @brief Longueur max supposée du nom d'un héro
     */
    #define SUPPOSED_HERO_NAME_MAX_LEN 20

    /**
     * @brief Longueur max supposée du nom de l'action d'un héro
     */
    #define SUPPOSED_HERO_ACTION_NAME_MAX_LEN 40

    /**
     * @brief Liste des actions hero
     */
    typedef enum{
        HERO_BACK_ANIMATION,
        HERO_DEATH_ANIMATION,
        HERO_FRONT_ANIMATION,
        HERO_LEFT_ANIMATION,
        HERO_RIGHT_ANIMATION,
        HERO_VICTORY_ANIMATION,
        HERO_IDLE_FRONT_ANIMATION,
        HERO_IDLE_BACK_ANIMATION,
        HERO_IDLE_LEFT_ANIMATION,
        HERO_IDLE_RIGHT_ANIMATION,
        HERO_FRONT_WALK_ANIMATION,
        HERO_RIGHT_WALK_ANIMATION,
        HERO_LEFT_WALK_ANIMATION,
        HERO_BACK_WALK_ANIMATION,
        /**
         * @brief Clé pour défini la taille max des valeurs d'actions
         */
        HERO_MAX_FOR_ARRAY_KEYS
    }HeroAction;

    /**
     * @brief Configuration d'une action hero
     */
    typedef struct {
        /**
         * @brief Nom de l'action
         */
        char actionName[SUPPOSED_HERO_ACTION_NAME_MAX_LEN];

        /**
         * @brief Liste des images composant l'action
         */
         ImageConfig* framesConfig;
    }HeroActionConfig;

    /**
     * @brief Configuration d'un héro
     */
    typedef struct{
        /**
         * @brief Configuration des actions héro
         */
        HeroActionConfig actionsConfigs[HERO_MAX_FOR_ARRAY_KEYS];

        /**
         * @brief Nom du héro
         */
        char name[SUPPOSED_HERO_NAME_MAX_LEN];

        /**
         * @brief If du héro
         */
        char id[SUPPOSED_HERO_ID_MAX_LEN];
    }HeroConfig;

    /**
     * @brief Map des héros
     */
    typedef HeroConfig* HeroesMap;

    /**
     * @brief Configuration des héros
     */
    typedef struct{
        HeroesMap map;

        /**
         * @brief Nombre de héros
         */
        int countOfHeroes;
    }HeroesConfig;

    /**
     * @brief Recherche l'indice tableau d'une action héro à partir du nom
     * @param name nom de l'action
     * @return l'indice ou -1 si non trouvé
     */
    HeroAction getHeroActionIndexFromName(char* name);

    /**
     * @brief Charge la configuration
     * @param parser parser
     * @param parentDirPath chemin du dossier parent
     * @return la configuration
     */
    void* loadHeroesConfig(yaml_parser_t *parser,char* parentDirPath);

    /**
     * @brief libère la configuration d'un héro
     * @param config configuration
     * @param freeContainer si true libère la config fournie
     */
    void freeHeroConfig(HeroConfig* config,bool freeContainer);

    /**
     * @brief libère la configuration de héros
     * @param config configuration
     * @param freeContainer si true libère la config fournie
     */
    void freeHeroesConfig(HeroesConfig* config,bool freeContainer);

    /**
     * @brief Affiche la configuration des héros
     * @param heroesConfig configuration
     * @param toPrintBefore à afficher avant chaque ligne de configuration
     */
    void printHeroesConfig(HeroesConfig* heroesConfig, char* toPrintBefore);
#endif