#include "./assets.h"
#include <assert.h>
#include <string.h>

void extractDirPathFrom(char* fullPath,char* buffer,size_t bufferSize){
    assert(fullPath != NULL && "Le chemin d'accès fourni est NULL");
    assert(buffer != NULL && "Le buffer fourni est NULL");
    assert(bufferSize >= (sizeof(char) * SUPPOSED_PATH_MAX_LEN) && "La taille de buffer fourni est invalide");

    // suppression du contenu du buffer
    memset(buffer,0,bufferSize);

    // recherche du caractère de séparation
    char* lastPos = strrchr(fullPath,'/');

    if(lastPos == NULL)
        lastPos = strrchr(fullPath,'\\');

    // définition forcée de la dernière position comme position du séparateur
    if(lastPos == NULL)
        lastPos = fullPath + (strlen(fullPath) - 1);

    memcpy(buffer,fullPath,sizeof(char) * (lastPos - fullPath));
}
