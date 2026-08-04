#include "./enemies-manager.h"
#include "../assets/assets.h"
#include <string.h>
#include <assert.h>
#include "custom-color.h"

/**
 * @brief Ternaire de choix de l'affichage
 */
#define TO_PRINT toPrintBefore == NULL ? "" : toPrintBefore

/**
 * @brief Taille du buffer d'écart
 */
#define SPACE_BUFFER_SIZE 20

/**
 * @brief Libère les ressources et stoppe
 */
#define FREE_RESOURCES_ANQ_QUIT freeEnemiesConfig(config,true);\
    yaml_token_delete(&token);\
    return NULL;

/**
 * @brief Libère les ressources de consumation et quitte
 */
#define FREE_CONSUME_AND_QUIT freeEnemyConfig(&enemy,false);\
    yaml_token_delete(token);\
    return NULL;

EnemyAction getEnemyActionIndexFromName(char* name){
    static char enemyActionsMap[ENEMY_MAX_FOR_ARRAY_KEYS][SUPPOSED_ENEMY_ACTION_MAX_LEN] = {
        "front-walk",
        "right-walk",
        "left-walk",
        "back-walk"
    };

    for(int i = 0; i < ENEMY_MAX_FOR_ARRAY_KEYS; i++){
        if(strcmp(enemyActionsMap[i],name) == 0)
            return i;
    }

    return -1;
}

/**
 * @brief Charge les actions du héro dans la configuration fournie
 * @param enemyConfig configuration de l'ennemie
 * @param configFilePath chemin de configuration des actions
 * @return si le chargement réussi
 */
bool loadEnemyActions(EnemyConfig* enemyConfig, char* configFilePath){
    assert(enemyConfig != NULL && "La configuration ennemie fournie pour le chargement des actions est NULL");
    assert(configFilePath != NULL && "Le chemin fournie pour le chargement des actions est NULL");

    char parentDirPath[SUPPOSED_PATH_MAX_LEN];

    extractDirPathFrom(configFilePath,parentDirPath,sizeof(char) * SUPPOSED_PATH_MAX_LEN);

    yaml_parser_t parser;

    if(!yaml_parser_initialize(&parser)){
        fputs("Echec d'initialisation du parser pour le chargement des actions ennemie",stderr);
        return false;
    }

    // ouverture du fichier pour parsing
    FILE* file = fopen(configFilePath,"r");

    if(file == NULL){
        fprintf(stderr,"Echec d'ouverture du fichier de configuration <%s>",configFilePath);
        return false;
    }

    yaml_parser_set_input_file(&parser,file);

    // parsing du fichier
    yaml_token_t token;

    int countOfElementsToGet = ENEMY_MAX_FOR_ARRAY_KEYS;
    bool nextIsKey = false;
    bool stop = false;

    while(countOfElementsToGet != 0 && !stop){
        if(!yaml_parser_scan(&parser,&token)){
            fputs("Echec de récupération du token lors du chargement des actions ennemie",stderr);
            return false;
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
                nextIsKey = true;
                break;

            case YAML_SCALAR_TOKEN:
                if(!nextIsKey)
                    break;

                nextIsKey = false;

                // récupération de l'index de l'action

                int actionKey = getEnemyActionIndexFromName((char*)token.data.scalar.value);

                if(actionKey == -1){
                    fprintf(stderr,"\nL'indice de l'action <%s> n'a pas été trouvé, lors du chargement des actions ennemie",(char*)token.data.scalar.value);
                    stop = true;
                    break;
                }

                // chargement de la configuration d'image de l'action et enregistrement
                ImageConfig createdImage = createImageFromConfig(&parser,parentDirPath);

                if(createdImage.errorState){
                    fputs("Echec de chargement d'une image lors du chargement des actions",stderr);
                    stop = true;
                    break;
                }

                memset(enemyConfig->actionsMap[actionKey].actionName,0,sizeof(char) * SUPPOSED_ENEMY_ACTION_MAX_LEN);
                strncpy(
                    enemyConfig->actionsMap[actionKey].actionName,
                    (char*)token.data.scalar.value,
                    sizeof(char) * (SUPPOSED_ENEMY_ACTION_MAX_LEN - 1)
                );
                enemyConfig->actionsMap[actionKey].framesConfig = malloc(sizeof(ImageConfig));

                if(enemyConfig->actionsMap[actionKey].framesConfig == NULL){
                    fputs("Echec d'allocation d'une image lors du chargement des actions",stderr);
                    stop = true;
                    break;
                }
                memcpy(enemyConfig->actionsMap[actionKey].framesConfig,&createdImage,sizeof(ImageConfig));
                countOfElementsToGet--;
                break;

            default:;
        }

        yaml_token_delete(&token);
    }

    yaml_parser_delete(&parser);
    fclose(file);

    return countOfElementsToGet == 0;
}

/**
 * @brief Consume une configuration dans et l'ajoute dans la configuration
 * @param parser parser
 * @param token token
 * @param config configuration (alloué attendu)
 * @param parentDirPath chemin du dossier parent
 * @return si la création réussi
 */
bool consumeEnemyIn(yaml_parser_t* parser,yaml_token_t* token,EnemiesConfig * config,char* parentDirPath){
    assert(parser != NULL && "Le parser fourni pour la consumation des données ennemie est NULL");
    assert(token != NULL && "Le token fourni pour la consumation des données ennemie est NULL");
    assert(config != NULL && "La configuration hero fournie pour la consumation des données hero est NULL");
    assert(parentDirPath != NULL && "Le chemin fournie pour la consumation des données hero est NULL");

    int keyCount = 0;
    EnemyConfig enemy;

    do{
        char configFilePath[SUPPOSED_PATH_MAX_LEN];

        if(
            token->type == YAML_DOCUMENT_END_TOKEN ||
            token->type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(token);
            return false;
        }

        if(
            token->type == YAML_KEY_TOKEN ||
            token->type == YAML_VALUE_TOKEN
        )
            keyCount++;

        switch(token->type){
            case YAML_SCALAR_TOKEN:
                switch(keyCount){
                    case 1:
                        // récupération de l'id
                        memset(enemy.id,0,sizeof(char) * SUPPOSED_ENEMY_ID_MAX_LEN);
                        strncpy(enemy.id,(char*) token->data.scalar.value,sizeof(char) * (SUPPOSED_ENEMY_ID_MAX_LEN - 1));
                        break;

                    case 4:
                        // récupération du chemin
                        memset(configFilePath, 0, SUPPOSED_PATH_MAX_LEN);
                        strncpy(configFilePath, parentDirPath, sizeof(char) * (SUPPOSED_PATH_MAX_LEN - 1));
                        strncat(
                            configFilePath,
                            (char*) token->data.scalar.value,
                            sizeof(char) * (SUPPOSED_PATH_MAX_LEN - strlen(parentDirPath) - 1)
                        );

                        if(!loadEnemyActions(&enemy,configFilePath)){
                            fputs("\nEchec du chargement des actions hero",stderr);
                            FREE_CONSUME_AND_QUIT
                        }
                        break;

                    case 6:
                        // récupération du nom
                        memset(enemy.name,0,sizeof(char) * SUPPOSED_ENEMY_NAME_MAX_LEN);
                        strncpy(enemy.name,(char*) token->data.scalar.value,sizeof(char) * (SUPPOSED_ENEMY_NAME_MAX_LEN - 1));
                        yaml_token_delete(token);

                        // allocation de l'espace de stockage
                        void* tmpAddress = realloc(config->map,sizeof(EnemyConfig) * (config->countOfEnemies + 1));

                        if(tmpAddress == NULL){
                            fputs("\nEchec de reallocation de la liste des heros",stderr);
                            FREE_CONSUME_AND_QUIT
                        }

                        // ajout de l'ennemie dans la map
                        config->countOfEnemies++;
                        config->map = tmpAddress;
                        memcpy(config->map + (config->countOfEnemies - 1),&enemy,sizeof(EnemyConfig));

                        return true;

                    default:;
                }
                ; break;

            default:;
        }

        yaml_token_delete(token);

        if(!yaml_parser_scan(parser,token)){
            fputs("\nEchec de récupération de token, lors de consumation d'un héro",stderr);
            return false;
        }
    }while(true);
}

void* loadEnemies(yaml_parser_t* parser,char* parentDirPath){
    assert(parser != NULL && "Le parseur fourni est null pour la configuration des ennemies");
    assert(parentDirPath != NULL && "Le chemin fourni est null pour la configuration des ennemies");

    // allocation des éléments
    EnemiesConfig* config = malloc(sizeof(EnemiesConfig));

    if(config == NULL){
        fputs("Echec de l'allocation de la configuration ennemie",stderr);
        return NULL;
    }

    config->countOfEnemies = 0;
    config->map = malloc(sizeof(EnemyConfig));

    if(config->map == NULL){
        free(config);
        fputs("Echec d'allocation le map des ennemies",stderr);
        return NULL;
    }

    // parsing de la configuration
    yaml_token_t token;

    while(true){
        if(!yaml_parser_scan(parser,&token)){
            fputs("\nEchec de lecture de token lors du parsing de configuration des héros\n",stderr);
            FREE_RESOURCES_ANQ_QUIT
        }

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        if(token.type == YAML_KEY_TOKEN && !consumeEnemyIn(parser,&token,config,parentDirPath)){
            fputs("\nEchec de récupération de la configuration ennemie",stderr);
            break;
        }

        yaml_token_delete(&token);
    }

    return config;
}

void freeEnemyConfig(EnemyConfig* config,bool freeContainer){
    assert(config != NULL && "L'ennemie fourni à la libération est NULL");

    for(int i = 0; i < ENEMY_MAX_FOR_ARRAY_KEYS; i++)
        freeImageConfig(config->actionsMap[i].framesConfig,true);

    if(freeContainer)
        free(config);
}

void freeEnemiesConfig(EnemiesConfig* config,bool freeContainer){
    assert(config != NULL && "La configuration d'ennemies fournie est NULL");

    for(int i = 0; i < config->countOfEnemies; i++)
        freeEnemyConfig(config->map + i,false);

    free(config->map);

    if(freeContainer)
        free(config);
}

void printEnemiesConfig(EnemiesConfig* enemiesConfig, char* toPrintBefore){
    assert(enemiesConfig != NULL && "Configuration des ennemies NULL pour l'affichage");

    printf("\n"CC_BLUE"%s------------------------------------------------------------------------"CC_RESET"\n",TO_PRINT);
    printf("\n"CC_BBLUE"%sConfiguration des ennemies"CC_RESET"\n",TO_PRINT);
    printf("\n"CC_BG_BLUE"%sNombre d'ennemies : %d"CC_RESET"\n",TO_PRINT,enemiesConfig->countOfEnemies);
    printf("\n"CC_BG_BLUE"%sListe des ennemies"CC_RESET"\n",TO_PRINT);
    printf(CC_BLUE"%s------------------------------------------------------------------------"CC_RESET"\n",TO_PRINT);

    char spaceBuffer[SPACE_BUFFER_SIZE];
    char spaceBufferImage[SPACE_BUFFER_SIZE];
    memset(spaceBuffer,0,sizeof(char) * SPACE_BUFFER_SIZE);
    memset(spaceBufferImage,0,sizeof(char) * SPACE_BUFFER_SIZE);

    if(toPrintBefore != NULL)
        strcpy(spaceBuffer,toPrintBefore);

    strncat(
        spaceBuffer,
        "\t",
        sizeof(char) * (SPACE_BUFFER_SIZE - (toPrintBefore != NULL ? strlen(toPrintBefore) : 0))
    );

    strncpy(spaceBufferImage,spaceBuffer,sizeof(char) * SPACE_BUFFER_SIZE);
    strncpy(spaceBufferImage,"\t\t\t",sizeof(char) * (SPACE_BUFFER_SIZE - strlen(spaceBuffer)));

    for(int i = 0; i < enemiesConfig->countOfEnemies; i++){
        EnemyConfig enemy = enemiesConfig->map[i];

        printf(CC_BWHITE"%sNom: "CC_RESET"%s\n",spaceBuffer,enemy.name);
        printf(CC_BWHITE"%sId: "CC_RESET"%s\n",spaceBuffer,enemy.id);
        printf(CC_BWHITE"%sActions: "CC_RESET"\n",spaceBuffer);

        for(int j = 0; j < ENEMY_MAX_FOR_ARRAY_KEYS; j++){
            printf(CC_BWHITE"%s\tNom de l'action: "CC_RESET"%s\n",spaceBuffer,enemy.actionsMap[j].actionName);
            printImageConfig(enemy.actionsMap[j].framesConfig,spaceBufferImage);
            printf("\n");
        }

        printf(CC_BLUE"%s------------------------------------------------------------------------"CC_RESET"\n",TO_PRINT);
        printf("\n");
    }
}