#include "map-rendering.h"
#include "./rendering.h"
#include "../assets-manager/config-manager/items-manager.h"
#include "../assets-manager/config-manager/tills-manager.h"

bool drawItems(const GameConfig* gameConfig, const GameMapConfig* mapConfig)
{
    const GenericList itemsConfiguration = mapConfig->itemsConfig;

    if (itemsConfiguration.errorState)
    {
        TraceLog(LOG_ERROR, "Erreur lors du chargement des items, la liste a un statut d'erreur à true");
        return false;
    }

    const GenericListItem* currentNode = itemsConfiguration.items;

    while (currentNode != NULL)
    {
        const GameMapItemConfig* itemConfig      = (GameMapItemConfig*)currentNode->data;
        ImageConfig*             itemConfigImage = getItemImageConfigFromId(itemConfig->id, gameConfig->itemsConfig);

        if (itemConfigImage == NULL)
        {
            TraceLog(LOG_ERROR, "Item non trouvé id : %d", itemConfig->id);
            currentNode = currentNode->nextItem;
            continue;
        }

        const Vector2 tillPosition = {
            .x = (float)itemConfig->x,
            .y = (float)itemConfig->y
        };

        renderImageFromConfig(itemConfigImage, mapConfig, tillPosition);

        currentNode = currentNode->nextItem;
    }

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

            const Vector2 tillPosition = {
                .x = (float)tillConfiguration.x,
                .y = (float)tillConfiguration.y
            };

            renderImageFromConfig(tillConfig, mapConfig, tillPosition);
        }
    }

    return true;
}

bool renderMapFromConfig(const GameConfig* gameConfig, const GameMapConfig* mapConfig)
{
    return drawTills(gameConfig, mapConfig) &&
        drawItems(gameConfig, mapConfig);
}
