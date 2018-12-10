//
// Created by Wangdawei on 2018/11/11.
//

#include <stdlib.h>
#include "player.h"
#include <string.h>
#include <stdio.h>

#define MIN(a, b) a > b ? b : a

Player *initialize_player() {
    const char *country[] = {"WEI", "SHU", "WU", "QUN"};
    Player *ret = calloc(1, sizeof(Player));
    ret->hand = calloc(1, sizeof(Handnode *));
    ret->max_blood = rand() % 2 + 3;
    ret->num_of_hand = 0;
    ret->top_fate = 0;
//    ret->play_which_card = 0;
    ret->icountry = rand() % 4;
    ret->country = country[ret->icountry];
//    ret->reply = 0;
    ret->blood = ret->max_blood;
    ret->use_of_wine = 0;
    ret->use_of_strike = 0;
    ret->country_reveal = 0;
    ret->idraw = 1;
    ret->iplay = 1;
    ret->dead_or_alive = 1;//alive
    return ret;
}

Player **player_append(Player **pPlayer, Player *player_to_append) {
    if (*pPlayer == NULL) {
        *pPlayer = player_to_append;
        (*pPlayer)->next = player_to_append;
        (*pPlayer)->prev = player_to_append;
    } else {
        player_to_append->next = *pPlayer;
        player_to_append->prev = (*pPlayer)->prev;
        (*pPlayer)->prev->next = player_to_append;
        (*pPlayer)->prev = player_to_append;
    }
    return pPlayer;
}

Player **player_delete(Player **pPlayer, Player *player_to_delete) {
    Player *current = *pPlayer;
    for (; current != player_to_delete; current = current->next);
    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current->hand);
    free(current);
    return pPlayer;
}

int draw(Player *player,
         Deck **pDeck,//n for fail; -1 for success(fail means need reshuffle) and n means the number that has been drawn
         int n) {
    Card *temp;
    for (int i = 0; i < n; ++i) {
        temp = pop_deck(pDeck);
        if (temp == NULL) {
            player->num_of_hand += i;
            return i;
        }

        append_handnode(player->hand, temp);
    }
    player->num_of_hand += n;
    return -1;
}

void free_players(Player **pPlayer) {
    Player *tmp = NULL;
    Player *pHead = *pPlayer;
    if (pHead == NULL)
        return;
    pHead->prev->next = NULL;
    while (pHead->next != NULL) {
        tmp = pHead;
        pHead = pHead->next;
        free(tmp);
    }
    free(pHead);
}

int distance(Player *attack, Player *defense) {
    int dist;
    int times1 = 0, times2 = 0;
    Player *current = attack;
    for (times1 = 0; current != defense; current = current->next) {
        if (current->dead_or_alive == 0)
            continue;
        ++times1;
    }
//    printf("Player%d and Player%d.\n",attack->index,defense->index);
//    fflush(stdout);/
//    fflush(fp);
    for (times2 = 0, current = attack; current != defense; current = current->prev) {
        if (current->dead_or_alive == 0)
            continue;
        ++times2;
    }
    dist = MIN(times1, times2);
    //dist = MIN(abs(attack.index - defense.index), abs(n - (attack.index - defense.index)));
    if (attack->equip[2] != NULL)
        dist--;
    if (defense->equip[0] != NULL)
        dist++;
    if (dist < 1)
        dist = 1;
    return dist;
}