#include "./config-manager.h"
#include "../assets/assets.h"
#include <assert.h>
#include <stdio.h>

/**
 * @brief Vérifie si la valeur fournie est NULL et libère les ressources au cas où
 */
#define CHECK_NULL_AND_QUIT(on,errorMessage)if(on == NULL){ \
    freeGameConfig(config,true);\
    fputs("\n"errorMessage,stderr);\
    return NULL;\
}

void* loadConfig(char* path,void* (*treatmentFunction)(yaml_parser_t*,char*)){
    assert(path != NULL && "Le chemin fourni pour le document à charger est NULL");

    // chargement du fichier
    FILE* configFile = fopen(path,"r");

    if(configFile == NULL){
        fprintf(stderr,"\nEchec de lecture d'ouverture du fichier de configuration <%s>\n",path);
        return NULL;
    }

    // chargement du document
    yaml_parser_t parser;

    if(!yaml_parser_initialize(&parser)){
        fputs("\nEchec d'initialisation du parser yaml\n",stderr);
        fclose(configFile);
        return NULL;
    }

    yaml_parser_set_input_file(&parser,configFile);

    // parsing
    char dirPath[SUPPOSED_PATH_MAX_LEN];
    memset(dirPath,0,sizeof(char) * SUPPOSED_PATH_MAX_LEN);
    extractDirPathFrom(path,dirPath,sizeof(char) * SUPPOSED_PATH_MAX_LEN);

    void* treatmentResult = treatmentFunction(&parser,dirPath);

    // libération des ressources
    yaml_parser_delete(&parser);
    fclose(configFile);

    return treatmentResult;
}

GameConfig* loadGameConfig(){
    // allocation de la configuration de jeux
    GameConfig* config = malloc(sizeof(GameConfig) );

    if(config == NULL){
        fputs("Echec d'allocation de la configuration de jeux",stderr);
        return NULL;
    }

    // initialisation à NULL
    config->enemiesConfig = NULL;
    config->heroesConfig = NULL;
    config->tillsConfig = NULL;
    config->itemsConfig = NULL;
    config->homeMapConfig = NULL;

    // chargement des configurations
    config->enemiesConfig = loadConfig(ENEMIES_CONFIG_FILE_PATH,loadEnemies);
    CHECK_NULL_AND_QUIT(config->enemiesConfig,"Echec de chargement de la configuration des ennemies")

    config->heroesConfig = loadConfig(HEROES_CONFIG_FILE_PATH,loadHeroesConfig);
    CHECK_NULL_AND_QUIT(config->heroesConfig,"Echec de chargement de la configuration des héros")

    config->tillsConfig = loadConfig(TILLS_CONFIG_FILE_PATH,loadTillsConfig);
    CHECK_NULL_AND_QUIT(config->tillsConfig,"Echec de chargement de la configuration des tills")

    config->itemsConfig = loadConfig(ITEMS_CONFIG_FILE_PATH,loadItemsConfig);
    CHECK_NULL_AND_QUIT(config->itemsConfig,"Echec de chargement de la configuration des items")

    config->homeMapConfig = loadConfig(HOME_MAP_CONFIG_FILE_PATH,loadGameMapConfig);
    CHECK_NULL_AND_QUIT(config->homeMapConfig,"Echec de chargement de la map d'accueil")

    return config;
}

bool loadItemsImages(GameConfig* gameConfig){
    assert(gameConfig != NULL && gameConfig->itemsConfig != NULL && "La configuration du jeux passée est NULL");

    for(int i = 0; i < gameConfig->itemsConfig->countOfItems; i++){
        if(!loadLinkedImages(gameConfig->itemsConfig->map + i))
            return false;
    }

    return true;
}

bool loadTillsImages(GameConfig* gameConfig){
    assert(gameConfig != NULL && gameConfig->tillsConfig != NULL && "La configuration du jeux passé est NULL");

    for(int i = 0; i < gameConfig->tillsConfig->countOfTills; i++){
        if(!loadLinkedImages(gameConfig->tillsConfig->map + 1))
            return false;
    }

    return true;
}

bool loadEnemiesImages(GameConfig* gameConfig){
    assert(gameConfig != NULL && gameConfig->enemiesConfig != NULL && "La configuration de jeux fourni est NULL");

    for(int i = 0; i < gameConfig->enemiesConfig->countOfEnemies; i++){
        EnemyConfig* config = gameConfig->enemiesConfig->map + i;

        for(int j = 0; j < ENEMY_MAX_FOR_ARRAY_KEYS; j++){
            if(!loadLinkedImages(config->actionsMap[j].framesConfig))
                return false;
        }
    }

    return true;
}

bool loadHeroesImages(GameConfig* gameConfig){
    assert(gameConfig != NULL && gameConfig->heroesConfig != NULL && "La configuration de jeux fourni est NULL");

    for(int i = 0; i < gameConfig->heroesConfig->countOfHeroes; i++){
        HeroConfig* config = gameConfig->heroesConfig->map + i;

        for(int j = 0; j < HERO_MAX_FOR_ARRAY_KEYS; j++){
            if(!loadLinkedImages(config->actionsConfigs[j].framesConfig))
                return false;
        }
    }

    return true;
}

void freeGameConfig(GameConfig* config,bool freeContainer){
    assert(config != NULL && "La configuration de jeux fournie est NULL pour la libération");

    if(config->enemiesConfig != NULL)
        freeEnemiesConfig(config->enemiesConfig,true);

    if(config->heroesConfig != NULL)
        freeHeroesConfig(config->heroesConfig,true);

    if(config->itemsConfig != NULL)
        freeItemsConfig(config->itemsConfig,true);

    if(config->tillsConfig != NULL)
        freeTillsConfig(config->tillsConfig,true);

    if(config->homeMapConfig != NULL)
        freeGameMapConfig(config->homeMapConfig,true);

    if(freeContainer)
        free(config);
}