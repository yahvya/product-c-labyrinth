#if !defined(GAME_WINDOW_MANAGER_H)
#define GAME_WINDOW_MANAGER_H
    #include <stdbool.h>

    /**
     * @brief Configuration de rendu
     */
    typedef struct{
        /**
         * @brief Données à transmettre
         */
        void* data;
    }RenderingConfig;

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
    bool renderWindow(RenderingConfig* renderingConfig,bool (*customRenderingFunction)(RenderingConfig*));
#endif