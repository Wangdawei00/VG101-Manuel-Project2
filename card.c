//
// Created by Wangdawei on 2018/11/11.
//

#include <stdlib.h>
#include "card.h"
#include <string.h>

Card *create_card(const char *name) {
    Card *ret = calloc(1, sizeof(Card));
    char *suit[] = {"Spade", "Heart", "Club", "Diamond"};
    char *num[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    char *names[] = {"Duel", "Dismantle", "Snatch", "Borrowed sword", "Arrow barrage", "Barbarian invasion",
                     "Peach garden",
                     "Something for nothing", "Bountiful harvest", "Lightning", "Drown in happiness", "Starvation",
                     "Binocular", "Bow", "Horse", "Strike", "Peach", "Dodge", "Wine"};
    ret->name = name;
    ret->iname = rand() % 13;
    ret->num = num[ret->iname];
    ret->suit = suit[rand() % 4];
    for (int i = 0; i < 19; ++i) {
        if (strcmp(name, names[i]) == 0) {
            ret->iname = i + 1;
            break;
        }
    }
//    ret->index = NULL_INDEX_OF_CARD;
    return ret;
}