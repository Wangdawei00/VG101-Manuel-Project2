//
// Created by Wangdawei on 2018/11/11.
//

#include <stdlib.h>
#include <stdio.h>
#include "hand.h"

Handnode *handnode_create(Card *card) {
    Handnode *handnode = calloc(1, sizeof(Handnode));
    handnode->card = card;
    handnode->next = NULL;
    return handnode;
}

Handnode **append_handnode(Handnode **pHandnode, Card *card) {
    if (*pHandnode == NULL) {
        *pHandnode = handnode_create(card);
    } else {
        Handnode *temp = handnode_create(card);
        temp->next = (*pHandnode)->next;
        (*pHandnode)->next = temp;
    }
    return pHandnode;
}

Handnode **delete_handnode(Handnode **pHandnode, Handnode *handnode) {
    Handnode *current = *pHandnode;
    Handnode *prev = current;
    if (current == handnode) {
        (*pHandnode) = (*pHandnode)->next;
        handnode = (*pHandnode);
//        free(current);
        return pHandnode;
    }

    for (; current != handnode && current != NULL; current = current->next)
        prev = current;
    if (current == NULL)
        fprintf(stderr, "can't delete");
    else {
        prev->next = current->next;
        handnode = prev;
//        free(current);
    }
    return pHandnode;
}

void show_hand(Handnode **pHandnode) {
    Handnode *current = *pHandnode;
    if (current == NULL)
        return;
    else {
        for (int i = 1; current != NULL; i++, current = current->next) {
            printf("%d) %s\n", i, current->card->name);
        }
    }
}