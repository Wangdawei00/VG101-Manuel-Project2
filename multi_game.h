//
// Created by Wangdawei on 2018/11/11.
//

#ifndef NEW_NEW_PROJECT2_MULTI_GAME_H
#define NEW_NEW_PROJECT2_MULTI_GAME_H

#include "player.h"
void show_max_blood(Player*);

void multi_Game(Deck *);

void cls();

void show_field(Player *main, int num_of_player);

void disp_hand(Player *);

void disp_fate(Player *);

void disp_equip(Player *);

void disp_identity(Player *);

void show_history(const char *history, FILE **fp_history);

void show_blood(Player *);

int
deal_fate_cards_multi(Player **pPlayer, Player *player, Deck **discard_deck, Deck **draw_deck, FILE *fp,
                      int *num_of_alive,
                      int num_of_player);

// 0 means the dying player is saved, 1 means the dying player is dead.;END_OF_GAME means the end of the game;
int multi_dying(Player **pPlayer, Player *main, Player *die, int *num_of_alive, int num_of_player, FILE *fp,
                Deck **discard_pile);

void change_player(Player *);

void disp_handnum(Player *);

int confirm();

int multi_strike(Player **pPlayer, Player *attack, Deck **discard_deck, int *num_of_alive,
                 int num_of_player, FILE *fp, Handnode *strike);

int multi_duel(Player **, Player *, Deck **, int *, int, FILE *, Handnode *);

void multi_wine(Player *player, Handnode *wine, Deck **discard_deck, FILE *fp);

void multi_peach(Player *player, Handnode *peach, Deck **discard_deck, FILE *fp);

void multi_equip(Player *player, Handnode *equip, Deck **discard_deck, FILE *fp);

void multi_lightning(Player *player, Handnode *lightning, FILE *fp);

void multi_some_for_no(Player *player, Deck **pDeck, Deck **discard_deck, FILE *fp, Handnode *some_for_no);

void multi_drown(Player *, Player *, Handnode *, FILE *);

void multi_starve(Player *, Player *, FILE *, Handnode *);

void multi_peach_garden(Player *, int, Handnode *, Deck **, FILE *);

void multi_dismantle(Player *attack, Player *defense, Deck **discard_deck, FILE *fp, Handnode *dismantle);

void multi_snatch(Player *, Player *, Deck **, FILE *, Handnode *);

void multi_harvest(Player *main, int num_of_player, int num_of_alive, Deck **draw_deck, Deck **discard_deck, FILE *fp,
                   Handnode *harvest);

int multi_AOE(Player **pPlayer, Player *main, Deck **discard_deck, FILE *fp, int num_of_player, int *num_of_alive,
              Handnode *aoe);
// 1 for successful ; 2 for self dead
int
multi_borrow_sword(Player **pPlayer, Player *user, Deck **discard_deck, FILE *fp, int *num_of_alive, int num_of_player,
                   Handnode *borrow);

#endif //NEW_NEW_PROJECT2_MULTI_GAME_H
