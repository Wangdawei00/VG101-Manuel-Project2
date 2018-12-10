//
// Created by Wangdawei on 2018/11/11.
//

#ifndef NEW_NEW_PROJECT2_DECK_H
#define NEW_NEW_PROJECT2_DECK_H
#define MAX_NUMBER_OF_DECK 500

#include "card.h"

typedef struct _deck {
    Card *cards[MAX_NUMBER_OF_DECK];
    int num;
} Deck;

Card *pop_deck(Deck **);

void push_deck(Deck **, Card *);

Deck *deck_initialize_file(char *);

Deck *deck_initialize_stdin();

void free_deck(Deck *);

void deck_shuffle(Deck **);

void deck_reshuffle(Deck ** draw_deck, Deck ** discard_deck);

#endif //NEW_NEW_PROJECT2_DECK_H
