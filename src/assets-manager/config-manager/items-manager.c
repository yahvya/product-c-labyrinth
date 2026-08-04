#include "./items-manager.h"
#include "../assets/assets.h"
#include <assert.h>
#include "custom-color.h"

/**
 * @brief Macro de libération des ressources de parsing
 */
#define FREE_RESOURCES_AND_QUIT freeItemsConfig(config,true);\
    yaml_token_delete(&token);\
    return NULL;

/**
 * @brief Ternaire de choix de l'affichage
 */
#define TO_PRINT toPrintBefore == NULL ? "" : toPrintBefore

/**
 * @brief Taille du buffer d'écart
 */
#define SPACE_BUFFER_SIZE 20

void* loadItemsConfig(yaml_parser_t* parser, char* parentDirPath){
    assert(parser != NULL && "Le parser fourni pour la lecture de configuration des items est NULL");
    assert(parentDirPath != NULL && "Le chemin parent fourni pour la lecture de configuration des items est NULL");
    assert(strlen(parentDirPath) < SUPPOSED_PATH_MAX_LEN && "La longueur du chemin fourni est supérieur à celle supposée sur le chargement des items");

    // allocation de base des données items et de map
    ItemsConfig* config = malloc(sizeof(ItemsConfig));

    if(config == NULL){
        fputs("\nEchec d'allocation de la configuration d'items",stderr);
        return NULL;
    }

    config->countOfItems = 0;
    config->map = malloc(sizeof(ImageConfig));

    if(config->map == NULL){
        free(config);
        fputs("\nEchec d'allocation le map de la configuration d'items",stderr);
        return NULL;
    }

    // parsing des items
    yaml_token_t token;
    int keyGetCount = 0;
    bool nextIsValue = false;
    char lastId[SUPPOSED_ID_MAX_LEN];

    while(true){
        if(!yaml_parser_scan(parser,&token)){
            fputs("\nEchec de lecture de token lors du parsing de configuration des items\n",stderr);
            FREE_RESOURCES_AND_QUIT
        }

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        switch(token.type){
            case YAML_KEY_TOKEN:
                keyGetCount++;
            break;

            case YAML_VALUE_TOKEN:
                if(keyGetCount == 2)
                    nextIsValue = true;
            break;

            case YAML_SCALAR_TOKEN:
                if(keyGetCount == 1){
                    // agrandissement d'allocation de map
                    void* tmpAddress = realloc(config->map,sizeof(ImageConfig) * (config->countOfItems + 1));

                    if(tmpAddress == NULL){
                        fputs("\nEchec de reallocation d'adresse lors du parsing de configuration d'items\n",stderr);
                        FREE_RESOURCES_AND_QUIT
                    }

                    config->countOfItems++;
                    config->map = tmpAddress;

                    // stockage de l'id
                    memset(lastId,0,sizeof(char) * SUPPOSED_ID_MAX_LEN);
                    strncpy(lastId,(char*) token.data.scalar.value,sizeof(char) * SUPPOSED_ID_MAX_LEN);
                }
                else if(nextIsValue){
                    nextIsValue = false;

                    // récupération du chemin d'élément suivant
                    keyGetCount = 0;

                    char configFilePath[SUPPOSED_PATH_MAX_LEN];
                    memset(configFilePath, 0, SUPPOSED_PATH_MAX_LEN);
                    strncpy(configFilePath, parentDirPath, sizeof(char) * (SUPPOSED_PATH_MAX_LEN - 1));
                    strncat(
                            configFilePath,
                            (char*) token.data.scalar.value,
                        sizeof(char) * (SUPPOSED_PATH_MAX_LEN - strlen(parentDirPath) - 1)
                    );

                    // ouverture de la configuration
                    yaml_parser_t internalParser;

                    if(!yaml_parser_initialize(&internalParser)){
                        fprintf(stderr,"Echec d'initialisation du parser yaml lors de la configuration des items");
                        FREE_RESOURCES_AND_QUIT
                    }

                    FILE* file = fopen(configFilePath,"r");
                    char configParentDirPath[SUPPOSED_PATH_MAX_LEN];

                    extractDirPathFrom(configFilePath,configParentDirPath,SUPPOSED_PATH_MAX_LEN);

                    if(file == NULL){
                        fprintf(stderr,"Echec d'ouverture de la configuration sur le chemin suivant <%s> lors de la configuration des items",configFilePath);
                        yaml_parser_delete(&internalParser);
                        FREE_RESOURCES_AND_QUIT
                    }

                    // parsing de la configuration
                    yaml_parser_set_input_file(&internalParser,file);

                    ImageConfig createdImage = createImageFromConfig(&internalParser,configParentDirPath);

                    yaml_parser_delete(&internalParser);

                    if(createdImage.errorState){
                        fputs("\nEchec de parsing de la configuration d'image lors du parsing de configuration des items\n",stderr);
                        fclose(file);
                        FREE_RESOURCES_AND_QUIT
                    }

                    // stockage des données dans la map
                    strcpy(createdImage.id,lastId);
                    memcpy(config->map + (config->countOfItems - 1),&createdImage,sizeof(ImageConfig));

                    fclose(file);
                }
            break;

            default:;
        }

        yaml_token_delete(&token);
    }

    return config;
}

void printItemsConfig(ItemsConfig * config,char* toPrintBefore){
    assert(config != NULL && "Configuration d'items NULL pour l'affichage");

    printf("\n"CC_BLUE"%s------------------------------------------------------------------------"CC_RESET,TO_PRINT);
    printf("\n"CC_BBLUE"%sConfiguration des items"CC_RESET"\n",TO_PRINT);
    printf("\n"CC_BG_BLUE"%sNombre d'elements : %d"CC_RESET"\n",TO_PRINT,config->countOfItems);
    printf("\n"CC_BG_BLUE"%sListe des elements"CC_RESET"\n",TO_PRINT);
    printf(CC_BLUE"%s------------------------------------------------------------------------"CC_RESET"\n",TO_PRINT);

    char spaceBuffer[SPACE_BUFFER_SIZE];
    memset(spaceBuffer,0,sizeof(char) * SPACE_BUFFER_SIZE);

    if(toPrintBefore != NULL)
        strcpy(spaceBuffer,toPrintBefore);

    strncat(
            spaceBuffer,
            "\t",
            sizeof(char) * (SPACE_BUFFER_SIZE - (toPrintBefore != NULL ? strlen(toPrintBefore) : 0))
    );

    for(int i = 0; i < config->countOfItems; i++){
        printImageConfig(&config->map[i],spaceBuffer);
        printf("\n");
    }

    printf("\n"CC_BLUE"------------------------------------------------------------------------"CC_RESET"\n");
}

void freeItemsConfig(ItemsConfig* config,bool freeContainer){
    assert(config != NULL && "Configuration d'image NULL pour la libération");

    for(int i = 0; i < config->countOfItems; i++)
        freeImageConfig(&config->map[i],false);

    free(config->map);

    if(freeContainer)
        free(config);
}
