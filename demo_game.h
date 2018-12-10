//
// Created by Wangdawei on 2018/11/11.
//



#ifndef NEW_NEW_PROJECT2_DEMO_GAME_H
#define NEW_NEW_PROJECT2_DEMO_GAME_H

#include "player.h"
#include <stdio.h>

void demo_Game(Deck *);

// the meaning of the return value of demo_strike(): 0 means unsuccessfully used strike,
// 1  means successfully used . END_OF_GAME means the end of the game
int demo_strike(Player **, Player *, Player *, int *, int, FILE *, Deck **, Handnode *);

// 0 means the dying player is saved, 1 means the dying player is dead.;END_OF_GAME means the end of the game;
int demo_dying(Player **, Player *, Player *, int *, int, FILE *, Deck **);

//0 for unsuccessful ;1 for successful
int demo_dismantle(Player *, Player *, Deck **, FILE *, Handnode *);

//0 for unsuccessful ;1 for successful
int demo_snatch(Player *, Player *, Deck **, FILE *, Handnode *);

//1 for no one dies ;2 for other playerdie ; 3 for self dead;END_OF_GAME for the end of the game;
int demo_duel(Player **, Player *, Player *, Deck **, int *, int, FILE *, Handnode *);

//return 1
int demo_some_for_no(Player *, Deck **, Deck **, FILE *, Handnode *);

//return END_OF_GAME or 1
int demo_AOE(Player **, Player *, Deck **, FILE *, int *, int, Handnode *);

//0 for unsuccessful;1 for successful
int demo_peach(Player *, Handnode *, Deck **, FILE *);

//0 for unsuccessful;1 for successful
int demo_wine(Player *, Handnode *, Deck **, FILE *);

//return 1
int demo_equip(Player *player, Handnode *equip, Deck **discard_deck, FILE *fp);

//0 for fail,1 for success
int demo_lightning(Player *player, Handnode *lightning, FILE *fp);

//0 for fail,1 for success
int demo_drown(Player *, Player *, Handnode *, FILE *);

//0 for fail,1 for success
int demo_starve(Player *, Player *, FILE *, Handnode *);

//return 1
int demo_peach_garden(Player *, int, Handnode *, Deck **, FILE *);

//return 1
int demo_harvest(Player *, int, Handnode *, Deck **draw_deck, Deck **discard_deck, FILE *fp);


//0 for unsuccessful, 1 for successful, END_OF_GAME, 2 for self dead
int demo_borrow_sword(Player **pPlayer, Player *user, Player *attack, Player *defense, Deck **discard_deck, FILE *fp,
                      int *num_of_alive, int num_of_player, Handnode *borrow);

int//1 for current player dead 0 for survive ;END_OF_GAME
deal_fate_cards(Player **pPlayer, Player *player, Deck **discard_deck, Deck **draw_deck, FILE *fp, int *num_of_alive,
                int num_of_player);


#endif //NEW_NEW_PROJECT2_DEMO_GAME_H