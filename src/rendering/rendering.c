#include "rendering.h"

bool renderImageFromConfig(ImageConfig* imageConfig, const GameMapConfig* mapConfig, const Vector2 texturePosition)
{
    // rotation des images pour avoir l'animation visuelle
    const GenericListItem* currentItem = imageConfig->linkedImages.items;

    if (currentItem == NULL)
    {
        currentItem = imageConfig->linkedImages.items = imageConfig->linkedImages.listStart;
    }

    if (currentItem == NULL || currentItem->data == NULL)
    {
        TraceLog(LOG_ERROR, "Texture non chargée");
        return false;
    }

    // dessin
    const Texture2D* texture = (Texture2D*)currentItem->data;

    DrawTextureEx(*texture, texturePosition, (float)imageConfig->rotation, (float)mapConfig->scale, WHITE);

    // avance du curseur vers la prochaine frame d'animation, bouclage en fin de liste
    imageConfig->linkedImages.items = currentItem->nextItem != NULL
                                          ? currentItem->nextItem
                                          : imageConfig->linkedImages.listStart;

    return true;
}
