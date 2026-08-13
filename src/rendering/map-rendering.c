#include "map-rendering.h"
#include "../assets-manager/config-manager/tills-manager.h"

bool renderMapFromConfig(GameConfig* gameConfig,GameMapConfig* mapConfig)
{
    // dessin des tills
    GameTillsMap tillsMapConfig = mapConfig->tillsMapConfig;

    for (int rows = 0; rows < tillsMapConfig.countOfRows; rows++)
    {
        for (int cols = 0; cols < tillsMapConfig.countOfCols; cols++)
        {
            // récupération de la bonne till
            GameMapTillConfig tillConfiguration = tillsMapConfig.tillsMap[rows][cols];

            ImageConfig* tillConfig = getTillImageConfigFromId(tillConfiguration.id, gameConfig->tillsConfig);

            if (tillConfig == NULL)
            {
                TraceLog(LOG_ERROR, "Till non trouvé id : %d", tillConfiguration.id);
                continue;
            }

            // rotation des images pour avoir l'animation visuelle
            GenericListItem* currentItem = tillConfig->linkedImages.items;

            if (currentItem == NULL)
            {
                currentItem = tillConfig->linkedImages.items = tillConfig->linkedImages.listStart;
            }

            if (currentItem == NULL || currentItem->data == NULL)
            {
                TraceLog(LOG_ERROR, "Texture non chargée");
                continue;
            }

            // dessin
            Texture2D* texture = (Texture2D*) currentItem->data;
            Vector2 tillPosition = {
                .x = (float) tillConfiguration.x,
                .y = (float) tillConfiguration.y
            };

            DrawTextureEx(*(texture), tillPosition, (float) tillConfig->rotation, (float) mapConfig->scale, WHITE);

            // avance du curseur vers la prochaine frame d'animation, bouclage en fin de liste
            tillConfig->linkedImages.items = currentItem->nextItem != NULL ? currentItem->nextItem : tillConfig->linkedImages.listStart;
        }
    }

    return true;
}