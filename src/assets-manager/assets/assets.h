#if !defined(ASSETS_MANAGER_ASSETS_H)
#define ASSETS_MANAGER_ASSETS_H
    /**
     * @brief Chemin racine du dossier ressource
     */
    #define RESOURCES_PATH "./resources"

    /**
     * @brief Chemin du fichier de configuration des ennemies
     */
    #define ENEMIES_CONFIG_FILE_PATH RESOURCES_PATH"/game-assets/enemies/config.yaml"

    /**
     * @brief Chemin du fichier de configuration des héros
     */
    #define HEROES_CONFIG_FILE_PATH RESOURCES_PATH"/game-assets/heroes/config.yaml"

    /**
     * @brief Chemin du fichier de configuration des items
     */
    #define ITEMS_CONFIG_FILE_PATH RESOURCES_PATH"/game-assets/items/config.yaml"

    /**
     * @brief Chemin du fichier de configuration des tills
     */
    #define TILLS_CONFIG_FILE_PATH RESOURCES_PATH"/game-assets/tills/config.yaml"

    /**
     * @brief Chemin du fichier de configuration de la map de page d'accueil
     */
    #define HOME_MAP_CONFIG_FILE_PATH RESOURCES_PATH"/home-config/map.yaml"

    /**
     * @brief Longueur maximale supposée d'un chemin
     */
    #define SUPPOSED_PATH_MAX_LEN 300

    #include <stddef.h>

    /**
     * @brief Extrait le chemin de dossier en supprimant le dernier caractère de séparation rencontré (/ ou \)
     * @param fullPath chemin
     * @param buffer à remplir de taille SUPPOSED_PATH_MAX_LEN au minimum
     * @param bufferSize taille du buffer
     */
    void extractDirPathFrom(char* fullPath,char* buffer,size_t bufferSize);
#endif
