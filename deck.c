//
// Created by Wangdawei on 2018/11/11.
//

#include <stdlib.h>
#include <stdio.h>
#include "deck.h"
#include <string.h>
#include <ctype.h>

#define STACK_OVER_FLOW 42

Card *pop_deck(Deck **pDeck) {
    if ((*pDeck)->num == 0)
        return NULL;
    else {
        Card *ret = (*pDeck)->cards[(*pDeck)->num - 1];

        (*pDeck)->cards[(*pDeck)->num - 1] = NULL;
        (*pDeck)->num--;
        return ret;
    }
}

void push_deck(Deck **pDeck, Card *card) {
    if ((*pDeck)->num == MAX_NUMBER_OF_DECK) {
        fprintf(stderr, "STACK OVER FLOW! TOO MANY CARDS!(NO GREATER THAN %d)\n", MAX_NUMBER_OF_DECK);
        exit(STACK_OVER_FLOW);
    } else {
        (*pDeck)->cards[(*pDeck)->num] = card;
        (*pDeck)->num++;
    }
}

Deck *deck_initialize_file(char *argc) {
    Deck *ret = calloc(1, sizeof(Deck));
    Card *temp;
    char input[20][25] = {'\0'};
    int num_of_cd = 0;
    char cd_name[20][25] = {'\0'};
    int char_num = 0;
    char num[20][3];
    int index = NULL_INDEX_OF_CARD + 1;
    FILE *fp = fopen(argc, "r");
    if (fp == NULL) {
        fprintf(stderr, "Invalid file location!\n");
        exit(-1);
    } else {
        for (int i = 0; fgets(input[i], 25, fp) != NULL; i++) {
            if (strcmp(input[i], "\n") == 0)
                break;
            char_num = 0;
            for (int j = 0; j < 25; ++j) {
                if (input[i][j] == ' ' || isalpha(input[i][j])) {
                    cd_name[i][j] = input[i][j];
                    char_num++;
                } else if (isdigit(input[i][j])) {
                    num[i][j - char_num] = input[i][j];
                } else
                    break;
            }
            for (int p = char_num - 1; cd_name[i][p] == ' '; --p) {
                cd_name[i][p] = (char) 0;
            }
            num_of_cd = strtol(num[i], NULL, 10);
            for (int k = 0; k < num_of_cd; ++k) {
                temp = create_card(cd_name[i]);
//                temp->index = index;
//                index++;
                push_deck(&ret, temp);
            }
        }
        fclose(fp);
    }
    return ret;
}

Deck *deck_initialize_stdin() {
    char *name[] = {"Duel", "Dismantle", "Snatch", "Borrowed sword", "Arrow barrage", "Barbarian invasion",
                    "Peach garden",
                    "Something for nothing", "Bountiful harvest", "Lightning", "Drown in happiness", "Starvation",
                    "Binocular", "Bow", "Horse", "Strike", "Peach", "Dodge", "Wine"};
    int index = NULL_INDEX_OF_CARD + 1;
    Deck *ret = calloc(1, sizeof(Deck));
    Card *temp;
    puts("Please input the number of cards.");
//    fflush(stdout);
    int num;
    for (int i = 0; i < 19; ++i) {
        num = 0;
        printf("%s: ", name[i]);
        scanf("%d", &num);
        for (int j = 0; j < num; ++j) {
            temp = create_card(name[i]);
//            temp->index = index;
//            index++;
            push_deck(&ret, temp);
        }
    }
    return ret;
}

void deck_shuffle(Deck **deck_need_shuffle) {
    Card *temp;
    int a, b;
    int times = 1000;
//    deck ret = deck_need_shuffle;
    for (int i = 0; i < times; ++i) {
        a = rand() % (*deck_need_shuffle)->num;
        b = rand() % (*deck_need_shuffle)->num;
        temp = (*deck_need_shuffle)->cards[a];
        (*deck_need_shuffle)->cards[a] = (*deck_need_shuffle)->cards[b];
        (*deck_need_shuffle)->cards[b] = temp;
    }
//    return ret;
}

void free_deck(Deck *deck) {
    for (int i = 0; i < (deck)->num; ++i) {
        free((deck)->cards[i]);
    }
}

void deck_reshuffle(Deck **pDeck, Deck **discard_deck) {
    int num = (*discard_deck)->num;
    for (int i = 0; i < num; ++i) {
        push_deck(pDeck, pop_deck(discard_deck));
    }
    deck_shuffle(pDeck);
//    fprintf(fp,"deck reshuffled.\n");
}