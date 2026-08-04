#if !defined(UTILS_GENERIC_LIST_GENERIC_LIST_H)
#define UTILS_GENERIC_LIST_GENERIC_LIST_H
    #include <stdbool.h>

    /**
     * @brief Liste chainée générique (items)
     */
    typedef struct GenericListItem{
        /**
         * @brief Donnée
         */
        void* data;

        /**
         * @brief Item suivant
         */
        struct GenericListItem* nextItem;
    }GenericListItem;

    /**
     * @brief Liste chainée générique (gestionnaire)
     */
    typedef struct{
        /**
         * @brief Liste d'éléments
         */
        GenericListItem* items;

        /**
         * @brief Tête de liste
         */
        GenericListItem* listStart;

        /**
         * @brief Tête de liste
         */
        GenericListItem* listEnd;

        /**
         * @brief Etat d'erreur de la dernière action sur la liste
         */
        bool errorState;
    }GenericList;

    /**
     * @brief Crée une nouvelle liste générique
     * @attention Attention à l'utilisation si dynamique
     * @return La liste générique
     */
    GenericList newGenericList();

    /**
     * @brief Initalise le contenu d'une liste comme nouvelle
     * @param list la liste
     */
    void newGenericListFrom(GenericList* list);

    /**
     * @brief Ajoute un item en fin de liste
     * @param listManager Gestionnaire de liste dans lequel ajouter
     * @param data donnée à ajouter
     * @return si l'ajout réussi
     * @attention Met à jour l'état d'erreur de la liste
     */
    bool listAppend(GenericList* listManager,void* data);

    /**
     * @brief Ajoute un item en début de liste
     * @param listManager Gestionnaire de liste dans lequel ajouter
     * @param data donnée à ajouter
     * @return si l'ajout réussi
     * @attention Met à jour l'état d'erreur de la liste
     */
    bool listPrepend(GenericList* listManager,void* data);

    /**
     * @brief Libère le contenu de la liste. La liste est re initialisé et est utilisable par les fonctions de liste
     * @param list la liste
     * @param freeContainedData si true libère la valeur contenue
     */
    void freeGenericList(GenericList* list,bool freeContainedData);
#endif
