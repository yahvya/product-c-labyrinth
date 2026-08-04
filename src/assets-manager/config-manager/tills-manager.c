#include "./tills-manager.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "custom-color.h"
#include "../assets/assets.h"

/**
 * @brief Taille du buffer d'écart
 */
#define SPACE_BUFFER_SIZE 20

/**
 * @brief Ternaire de choix de l'affichage
 */
#define TO_PRINT toPrintBefore == NULL ? "" : toPrintBefore

/**
 * @brief Libère les ressources de la boucle de traitement avant de quitter
 */
#define FREE_RESOURCES_AND_QUIT freeTillsConfig(config,true);\
    yaml_token_delete(&readToken);\
    return NULL;

void* loadTillsConfig(yaml_parser_t* parser,char* parentDirPath){
    assert(parser != NULL && "Le parser fourni pour la lecture de configuration des tills est NULL");
    assert(parentDirPath != NULL && "Le chemin parent fourni pour la lecture de configuration des tills est NULL");
    assert(strlen(parentDirPath) < SUPPOSED_PATH_MAX_LEN && "La longueur du chemin fourni est supérieur à celle supposée sur le chargement des items");

    // allocation de base de la configuration
    TillsConfig* config = malloc(sizeof(TillsConfig)); 

    if(config == NULL){
        fputs("\nEchec d'allocation de la configuration des tills\n",stderr);
        return NULL;
    }

    config->countOfTills = 0;

    // allocation de base de la map
    config->map = malloc(sizeof(ImageConfig));

    if(config->map == NULL){
        fputs("\nEchec d'allocation de la map des tills\n",stderr);
        free(config);
        return NULL;
    }

    // parsing du fichier
    yaml_token_t readToken;
    bool nextIsKey = false;

    while(true){
        if(!yaml_parser_scan(parser,&readToken)){
            fputs("\nEchec de lecture de token lors du parsing de configuration de tills\n",stderr);
            FREE_RESOURCES_AND_QUIT
        }

        if(
            readToken.type == YAML_DOCUMENT_END_TOKEN ||
            readToken.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&readToken);
            break;
        }

        switch(readToken.type){
            case YAML_KEY_TOKEN:
                nextIsKey = true;
            break;

            case YAML_SCALAR_TOKEN:
                if(nextIsKey){
                    // agrandissement de l'espace alloué
                    void* tmpAddress = realloc(config->map,sizeof(ImageConfig) * (config->countOfTills + 1));

                    if(tmpAddress == NULL){
                        fputs("\nEchec de reallocation d'adresse lors du parsing de configuration de tills\n",stderr);
                        FREE_RESOURCES_AND_QUIT
                    }

                    // création de la configuration
                    config->countOfTills++;
                    config->map = tmpAddress;
                    ImageConfig createdImage = createImageFromConfig(parser, parentDirPath);
                    
                    if(createdImage.errorState){
                        fputs("\nEchec de parsing de la configuration d'image lors du parsing de configuration de tills\n",stderr);
                        FREE_RESOURCES_AND_QUIT
                    }

                    strncpy(createdImage.id,(char*)readToken.data.scalar.value,sizeof(char) * (SUPPOSED_ID_MAX_LEN - 1));
                    memcpy(config->map + (config->countOfTills - 1),&createdImage,sizeof(ImageConfig));

                    // attente de la clé suivante
                    nextIsKey = false;
                }
            break;

            default:;
        }
    
        yaml_token_delete(&readToken);
    }

    return config;
}

void freeTillsConfig(TillsConfig* config,bool freeContainer){
    assert(config != NULL && "Configuration d'image NULL pour la libération");

    for(int i = 0; i < config->countOfTills; i++)
        freeImageConfig(&config->map[i],false);

    free(config->map);

    if(freeContainer)
        free(config);
}

void printTillsConfig(TillsConfig* config,char* toPrintBefore){
    assert(config != NULL && "Configuration de tills NULL pour l'affichage");

    printf(CC_BLUE"\n%s------------------------------------------------------------------------\n",TO_PRINT);
    printf(CC_BBLUE"\n%sConfiguration de tills\n"CC_RESET,TO_PRINT);
    printf(CC_BG_BLUE"\n%sNombre d'elements : %d\n",TO_PRINT,config->countOfTills);
    printf("\n%sListe des elements\n"CC_RESET,TO_PRINT);
    printf(CC_BLUE"%s------------------------------------------------------------------------\n"CC_RESET,TO_PRINT);

    char spaceBuffer[SPACE_BUFFER_SIZE];
    memset(spaceBuffer,0,sizeof(char) * SPACE_BUFFER_SIZE);

    if(toPrintBefore != NULL)
        strcpy(spaceBuffer,toPrintBefore);
    
    strncat(
        spaceBuffer,
        "\t",
        sizeof(char) * (SPACE_BUFFER_SIZE - (toPrintBefore != NULL ? strlen(toPrintBefore) : 0))
    );

    for(int i = 0; i < config->countOfTills; i++){
        printImageConfig(&config->map[i],toPrintBefore);
        printf("\n");
    }

    printf(CC_BLUE"------------------------------------------------------------------------"CC_RESET"\n");
}
