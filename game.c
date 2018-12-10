//
// Created by Wangdawei on 2018/11/11.
//

#include <stdlib.h>
#include "game.h"
#include "player.h"
#include <string.h>

void discard(Player *player, Handnode *handnode, Deck **discard_deck) {
    push_deck(discard_deck, handnode->card);
    delete_handnode(player->hand, handnode);
    player->num_of_hand--;
}

void player_die(Player **pPlayer, Player *player_dead, Deck **discard_deck, int *num_of_alive) {
    Handnode *curr = *player_dead->hand;
    for (int i = 0; i < player_dead->num_of_hand; ++i, curr = curr->next) {
        push_deck(discard_deck, curr->card);
        delete_handnode(player_dead->hand, curr);
    }
    for (int j = 0; j < 3; ++j) {
        if (player_dead->equip[j] != NULL)
            push_deck(discard_deck, player_dead->equip[j]);
    }
    for (int j = 0; j < 3; ++j) {
        if (player_dead->fate[j] != NULL)
            push_deck(discard_deck, player_dead->fate[j]);
    }
    player_dead->dead_or_alive = 0;
    (*num_of_alive)--;
}

int Game_over(Player **pPlayer, int num_of_player, int *num_of_alive) {//0: Game not over; END_OF_GAME and Game over!
    Player *pcurrent1 = *pPlayer;
    Player *pcurrent2;
//    while (pcurrent1->dead_or_alive == 0) {
//        pcurrent1 = pcurrent1->next;
//    }
    if (*num_of_alive == 1)
        return END_OF_GAME;
    for (int i = 0; i < num_of_player - 1; ++i, pcurrent1 = pcurrent1->next) {
        if (pcurrent1->dead_or_alive == 0)
            continue;
        pcurrent2 = pcurrent1->next;
        for (int j = i; j < num_of_player; ++j, pcurrent2 = pcurrent2->next) {
            if (pcurrent2->dead_or_alive == 0)
                continue;
            if (pcurrent1->icountry != pcurrent2->icountry)
                return 0;
        }
    }
//    for (int i = 0; i < num_of_alive; ++i, current = current->next) {
//        if (current->country_reveal == 0 || strcmp(country, current->country) != 0)
//            return 0;
//    }
    return END_OF_GAME;
}

void push_fatecard(Player *player, Card *card) {
    player->fate[player->top_fate] = card;
    player->top_fate++;
}

Card *pop_fatezone(Player *player) {
    if (player->top_fate == 0) {
        return NULL;
    }
    player->top_fate--;
    Card *ret = player->fate[player->top_fate];
    player->fate[player->top_fate] = NULL;
    return ret;
}

int reveal_identity(Player **pPlayer, Player *main, int num_of_player) {//0 means done,1 means mercenary
    int count = 0;
    main->country_reveal = 1;
    Player *current = *pPlayer;
    for (int i = 0; i < num_of_player; ++i, current = current->next) {
        if (current->country_reveal == 1 && current->icountry == main->icountry)
            count++;
    }
    if (count >= num_of_player / 2.0) {
        return 1;//mercenary
    }
    return 0;
}