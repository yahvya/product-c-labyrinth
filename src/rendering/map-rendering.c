#include "map-rendering.h"
#include "../assets-manager/config-manager/items-manager.h"
#include "../assets-manager/config-manager/tills-manager.h"

bool drawItems(const GameConfig* gameConfig, const ItemsConfig* itemsConfig)
{
    return true;
}

bool drawTills(const GameConfig* gameConfig, const GameMapConfig* mapConfig)
{
    const GameTillsMap tillsMapConfig = mapConfig->tillsMapConfig;

    for (int rows = 0; rows < tillsMapConfig.countOfRows; rows++)
    {
        for (int cols = 0; cols < tillsMapConfig.countOfCols; cols++)
        {
            // récupération de la bonne till
            const GameMapTillConfig tillConfiguration = tillsMapConfig.tillsMap[rows][cols];

            ImageConfig* tillConfig = getTillImageConfigFromId(tillConfiguration.id, gameConfig->tillsConfig);

            if (tillConfig == NULL)
            {
                TraceLog(LOG_ERROR, "Till non trouvé id : %d", tillConfiguration.id);
                continue;
            }

            // rotation des images pour avoir l'animation visuelle
            const GenericListItem* currentItem = tillConfig->linkedImages.items;

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
            const Texture2D* texture      = (Texture2D*)currentItem->data;
            const Vector2    tillPosition = {
                .x = (float)tillConfiguration.x,
                .y = (float)tillConfiguration.y
            };

            DrawTextureEx(*texture, tillPosition, (float)tillConfig->rotation, (float)mapConfig->scale, WHITE);

            // avance du curseur vers la prochaine frame d'animation, bouclage en fin de liste
            tillConfig->linkedImages.items = currentItem->nextItem != NULL
                                                 ? currentItem->nextItem
                                                 : tillConfig->linkedImages.listStart;
        }
    }

    return true;
}

bool renderMapFromConfig(const GameConfig* gameConfig, const GameMapConfig* mapConfig)
{
    return drawTills(gameConfig, mapConfig) &&
        drawItems(gameConfig, gameConfig->itemsConfig);
}
