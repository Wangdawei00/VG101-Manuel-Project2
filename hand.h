//
// Created by Wangdawei on 2018/11/11.
//

#ifndef NEW_NEW_PROJECT2_HAND_H
#define NEW_NEW_PROJECT2_HAND_H

#include "card.h"

typedef struct _handnode {
    Card *card;
    struct _handnode *next;
} Handnode;

Handnode *handnode_create(Card *);

Handnode **append_handnode(Handnode **, Card *);

Handnode **delete_handnode(Handnode **, Handnode *);

void show_hand(Handnode**);

#endif //NEW_NEW_PROJECT2_HAND_H
