#if !defined(ASSETS_MANAGER_CONFIG_MANAGER_ITEMS_CONFIG_MANAGER_H)
#define ASSETS_MANAGER_CONFIG_MANAGER_ITEMS_CONFIG_MANAGER_H
    #include "yaml.h"

    #include "./image-config-manager.h"

    /**
     * @brief Configuration des items. Tableau indicé par l'id converti en entier - 1. Il est conseillé d'utiliser les fonctions d'accès
     */
    typedef ImageConfig* ItemsMap;

    /**
     * @brief Configuration d'item
     */
    typedef struct{ 
        /**
         * @brief Map des items
         */
        ItemsMap map;
        /**
         * @brief Nombre d'items
         */
        int countOfItems;
    }ItemsConfig;

    /**
     * @brief Charge la configuration des tuiles de jeux
     * @param parser l'adresse du parseur initialisé
     * @param parentDirPath chemin du dossier de configuration parent
     * @return TillsConfig la structure des tuiles ou null en cas
     */
    void* loadItemsConfig(yaml_parser_t* parser, char* parentDirPath);

    /**
     * @brief Affiche la configuration des items
     * @param config la configuration
     * @param toPrintBefore chaine à afficher avant chaque ligne de configuration
     */
    void printItemsConfig(ItemsConfig* config,char* toPrintBefore);

    /**
     * @brief Libère les ressources de la configuration d'items
     * @param config configuration
     * @param freeContainer si true libère aussi le conteneur
     */
    void freeItemsConfig(ItemsConfig* config,bool freeContainer);
#endif
