#include "./image-config-manager.h"
#include <string.h>
#include <assert.h>
#include "custom-color.h"
#include "raylib.h"

/**
 * @brief Assertions sur les fonctions de chargement
 */
#define LOAD_FUNCTIONS_GUARD assert(inConfig != NULL && "La configuration d'image à remplir fournie est NULL");\
    assert(parser != NULL && "Le parser fourni est NULL");\
    assert(parentDirPath != NULL && "Le chemin de dossier fourni est NULL");

/**
 * @brief Ternaire de choix de l'affichage
 */
#define TO_PRINT toPrintBefore == NULL ? "" : toPrintBefore

/**
 * @brief Libère les ressources lors du chargement raylib
 */
#define FREE_LOADING_RAYLIB_IMAGE_AND_QUIT freeGenericList(&config->linkedImages,true);\
config->linkedImages.errorState = true;\
return false;

/**
 * @brief Charge la configuration de chemins sous forme statique
 * @param inConfig la configuration dans laquelle placer le résultat
 * @param parser le parser
 *  * @param parentDirPath chemin du dossier parent de la configuration
 * @return si l'élément à bien été chargé
 */
bool loadStaticPath(ImageConfig* inConfig,yaml_parser_t* parser,char* parentDirPath){
    LOAD_FUNCTIONS_GUARD

    yaml_token_t token;
    bool stop = false;
    bool nextIsValue = false;

    while(!stop){
        if(!yaml_parser_scan(parser,&token))
            return false;

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        switch(token.type){
            case YAML_VALUE_TOKEN:
                nextIsValue = true;
            break;

            case YAML_SCALAR_TOKEN:
                if(nextIsValue){
                    stop = true;

                    // ajout du chemin par copie du chemin parent + chemin image
                    unsigned long long len = strlen(parentDirPath);
                    char* path = calloc(
                        (token.data.scalar.length / sizeof(char)) + (sizeof(char) * len) + 1,
                        sizeof(char)
                    );

                    if(path == NULL)
                        break;

                    strncpy(path,parentDirPath,sizeof(char) * len);
                    strncat(path,(char*) token.data.scalar.value,token.data.scalar.length);

                    if(listAppend(&inConfig->paths,path) ){
                        yaml_token_delete(&token);
                        return true;
                    }
                    else
                        stop = true;
                }
            break;

            default:;
        }

        yaml_token_delete(&token);
    }

    return false;
}

/**
 * @brief Charge la configuration de chemins sous forme de liste
 * @param inConfig la configuration dans laquelle placer le résultat
 * @param parser le parser
 *  * @param parentDirPath chemin du dossier parent de la configuration
 * @return si l'élément à bien été chargé
 */
bool loadListPath(ImageConfig* inConfig,yaml_parser_t* parser,char* parentDirPath){
    LOAD_FUNCTIONS_GUARD

    yaml_token_t token;
    bool startReadSequence = false;
    bool stop = false;

    while(!stop){
        if(!yaml_parser_scan(parser,&token))
            break;

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        switch(token.type){
            case YAML_FLOW_SEQUENCE_START_TOKEN:
                startReadSequence = true;
            break;

            case YAML_FLOW_SEQUENCE_END_TOKEN:
                return true;

            case YAML_SCALAR_TOKEN:
                if(!startReadSequence)
                    break;

                // ajout du chemin par copie du chemin parent + chemin image
                unsigned long long len = strlen(parentDirPath);
                char* path = calloc(
                    (token.data.scalar.length / sizeof(char)) + (sizeof(char) * len) + 1,
                    sizeof(char)
                );

                if(path == NULL)
                    break;

                strncpy(path,parentDirPath,sizeof(char) * len);
                strncat(path,(char*) token.data.scalar.value,token.data.scalar.length);

                if(listAppend(&inConfig->paths,path) )
                    break;

                stop = true;
                freeGenericList(&inConfig->paths,true);
            break;

            default:;
        }

        yaml_token_delete(&token);
    }

    return false;
}

/**
 * @brief Charge la configuration de chemins lors du parsing
 * @param inConfig la configuration dans laquelle placer le résultat
 * @param parser le parser
 *  * @param parentDirPath chemin du dossier parent de la configuration
 * @return si l'élément à bien été chargé
 */
bool loadPath(ImageConfig* inConfig,yaml_parser_t* parser,char* parentDirPath){
    LOAD_FUNCTIONS_GUARD

    newGenericListFrom(&inConfig->paths);

    return inConfig->type == STATIC ? loadStaticPath(inConfig,parser,parentDirPath) : loadListPath(inConfig,parser,parentDirPath);
}

/**
 * @brief Charge la configuration de description lors du parsing
 * @param inConfig la configuration dans laquelle placer le résultat
 * @param parser le parser
 *  * @param parentDirPath chemin du dossier parent de la configuration
 * @return si l'élément à bien été chargé
 */
bool loadDescription(ImageConfig* inConfig,yaml_parser_t* parser,char* parentDirPath){
    LOAD_FUNCTIONS_GUARD

    yaml_token_t token;
    bool nextIsValue = false;

    while(true){
        if(!yaml_parser_scan(parser,&token))
            return false;

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        switch(token.type){
            case YAML_VALUE_TOKEN:
                nextIsValue = true;
            break;

            case YAML_SCALAR_TOKEN:
                if(!nextIsValue)
                    break;

                strncpy(inConfig->description,(char*)token.data.scalar.value,sizeof(char) * (SUPPOSED_DESCRIPTION_MAX_LEN - 1));
                yaml_token_delete(&token);
                return true;

            default:;
        }

        yaml_token_delete(&token);
    }

    return false;
}

/**
 * @brief Charge la configuration de type lors du parsing
 * @param inConfig la configuration dans laquelle placer le résultat
 * @param parser le parser
 *  * @param parentDirPath chemin du dossier parent de la configuration
 * @return si l'élément à bien été chargé
 */
bool loadType(ImageConfig* inConfig,yaml_parser_t* parser,char* parentDirPath){
    LOAD_FUNCTIONS_GUARD

    yaml_token_t token;
    bool nextIsValue = false;

    while(true){
        if(!yaml_parser_scan(parser,&token))
            return false;

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        switch(token.type){
            case YAML_VALUE_TOKEN:
                nextIsValue = true;
            break;

            case YAML_SCALAR_TOKEN:
                if(!nextIsValue)
                    break;

                inConfig->type = atoi((char*) token.data.scalar.value);
                yaml_token_delete(&token);
                return true;

            default:;
        }

        yaml_token_delete(&token);
    }

    return false;
}

/**
 * @brief Charge la configuration de rotation lors du parsing
 * @param inConfig la configuration dans laquelle placer le résultat
 * @param parser le parser
 * @param parentDirPath chemin du dossier parent de la configuration
 * @return si l'élément à bien été chargé
 */
bool loadRotation(ImageConfig* inConfig,yaml_parser_t* parser,char* parentDirPath){
    LOAD_FUNCTIONS_GUARD

    yaml_token_t token;
    bool nextIsValue = false;

    while(true){
        if(!yaml_parser_scan(parser,&token))
            return false;

        if(
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_STREAM_END_TOKEN
        ){
            yaml_token_delete(&token);
            break;
        }

        switch(token.type){
            case YAML_VALUE_TOKEN:
                nextIsValue = true;
            break;

            case YAML_SCALAR_TOKEN:
                if(!nextIsValue)
                    break;

                inConfig->rotation = atoi((char*)token.data.scalar.value);
                yaml_token_delete(&token);
                return true;

            default:;
        }

        yaml_token_delete(&token);
    }

    return false;  
}

ImageConfig createImageFromConfig(yaml_parser_t* parser,char* parentDirPath){
    assert(parser != NULL && "Le parser fourni est NULL");
    assert(parentDirPath != NULL && "Le chemin parent fourni est NULL");

    ImageConfig config = {
        .errorState = true,
        .paths = {.errorState = true},
        .linkedImages = {.errorState = true},
    };

    memset(config.description,0,sizeof(char) * SUPPOSED_DESCRIPTION_MAX_LEN);
    memset(config.id,0,sizeof(char) * SUPPOSED_ID_MAX_LEN);

    int countOfElementsToLoad = 4;
    bool nextIsKey = false;
    bool stop = false;
    yaml_token_t token;

    while(countOfElementsToLoad != 0 && !stop){
        if(!yaml_parser_scan(parser,&token))
            break;

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
                if(nextIsKey){
                    nextIsKey = false;

                    // vérification de l'élément de comparaison à charger
                    bool (*loadFunction)(ImageConfig*,yaml_parser_t*,char*) = NULL;

                    if(strcmp((char*)token.data.scalar.value,"path") == 0)
                        loadFunction = loadPath;
                    else if(strcmp((char*)token.data.scalar.value,"description") == 0)
                        loadFunction = loadDescription;
                    else if(strcmp((char*)token.data.scalar.value,"type") == 0)
                        loadFunction = loadType;
                    else if(strcmp((char*)token.data.scalar.value,"rotation") == 0)
                        loadFunction = loadRotation;

                    if(loadFunction == NULL)
                        break;

                    // chargement de l'élément
                    if(!loadFunction(&config,parser,parentDirPath)){
                        stop = true;
                        break;
                    }

                    countOfElementsToLoad--;
                }
            break;

            default:;
        } 

        yaml_token_delete(&token);          
    }

    if(countOfElementsToLoad == 0)
        config.errorState = false;

    return config;
}

bool loadLinkedImages(ImageConfig* config){
    assert(config != NULL && "La configuration fournie pour le chargement des images raylib est NULL");

    if(config->paths.errorState)
        return false;

    // vérification de configuration non déjà chargée
    if(!config->linkedImages.errorState)
        return true;

    newGenericListFrom(&config->linkedImages);

    // chargement des images raylib
    while(config->paths.items != NULL){
        char* imagePath = (char*) config->paths.items->data;

        // création de l'image
        Texture2D* texture = malloc(sizeof(Texture2D));

        if(texture == NULL){
            fputs("\nEchec d'allocation de l'espace image raylib",stderr);
            FREE_LOADING_RAYLIB_IMAGE_AND_QUIT
        }

        Texture2D loadedTexture = LoadTexture(imagePath);

        if(!IsTextureReady(loadedTexture)){
            fputs("\nEchec du chargement de l'image par raylib",stderr);
            free(texture);
            FREE_LOADING_RAYLIB_IMAGE_AND_QUIT
        }

        memcpy(texture,&loadedTexture,sizeof(Texture2D));

        // enregistrement de l'image
        if(!listAppend(&config->linkedImages,texture)){
            fputs("\nEchec d'enregistrement de l'image",stderr);
            UnloadTexture(loadedTexture);
            free(texture);
            FREE_LOADING_RAYLIB_IMAGE_AND_QUIT
        }

        config->paths.items = config->paths.items->nextItem;
    }

    config->paths.items = config->paths.listStart;
    config->linkedImages.errorState = false;

    return true;
}

void freeImageConfig(ImageConfig* config,bool freeContainer){
    assert(config != NULL && "Configuration d'image NULL pour la liberation");

    if(!config->paths.errorState)
        freeGenericList(&config->paths,true);

    if(!config->linkedImages.errorState){
        while(config->linkedImages.items != NULL){
            Texture2D* texture = (Texture2D*) config->linkedImages.items->data;

            if(IsTextureReady(*(texture)))
                UnloadTexture(*(texture));

            config->linkedImages.items = config->linkedImages.items->nextItem;
        }

        config->linkedImages.items = config->linkedImages.listStart;
        freeGenericList(&config->linkedImages,true);
        config->linkedImages.errorState = true;
    }

    if(freeContainer)
        free(config);
}

void printImageConfig(ImageConfig* config,char* toPrintBefore){
    assert(config != NULL && "Configuration d'image NULL pour l'affichage");

    printf("\n"CC_BBLUE"%sConfiguration d'image :"CC_RESET,TO_PRINT);
    printf("\n"CC_BWHITE"%s\tId: "CC_RESET"%s",TO_PRINT,config->id);
    printf("\n"CC_BWHITE"%s\tType: "CC_RESET"%d",TO_PRINT,config->type);
    printf("\n"CC_BWHITE"%s\tDescription: "CC_RESET"%s",TO_PRINT,config->description);
    printf("\n"CC_BWHITE"%s\tRotation: "CC_RESET"%d",TO_PRINT,config->rotation);
    printf("\n"CC_BWHITE"%s\tContient une erreur: "CC_RESET"%s",TO_PRINT,config->errorState ? "Oui" : "Non");
    printf("\n"CC_BWHITE"%s\tListe des chemins: "CC_RESET,TO_PRINT);

    while(config->paths.items != NULL){
        printf("\n%s\t\t=> %s",TO_PRINT,(char*) config->paths.items->data);

        config->paths.items = config->paths.items->nextItem;
    }

    config->paths.items = config->paths.listStart;
}
