//
// Created by Wangdawei on 2018/11/11.
//



#ifndef NEW_NEW_PROJECT2_GAME_H
#define NEW_NEW_PROJECT2_GAME_H
#include "player.h"
#define END_OF_GAME 999

void discard(Player *player, Handnode *handnode, Deck **discard_deck);

void player_die(Player **pPlayer, Player *player_dead, Deck **discard_deck, int *num_of_alive);

int Game_over(Player **, int, int *);

void push_fatecard(Player *, Card *);

Card *pop_fatezone(Player *player);

int reveal_identity(Player **pPlayer, Player *main, int num_of_player);

#endif //NEW_NEW_PROJECT2_GAME_H
