//
// Created by Wangdawei on 2018/11/11.
//

#ifndef NEW_NEW_PROJECT2_PLAYER_H
#define NEW_NEW_PROJECT2_PLAYER_H

#include <stdio.h>
#include "hand.h"
#include "deck.h"

typedef struct _player {
    Handnode **hand;
    int num_of_hand;
    const char *country;
    Card *equip[3];// the first is the horse, the second is the bow, and the third is the binocular
    Card *fate[3];
    int blood;
    int max_blood;
    int index;
    struct _player *prev;
    struct _player *next;
    int top_fate;
//    int reply;
//    int play_which_card;
    int use_of_wine;
    int use_of_strike;
    int country_reveal;
    int icountry;
    int idraw;
    int iplay;
    int dead_or_alive;
} Player;

Player *initialize_player();

Player **player_append(Player **, Player *);

Player **player_delete(Player **, Player *);

int draw(Player *, Deck **, int);

void free_players(Player **);

int distance(Player *attack, Player *defense);

#endif //NEW_NEW_PROJECT2_PLAYER_H
