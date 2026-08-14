#if !defined(ASSETS_MANAGER_CONFIG_MANAGER_ENNEMIES_MANAGER_H)
#define ASSETS_MANAGER_CONFIG_MANAGER_ENNEMIES_MANAGER_H
#include <yaml.h>
#include "./image-config-manager.h"

/**
 * @brief Longueur maximale supposée du nom d'un ennemie
 */
#define SUPPOSED_ENEMY_NAME_MAX_LEN 13

/**
 * @brief Longueur max supposée du nom d'une action ennemie
 */
#define SUPPOSED_ENEMY_ACTION_MAX_LEN 20

/**
 * @brief Longueur max supposée de l'id d'un ennemie
 */
#define SUPPOSED_ENEMY_ID_MAX_LEN 4

/**
 * @brief Liste des actions ennemie
 */
typedef enum
{
    ENEMY_FRONT_WALK,
    ENEMY_RIGHT_WALK,
    ENEMY_LEFT_WALK,
    ENEMY_BACK_WALK,
    /**
     * @brief Clé max des actions ennemies
     */
    ENEMY_MAX_FOR_ARRAY_KEYS
} EnemyAction;

/**
 * @brief Configuration d'action ennemie
 */
typedef struct
{
    /**
     * @brief Nom de l'action
     */
    char actionName[SUPPOSED_ENEMY_ACTION_MAX_LEN];

    /**
     * @brief Configuration des images de l'action
     */
    ImageConfig* framesConfig;
} EnemyActionConfig;

/**
 * @brief Configuration d'un ennemie
 */
typedef struct
{
    /**
     * @brief Map des actions ennemies
     */
    EnemyActionConfig actionsMap[ENEMY_MAX_FOR_ARRAY_KEYS];

    /**
     * @brief Nom de l'ennemie
     */
    char name[SUPPOSED_ENEMY_NAME_MAX_LEN];

    /**
     * @brief Action courante de l'ennemi.
     * @attention Chargé par défaut sur ENEMY_MAX_FOR_ARRAY_KEYS
     */
    EnemyAction currentAction;

    /**
     * @brief Id ennemie
     */
    char id[SUPPOSED_ENEMY_ID_MAX_LEN];
} EnemyConfig;

/**
 * @brief map des ennemies
 */
typedef EnemyConfig* EnemiesMap;

/**
 * @brief Configuration des ennemies
 */
typedef struct
{
    /**
     * @brief Map des ennemies. Tableau indicé par l'id converti en entier - 1. Il est conseillé d'utiliser les fonctions d'accès
     */
    EnemiesMap map;

    /**
     * @brief Nombre d'ennemies
     */
    int countOfEnemies;
} EnemiesConfig;

/**
 * @brief Charge la configuration des ennemies
 * @param parser le parser
 * @param parentDirPath chemin du dossier parent
 * @return la configuration ou null en cas d'échec
 */
void* loadEnemies(yaml_parser_t* parser, const char* parentDirPath);

/**
 * @brief Recherche l'index une action ennemie à partir du nom
 * @param name nom de l'action
 * @return l'index de l'action ou -1 si non trouvé
 */
EnemyAction getEnemyActionIndexFromName(const char* name);

/**
 * @brief Libère la configuration de l'ennemie
 * @param config configuration
 * @param freeContainer si true libère le conteneur
 */
void freeEnemyConfig(EnemyConfig* config, bool freeContainer);

/**
 * @brief Libère la configuration des ennemies
 * @param config configuration
 * @param freeContainer si true libère le conteneur
 */
void freeEnemiesConfig(EnemiesConfig* config, bool freeContainer);

/**
 * @brief Affiche la configuration des ennemies
 * @param enemiesConfig configuration ennemies
 * @param toPrintBefore à afficher avant chaque ligne de configuration
 */
void printEnemiesConfig(const EnemiesConfig* enemiesConfig, char* toPrintBefore);

/**
 * Fourni un des ennemies de la configuration à partir de son id
 * @param id Identifiant de l'ennemie
 * @param enemiesConfig Configuration des items
 * @return La configuration de l'ennemie ou null si non trouvé
 */
EnemyConfig* getEnemyImageConfigFromId(int id, const EnemiesConfig* enemiesConfig);
#endif
