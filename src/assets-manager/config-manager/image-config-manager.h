#if !defined(ASSETS_MANAGER_CONFIG_MANAGER_IMAGE_CONFIG_MANAGER_H)
#define ASSETS_MANAGER_CONFIG_MANAGER_IMAGE_CONFIG_MANAGER_H
    /**
     * @brief Longueur supposée d'une description d'élément
     */
    #define SUPPOSED_DESCRIPTION_MAX_LEN 300
    
    /**
     * @brief Longueur supposée d'un id d'élément
     */
    #define SUPPOSED_ID_MAX_LEN 15

    #include "../../utils/generic-list/generic-list.h"
    #include <yaml.h>

        typedef enum{
        /**
         * @brief Image fixe
         */
        STATIC=1,
        /**
         * @brief Images à jouer de manière continue
         */
        CONTINIOUS_IMAGES,
        /**
         * @brief Images à jouer suite à une action
         */
        IMAGES_ON_ACTION
    }ImageType;

    /**
     * @brief Configuration d'image
     */
    typedef struct{
        /**
         * @brief Liste des chemins ordonnés des images
         * char*
         * @attention état d'erreur à true par défaut tant que les valeurs ne sont pas chargées
         */
        GenericList paths;

        /**
         * @brief Liste des images chargées raylib
         * Texture2D
         * @attention état d'erreur à true par défaut tant que les valeurs ne sont pas chargées
         */
        GenericList linkedImages;

        /**
         * @brief Id possible de l'image
         */
        char id[SUPPOSED_ID_MAX_LEN];

        /**
         * @brief Description de l'item
         */
        char description[SUPPOSED_DESCRIPTION_MAX_LEN];
        /**
         * @brief Type d'image
         */
        ImageType type;
        /**
         * @brief Valeur de rotation
         */
        int rotation;
        /**
         * @brief Etat d'erreur
         */
        bool errorState;
    }ImageConfig;

    /**
     * @brief Crée une configuration d'image à partir de la configuration
     * @param parser élément parser
     * @param parentDirPath chemin du dossier parent de la configuration
     * @return la configuration crée
     * @attention vérifier l'état d'erreur avant toute utilisation
     */
    ImageConfig createImageFromConfig(yaml_parser_t* parser,char* parentDirPath);

    /**
     * @brief Lance le chargement des images raylib contenues
     * @param config
     * @return
     */
    bool loadLinkedImages(ImageConfig* config);

    /**
     * @brief Libère les ressources de la configuration
     * @param config la configuration
     * @param freeContainer si true libère le conteneur fourni
     */
    void freeImageConfig(ImageConfig* config,bool freeContainer);

    /**
     * @brief Affiche le détails de la configuration
     * @param config configuration
     * @param toPrintBefore Chaine à afficher avant chaque ligne de configuration
     */
    void printImageConfig(ImageConfig* config,char* toPrintBefore);
#endif
