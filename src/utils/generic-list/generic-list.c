#include "./generic-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void newGenericListFrom(GenericList* list){
    list->items = list->listStart = list->listEnd = NULL;
    list->errorState = false;
}

GenericList newGenericList(){
    return (GenericList) {
        .items = NULL,
        .listStart = NULL,
        .listEnd = NULL,
        .errorState = false
    };
}

bool listAppend(GenericList* listManager,void* data){
    assert(listManager != NULL && "Le gestionnaire de liste fourni est NULL");

    // création de l'item
    GenericListItem* newItemAddress = malloc(sizeof(struct GenericListItem));

    if(newItemAddress == NULL){
        fputs("\nEchec d'allocation d'un élément de liste générique\n",stderr);
        listManager->errorState = true;
        return false;
    }

    newItemAddress->data = data;
    newItemAddress->nextItem = NULL;

    // ajout comme élément de fin de liste sinon ajout comme seul élément de liste
    if(listManager->listEnd != NULL){
        listManager->listEnd->nextItem = newItemAddress;
        listManager->listEnd = newItemAddress;
    }
    else
        listManager->items = listManager->listStart = listManager->listEnd = newItemAddress;

    return true;
}

bool listPrepend(GenericList* listManager,void* data){
    assert(listManager != NULL && "Le gestionnaire de liste fourni est NULL");
    
    // création de l'item
    GenericListItem* newItemAddress = malloc(sizeof(struct GenericListItem));

    if(newItemAddress == NULL){
        fputs("\nEchec d'allocation d'un élément de liste générique\n",stderr);
        listManager->errorState = true;
        return false;
    }

    newItemAddress->data = data;
    newItemAddress->nextItem = listManager->items;

    // ajout de l'item dans la liste et marquage comme début de liste
    listManager->items = newItemAddress;
    listManager->listStart = newItemAddress;

    // si premier élément alors marqué également comme dernier
    if(listManager->listEnd == NULL)
        listManager->listEnd = newItemAddress;

    return true;
}

/**
 * @brief Libérateur interne d'allocation de liste
 * @param item l'élément à libérer
 * @param freeContainedData si true libère la donnée contenue
 * @
 */
void internalFree(GenericListItem* item,bool freeContainedData){
    if(item == NULL)
        return;

    if(item->nextItem != NULL)
        internalFree(item->nextItem,freeContainedData);

    if(freeContainedData)
        free(item->data);

    free(item);
}

void freeGenericList(GenericList* list,bool freeContainedData){
    internalFree(list->items,freeContainedData);
    newGenericListFrom(list);
}
