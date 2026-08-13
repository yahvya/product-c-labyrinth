#include "map-rendering.h"

bool renderMapFromConfig(GameConfig* gameConfig,GameMapConfig* mapConfig)
{
    // dessin des tills
    GameTillsMap tillsMapConfig = mapConfig->tillsMapConfig;

    for (int rows = 0; rows < tillsMapConfig.countOfRows; rows++)
    {
        for (int cols = 0; cols < tillsMapConfig.countOfCols; cols++)
        {
            GameMapTillConfig tillConfiguration = tillsMapConfig.tillsMap[rows][cols];
        }
    }

    return true;
}