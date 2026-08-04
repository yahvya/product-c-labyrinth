#include "./game-map-manager.h"
#include "custom-color.h"
#include <assert.h>

/**
 * @brief Libère les ressources et stoppe le parsing
 */
#define FREE_AND_QUIT yaml_token_delete(&token);\
    freeGameMapConfig(config,true);\
    return NULL;

/**
 * @brief Consume l'echelle
 * @param config la configuration
 * @param parser le parser
 * @return si la consumation réussie
 */
bool consumeScale(GameMapConfig* config,yaml_parser_t* parser){
    yaml_token_t token;

    while(true){
        if(!yaml_parser_scan(parser,&token)){
            fputs("\nEchec de récupération du token lors de la consumation de l'echelle",stderr);
            break;
        }

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        switch(token.type){
            case YAML_SCALAR_TOKEN:
                config->scale = atoi((char*)token.data.scalar.value);
                yaml_token_delete(&token);
                return true;

            default:;
        }

        yaml_token_delete(&token);
    }

    return false;
}

/**
 * @brief Consume les tills
 * @param config la configuration
 * @param parser le parser
 * @return si la consumation réussie
 */
bool consumeTills(GameMapConfig* config,yaml_parser_t* parser){
    // allocation de base
    config->tillsMapConfig.tillsMap = malloc(sizeof(GameMapTillConfig*));

    if(config->tillsMapConfig.tillsMap == NULL){
        fputs("\nEchec d'allocation de la map des tills sur la configuration de jeux",stderr);
        return false;
    }

    config->tillsMapConfig.countOfRows = config->tillsMapConfig.countOfCols = 0;

    // récupération des items
    yaml_token_t token;

    // nombre de blocs ouvert
    bool firstBlocEncountered = false;
    int blocTabCount = 0;
    int countOfCurrentCols = 0;
    int structValueCount = 0;
    void* tmpAddress;

    while(true){
        if(!yaml_parser_scan(parser,&token)){
            fputs("\nEchec de lecture du token sur la configuration de jeux lors de la récupération des tills",stderr);
            break;
        }

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        switch(token.type){
            case YAML_FLOW_SEQUENCE_START_TOKEN:
                if(!firstBlocEncountered)
                    firstBlocEncountered = true;

                blocTabCount++;

                switch(blocTabCount){
                    case 2:
                        // tab de nouvelle ligne
                        tmpAddress = realloc(
                            config->tillsMapConfig.tillsMap,
                            (config->tillsMapConfig.countOfRows + 1) * sizeof(GameMapTillConfig*)
                        );

                        if(tmpAddress == NULL){
                            fputs("\nEchec d'allocation d'une nouvelle ligne lors de la consumation de la map tills",stderr);
                            yaml_token_delete(&token);
                            return false;
                        }

                        config->tillsMapConfig.tillsMap = tmpAddress;
                        config->tillsMapConfig.countOfRows++;
                    break;

                    case 3:
                        // tab de nouvelle colonne
                        if(countOfCurrentCols != 0){
                            tmpAddress = realloc(
                                config->tillsMapConfig.tillsMap[config->tillsMapConfig.countOfRows - 1],
                                (countOfCurrentCols + 1) * sizeof(GameMapTillConfig)
                            );
                        }
                        else
                            tmpAddress = malloc(sizeof(GameMapTillConfig));

                        if(tmpAddress == NULL){
                            fputs("\nEchec de reallocation de colonne",stderr);
                            yaml_token_delete(&token);
                            return false;
                        }

                        config->tillsMapConfig.tillsMap[config->tillsMapConfig.countOfRows - 1] = tmpAddress;

                        countOfCurrentCols++;
                    break;

                    default:;
                }
            break;

            case YAML_FLOW_SEQUENCE_END_TOKEN:
                blocTabCount--;

                switch(blocTabCount){
                    case 2:
                        // fin d'une colonne
                        structValueCount = 0;
                    break;

                    case 1:
                        // fin d'une ligne
                        if(countOfCurrentCols > config->tillsMapConfig.countOfCols)
                            config->tillsMapConfig.countOfCols = countOfCurrentCols;

                        countOfCurrentCols = 0;
                    break;
                    default:;
                }
            break;

            case YAML_SCALAR_TOKEN:
                structValueCount++;

                switch(structValueCount){
                    // valeur x
                    case 1:
                        config->tillsMapConfig.tillsMap[config->tillsMapConfig.countOfRows - 1][countOfCurrentCols - 1].x = atoi((char*)token.data.scalar.value);
                    break;

                    case 2:
                        // valeur y
                        config->tillsMapConfig.tillsMap[config->tillsMapConfig.countOfRows - 1][countOfCurrentCols - 1].y = atoi((char*)token.data.scalar.value);
                    break;

                    case 3:
                        // valeur id
                        config->tillsMapConfig.tillsMap[config->tillsMapConfig.countOfRows - 1][countOfCurrentCols - 1].id = atoi((char*)token.data.scalar.value);
                    break;

                    default:;
                }
            break;

            default:;
        }

        yaml_token_delete(&token);

        if(firstBlocEncountered && blocTabCount == 0)
            break;
    }

    return true;
}

/**
 * @brief Consume les items
 * @param config la configuration
 * @param parser le parser
 * @return si la consumation réussie
 */
bool consumeItems(GameMapConfig* config,yaml_parser_t* parser){
    yaml_token_t token;

    GameMapItemConfig* newItemAddress = NULL;

    bool firstBlocEncountered = false;
    int blocMapCounter = 0;
    int structValueCounter = 0;

    while(true){
        if(!yaml_parser_scan(parser,&token)){
            fputs("\nEchec de recupération du token lors de la consumation des items",stderr);
            return false;
        }

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            return false;
        }

        switch(token.type){
            case YAML_FLOW_SEQUENCE_START_TOKEN:
                if(!firstBlocEncountered)
                    firstBlocEncountered = true;

                blocMapCounter++;
            break;

            case YAML_FLOW_SEQUENCE_END_TOKEN:
                blocMapCounter--;

                // ajout dans la liste de configuration
                if(blocMapCounter == 1){
                    structValueCounter = 0;

                    if(!listAppend(&config->itemsConfig,newItemAddress)){
                        fputs("\nEchec d'allocation de la configuration d'item",stderr);
                        yaml_token_delete(&token);
                        return false;
                    }

                    newItemAddress = NULL;
                }
            break;

            case YAML_SCALAR_TOKEN:
                if(blocMapCounter != 2)
                    break;

                // allocation d'une nouvelle configuration
                if(newItemAddress == NULL){
                    newItemAddress = malloc(sizeof(GameMapItemConfig));

                    if(newItemAddress == NULL){
                        fputs("\nEchec d'allocation de la configuration d'item",stderr);
                        yaml_token_delete(&token);
                        return false;
                    }
                }

                // remplissage de la structure
                structValueCounter++;

                switch(structValueCounter){
                    case 1:
                        newItemAddress->x = atoi((char*)token.data.scalar.value);
                    break;

                    case 2:
                        newItemAddress->y = atoi((char*)token.data.scalar.value);
                    break;

                    case 3:
                        newItemAddress->id = atoi((char*)token.data.scalar.value);
                    break;
                    default:;
                }
            break;

            default:;
        }

        yaml_token_delete(&token);

        if(firstBlocEncountered && blocMapCounter == 0)
            break;
    }

    return true;
}

/**
 * @brief Consume les ennemies
 * @param config la configuration
 * @param parser le parser
 * @return si la consumation réussie
 */
bool consumeEnemies(GameMapConfig* config,yaml_parser_t* parser){
    yaml_token_t token;

    GameMapEnemyConfig* newEnemyAddress = NULL;

    bool firstBlocEncountered = false;
    int blocMapCounter = 0;
    int structValueCounter = 0;

    while(true){
        if(!yaml_parser_scan(parser,&token)){
            fputs("\nEchec de recupération du token lors de la consumation des ennemies",stderr);
            return false;
        }

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            return false;
        }

        switch(token.type){
            case YAML_FLOW_SEQUENCE_START_TOKEN:
                if(!firstBlocEncountered)
                    firstBlocEncountered = true;

                blocMapCounter++;
                break;

            case YAML_FLOW_SEQUENCE_END_TOKEN:
                blocMapCounter--;

                // ajout dans la liste de configuration
                if(blocMapCounter == 1){
                    structValueCounter = 0;

                    if(!listAppend(&config->enemiesConfig, newEnemyAddress)){
                        fputs("\nEchec d'allocation de la configuration d'ennemie",stderr);
                        yaml_token_delete(&token);
                        return false;
                    }

                    newEnemyAddress = NULL;
                }
                break;

            case YAML_SCALAR_TOKEN:
                if(blocMapCounter != 2)
                    break;

                // allocation d'une nouvelle configuration
                if(newEnemyAddress == NULL){
                    newEnemyAddress = malloc(sizeof(GameMapEnemyConfig));

                    if(newEnemyAddress == NULL){
                        fputs("\nEchec d'allocation de la configuration d'ennemie",stderr);
                        yaml_token_delete(&token);
                        return false;
                    }
                }

                // remplissage de la structure
                structValueCounter++;

                switch(structValueCounter){
                    case 1:
                        newEnemyAddress->x = atoi((char*)token.data.scalar.value);
                        break;

                    case 2:
                        newEnemyAddress->y = atoi((char*)token.data.scalar.value);
                        break;

                    case 3:
                        newEnemyAddress->id = atoi((char*)token.data.scalar.value);
                        break;
                    default:;
                }
                break;

            default:;
        }

        yaml_token_delete(&token);

        if(firstBlocEncountered && blocMapCounter == 0)
            break;
    }

    return true;
}

void* loadGameMapConfig(yaml_parser_t* parser,char* parentDirPath){
    // allocation de la map
    GameMapConfig* config = malloc(sizeof(GameMapConfig));

    if(config == NULL){
        fputs("\nEchec d'allocation de la map de jeux",stderr);
        return NULL;
    }

    // pre allocation de la map de jeux
    config->tillsMapConfig.tillsMap = NULL;
    newGenericListFrom(&config->itemsConfig);
    newGenericListFrom(&config->enemiesConfig);

    int countOfKeysToLoad = 4;
    bool nextIsKey = false;

    yaml_token_t token;

    while(true){
        if(!yaml_parser_scan(parser,&token)){
            fputs("\nEchec de lecture du token lors de la lecture de configuration de map",stderr);
            freeGameMapConfig(config,true);
            return NULL;
        }

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        )
            break;

        switch(token.type){
            case YAML_KEY_TOKEN:
                nextIsKey = true;
            break;

            case YAML_SCALAR_TOKEN:
                if(!nextIsKey)
                    break;

                switch(countOfKeysToLoad){
                    case 4:
                        if(!consumeScale(config,parser)){
                            fputs("\nEchec de lecture de l'echelle",stderr);
                            FREE_AND_QUIT
                        }
                    break;

                    case 3:
                        if(!consumeTills(config,parser)){
                            fputs("\nEchec de lecture des tills",stderr);
                            FREE_AND_QUIT
                        }
                    break;

                    case 2:
                        if(!consumeItems(config,parser)){
                            fputs("\nEchec de lecture des items",stderr);
                            FREE_AND_QUIT
                        }
                    break;

                    case 1:
                        if(!consumeEnemies(config,parser)){
                            fputs("\nEchec de lecture des noms",stderr);
                            FREE_AND_QUIT
                        }
                    break;

                    default:;
                }

                nextIsKey = false;
                countOfKeysToLoad--;
            break;

            default:;
        }

        yaml_token_delete(&token);
    }

    if(countOfKeysToLoad != 0){
        freeGameMapConfig(config,true);
        return NULL;
    }

    return config;
}

void freeGameMapConfig(GameMapConfig* mapConfig,bool freeContainer){
    assert(mapConfig != NULL && "La map de jeux à libérer est NULL");

    if(mapConfig->tillsMapConfig.tillsMap != NULL){
        if(mapConfig->tillsMapConfig.countOfRows != 0){
            for(int rowIndex = 0; rowIndex < mapConfig->tillsMapConfig.countOfRows; rowIndex++)
                free(mapConfig->tillsMapConfig.tillsMap[rowIndex]);
        }

        free(mapConfig->tillsMapConfig.tillsMap);
    }

    freeGenericList(&mapConfig->itemsConfig,true);
    freeGenericList(&mapConfig->enemiesConfig,true);

    if(freeContainer)
        free(mapConfig);
}

void printMapConfig(GameMapConfig* config){
    assert(config != NULL && "La map de jeux à libérer est NULL");

    printf("\n"CC_BLUE"------------------------------------------------------------------------"CC_RESET"\n");
    printf("\n"CC_BBLUE"Configuration de map"CC_RESET"\n");
    printf("\n"CC_BG_BLUE"Echelle d'affichage : %d"CC_RESET"\n",config->scale);
    printf(CC_BLUE"------------------------------------------------------------------------"CC_RESET"\n");

//    map des items
    printf("\t"CC_BBLUE"Configuration des tills"CC_RESET"\n");
    printf("\n\t"CC_BG_BLUE"Lignes x Colonnes : %d x %d"CC_RESET"\n",
       config->tillsMapConfig.countOfRows,
       config->tillsMapConfig.countOfCols
   );

    for(int rowIndex = 0; rowIndex != config->tillsMapConfig.countOfRows; rowIndex++){
        printf("\n\t\t"CC_BBLUE"Ligne %d"CC_RESET"\n\t\t\t",rowIndex + 1);

        for(int colIndex = 0; colIndex < config->tillsMapConfig.countOfCols; colIndex++){
            printf("[x= %d,y= %d,id= %d] ",
               config->tillsMapConfig.tillsMap[rowIndex][colIndex].x,
               config->tillsMapConfig.tillsMap[rowIndex][colIndex].y,
               config->tillsMapConfig.tillsMap[rowIndex][colIndex].id
            );
        }
    }

    printf("\n"CC_BLUE"------------------------------------------------------------------------"CC_RESET"\n");
    printf("\t"CC_BBLUE"Configuration des items"CC_RESET"\n");

    while(config->itemsConfig.items != NULL){
        GameMapItemConfig* itemConfig = (GameMapItemConfig*) config->itemsConfig.items->data;

        printf("\t\t[x= %d,y= %d,id= %d]\n",
           itemConfig->x,
           itemConfig->y,
           itemConfig->id
        );

        config->itemsConfig.items = config->itemsConfig.items->nextItem;
    }

    config->itemsConfig.items = config->itemsConfig.listStart;

    printf("\n"CC_BLUE"------------------------------------------------------------------------"CC_RESET"\n");
    printf("\t"CC_BBLUE"Configuration des ennemies"CC_RESET"\n");

    while(config->enemiesConfig.items != NULL){
        GameMapEnemyConfig* enemyConfig = (GameMapEnemyConfig*) config->enemiesConfig.items->data;

        printf("\t\t[x= %d,y= %d,id= %d]\n",
            enemyConfig->x,
            enemyConfig->y,
            enemyConfig->id
        );

        config->enemiesConfig.items = config->enemiesConfig.items->nextItem;
    }

    config->enemiesConfig.items = config->enemiesConfig.listStart;

    printf("\n"CC_BLUE"------------------------------------------------------------------------"CC_RESET"\n");
}