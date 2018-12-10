//
// Created by Wangdawei on 2018/11/11.
//

#ifndef NEW_NEW_PROJECT2_CARD_H
#define NEW_NEW_PROJECT2_CARD_H
#define NULL_INDEX_OF_CARD 0
typedef struct _card {
    const char *suit;
    const char *num;
    const char *name;
    int inum;
    int iname;
} Card;

Card *create_card(const char *);

#endif //NEW_NEW_PROJECT2_CARD_H
