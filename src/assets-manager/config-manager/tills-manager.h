#if !defined(ASSETS_MANAGER_CONFIG_MANAGER_TILLS_MANAGER_H)
#define ASSETS_MANAGER_CONFIG_MANAGER_TILLS_MANAGER_H
    #include <yaml.h>
    #include "./image-config-manager.h"

    /**
     * @brief Configuration des tills. Tableau indicé par l'id converti en entier - 1. Il est conseillé d'utiliser les fonctions d'accès
     */
    typedef ImageConfig* TillsMap;

    /**
     * @brief Configuration de tills
     */
    typedef struct{ 
        /**
         * @brief Nombre d'éléments dans la map
         */
        int countOfTills;
        /**
         * @brief Nombre de tills
         */
        TillsMap map;
    }TillsConfig;

    /**
     * @brief Charge la configuration des tuiles de jeux
     * @param parser l'adresse du parseur initialisé
     * @param parentDirPath chemin du fichier de configuration parent
     * @return TillsConfig la structure des tuiles ou null en cas
     */
    void* loadTillsConfig(yaml_parser_t* parser,char* parentDirPath);

    /**
     * @brief Libère les ressources de la configuration
     * @param config la configuration
     * @param freeContainer si true libère également la configuration pasée
     */
    void freeTillsConfig(TillsConfig* config,bool freeContainer);

    /**
     * @brief Affiche le détails de la configuration
     * @param config configuration
     * @param toPrintBefore Chaine à afficher avant chaque ligne de configuration
     */
    void printTillsConfig(TillsConfig* config,char* toPrintBefore);
#endif
