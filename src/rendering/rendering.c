#include "rendering.h"

bool renderImageFromConfig(ImageConfig* imageConfig, const GameMapConfig* mapConfig, Vector2 texturePosition, const int ticksPerFrame)
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

    texturePosition.x = texturePosition.x * mapConfig->scale;
    texturePosition.y = texturePosition.y * mapConfig->scale;
        
    DrawTextureEx(*texture, texturePosition, (float)imageConfig->rotation, (float)mapConfig->scale, WHITE);

    // avance du curseur vers la prochaine frame d'animation (au rythme de ticksPerFrame
    // rendus par frame, pas à chaque rendu), bouclage en fin de liste
    imageConfig->animationTicks++;

    if (imageConfig->animationTicks >= ticksPerFrame)
    {
        imageConfig->animationTicks = 0;

        imageConfig->linkedImages.items = currentItem->nextItem != NULL
                                              ? currentItem->nextItem
                                              : imageConfig->linkedImages.listStart;
    }

    return true;
}
