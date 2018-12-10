//
// Created by Wangdawei on 2018/11/11.
//

#include <stdlib.h>
#include <stdio.h>
#include "demo_game.h"
#include <string.h>
#include <time.h>
#include "game.h"

void demo_Game(Deck *deck) {
    int identity_index = 4;
    deck_shuffle(&deck);
    int end_of_game = 0;
    int hand_uplimit;
    int choice;
    char *names[] = {"Duel", "Dismantle", "Snatch", "Borrowed Sword", "Arrow barrage", "Barbarian invasion",
                     "Peach garden",
                     "Something for nothing", "Bountiful harvest", "Lightning", "Drown in happiness", "Starvation",
                     "Binocular", "Bow", "Horse", "Strike", "Peach", "Dodge", "Wine"};
    char *num[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    for (int l = 0; l < deck->num; ++l) {
        deck->cards[l]->name = names[(deck->cards[l]->iname) - 1];
        for (int i = 0; i < 13; ++i) {
            if (strcmp(num[i], deck->cards[l]->num) == 0) {
                deck->cards[l]->inum = i;
                break;
            }
        }
    }
    FILE *fp;
    fp = fopen("demo file.txt", "w");
    Player **pPlayer = calloc(1, sizeof(Player *));
    const int num_of_player = rand() % 9 + 2;
    Deck *discard_deck = calloc(1, sizeof(Deck));
    Player *new;
    Handnode *handcurr;
    for (int i = 0; i < num_of_player; ++i) {
        new = initialize_player();
        new->index = i + 1;
        player_append(pPlayer, new);
    }
//     initialize all players and deck
    int num_of_alive = num_of_player;
    int target1;
    int target2;
    Player *target_one;
    Player *target_two;
    Player *current = *pPlayer;
    int selfdead = 0;
    int reveal_or_not;
    int temp_draw;
    for (int i = 0; i < num_of_player; i++) {
        draw(current, &deck, 4);
        current = current->next;
    }
    for (Player *pcurrent = *pPlayer; end_of_game != END_OF_GAME; pcurrent = pcurrent->next) {
        selfdead = 0;
        if (pcurrent->dead_or_alive == 0)
            continue;
        // fate phase
        switch (deal_fate_cards(pPlayer, pcurrent, &discard_deck, &deck, fp, &num_of_alive, num_of_player)) {
            case 1://player dead
                continue;
            case 0://
                break;
            case END_OF_GAME:
                end_of_game = END_OF_GAME;
                continue;
            default:
                break;
        }
        //fate phase end
        //draw phase
        if (pcurrent->idraw == 1) {
            temp_draw = draw(pcurrent, &deck, 2);
            if (temp_draw != -1) {
                deck_reshuffle(&deck, &discard_deck);
                draw(pcurrent, &deck, 2 - temp_draw);
            }
            fprintf(fp, "Player%d draws 2 cards.\n", pcurrent->index);
        } else {
            fprintf(fp, "Player%d skips his draw phase because of \"Starvation\".\n", pcurrent->index);
            pcurrent->idraw = 1;
        }
        //draw phase end
        //player phase
        if (pcurrent->iplay == 1) {
            //decide whether he reveals his identity or not
            if (pcurrent->country_reveal == 0) {
                reveal_or_not = rand() % 2;
                if (reveal_or_not == 1) {
                    if (reveal_identity(pPlayer, pcurrent, num_of_player) == 1) {
                        pcurrent->country = "Mer";
                        pcurrent->icountry = identity_index;
                        identity_index++;
                        fprintf(fp, "Player%d reveals his identity as a Mercenary.\n", pcurrent->index);
                    } else {
                        fprintf(fp, "Player%d reveals his identity as a General of \"%s\".\n", pcurrent->index,
                                pcurrent->country);
                    }
                }
            }
            //decided
            do {
                if (pcurrent->num_of_hand == 0)
                    choice = 0;
                else
                    choice = rand() % (pcurrent->num_of_hand + 1);
                if (choice == 0)
                    continue;
                handcurr = *pcurrent->hand;
                for (int i = 1; i < choice; ++i) {
                    handcurr = handcurr->next;
                }
                switch (handcurr->card->iname) {
                    case 1://duel
                        target1 = rand() % (num_of_player - 1);
                        target_one = pcurrent->next;
                        for (int cnt = 0;
                             cnt < target1 || target_one->dead_or_alive == 0 || target_one == pcurrent; ++cnt) {
                            target_one = target_one->next;
                        }
                        switch (demo_duel(pPlayer, pcurrent, target_one, &discard_deck, &num_of_alive, num_of_player,
                                          fp,
                                          handcurr)) {
                            case 3:
                                selfdead = 1;
                            case 1:
                            case 2:
                                continue;
                            case END_OF_GAME:
                                end_of_game = END_OF_GAME;
                            default:
                                break;
                        }
                        continue;
                    case 2://dismantle
                        target1 = rand() % (num_of_player - 1);
                        target_one = pcurrent->next;
                        for (int cnt = 0;
                             cnt < target1 || target_one->dead_or_alive == 0 || target_one == pcurrent; ++cnt) {
                            target_one = target_one->next;
                        }
                        demo_dismantle(pcurrent, target_one, &discard_deck, fp, handcurr);
                        continue;
                    case 3://snatch
                        target1 = rand() % (num_of_player - 1);
                        target_one = pcurrent->next;
                        for (int cnt = 0;
                             cnt < target1 || target_one->dead_or_alive == 0 || target_one == pcurrent; ++cnt) {
                            target_one = target_one->next;
                        }

                        demo_snatch(pcurrent, target_one, &discard_deck, fp, handcurr);
                        continue;
                    case 4://borrowed sword
                        target1 = rand() % (num_of_player - 1);
                        target_one = pcurrent->next;
                        for (int cnt = 0;
                             cnt < target1 || target_one->dead_or_alive == 0 || target_one == pcurrent; ++cnt) {
                            target_one = target_one->next;
                        }
                        target2 = rand() % (num_of_player - 1);
                        target_two = pcurrent->next;
                        for (int cnt = 0;
                             cnt < target2 || target_two->dead_or_alive == 0 || target_two == target_one; ++cnt) {
                            target_two = target_two->next;
                        }
                        switch (demo_borrow_sword(pPlayer, pcurrent, target_one, target_two, &discard_deck, fp,
                                                  &num_of_alive, num_of_player, handcurr)) {
                            case END_OF_GAME:
                                end_of_game = END_OF_GAME;
                                break;
                            case 2:
                                selfdead = 1;
                                break;
                            default:
                                break;
                        }
                        continue;
                    case 5://arrow barrage
                    case 6://barbarian invasion
                        if (demo_AOE(pPlayer, pcurrent, &discard_deck, fp, &num_of_alive, num_of_player, handcurr) ==
                            END_OF_GAME)
                            end_of_game = END_OF_GAME;
                        continue;
                    case 7://peach garden
                        demo_peach_garden(pcurrent, num_of_player, handcurr, &discard_deck, fp);
                        continue;
                    case 8://some for none
                        demo_some_for_no(pcurrent, &deck, &discard_deck, fp, handcurr);
                        continue;
                    case 9://bountiful harvest
                        demo_harvest(pcurrent, num_of_player, handcurr, &deck, &discard_deck, fp);
                        continue;
                    case 10://lightning
                        demo_lightning(pcurrent, handcurr, fp);
                        continue;
                    case 11://drown
                        target1 = rand() % (num_of_player - 1);
                        target_one = pcurrent->next;
                        for (int cnt = 0;
                             cnt < target1 || target_one->dead_or_alive == 0 || target_one == pcurrent; ++cnt) {
                            target_one = target_one->next;
                        }
                        demo_drown(pcurrent, target_one, handcurr, fp);
                        continue;
                    case 12://starve
                        target1 = rand() % (num_of_player - 1);
                        target_one = pcurrent->next;
                        for (int cnt = 0;
                             cnt < target1 || target_one->dead_or_alive == 0 || target_one == pcurrent; ++cnt) {
                            target_one = target_one->next;
                        }
                        demo_starve(pcurrent, target_one, fp, handcurr);
                        continue;
                    case 13://binocular
                    case 14://bow
                    case 15://horse
                        demo_equip(pcurrent, handcurr, &discard_deck, fp);
                        continue;
                    case 16://strike
                        target1 = rand() % (num_of_player - 1);
                        target_one = pcurrent->next;
                        for (int cnt = 0;
                             cnt < target1 || target_one->dead_or_alive == 0 || target_one == pcurrent; ++cnt) {
                            target_one = target_one->next;
                        }
//                        fflush(fp);
                        if (demo_strike(pPlayer, pcurrent, target_one, &num_of_alive, num_of_player, fp, &discard_deck,
                                        handcurr) == END_OF_GAME)
                            end_of_game = END_OF_GAME;
                        continue;
                    case 17://peach
                        demo_peach(pcurrent, handcurr, &discard_deck, fp);
                        continue;
                    case 19://wine
                        demo_wine(pcurrent, handcurr, &discard_deck, fp);
                    default:
                        continue;
                }
            } while (choice != 0 && end_of_game != END_OF_GAME && selfdead == 0);
            if (selfdead == 1 || end_of_game == END_OF_GAME)
                continue;
            fprintf(fp, "Player%d ends play phase.\n", pcurrent->index);
        } else {
            fprintf(fp, "Player%d skipped play phase because of \"Drown in happiness\".\n", pcurrent->index);
            pcurrent->iplay = 1;
        }
        pcurrent->use_of_strike = 0;
        pcurrent->use_of_wine = 0;
        //play phase ends here
        // discard phase starts here.
        fprintf(fp, "Player%d's health is %d and has %d card on hand.\n", pcurrent->index, pcurrent->blood,
                pcurrent->num_of_hand);
        if (pcurrent->num_of_hand <= pcurrent->blood) {
            fprintf(fp, "Player%d doesn't need to discard any card.\n", pcurrent->index);
        } else {
            hand_uplimit = pcurrent->num_of_hand - pcurrent->blood;
            for (int i = 0; i < /*pcurrent->num_of_hand - pcurrent->blood*/hand_uplimit; ++i) {
//                fflush(fp);
                choice = rand() % (pcurrent->num_of_hand) + 1;
                handcurr = *pcurrent->hand;
                for (int j = 1; j < choice; ++j) {
                    handcurr = handcurr->next;
                }
                fprintf(fp, "Player%d discards \"%s\" in discard phase.\n", pcurrent->index, handcurr->card->name);
                discard(pcurrent, handcurr, &discard_deck);
                free(handcurr);
            }

        }
        //discard ends here.
    }
    Handnode *curr;
    Handnode *temp;
    fprintf(fp, "Game over!\n");
    for (int j = 0; j < num_of_player; ++j, current = current->next) {
        if (current->dead_or_alive == 1) {
            fprintf(fp, "Player%d wins.\n", current->index);
            curr = *current->hand;
            for (int i = 0; i < current->num_of_hand; ++i) {
                push_deck(&discard_deck, curr->card);
                delete_handnode(current->hand, curr);
                temp = curr;
                curr = curr->next;
                free(temp);
            }
            for (int k = 0; k < 3; ++k) {
                if (current->equip[k] != NULL)
                    push_deck(&discard_deck, current->equip[k]);
            }
            for (int l = 0; l < 3; ++l) {
                if (current->fate[l] != NULL)
                    push_deck(&discard_deck, current->fate[l]);
            }
        }
    }
    for (int m = 0; m < discard_deck->num; ++m) {
        free(discard_deck->cards[m]);
    }
    free(discard_deck);
    for (int m = 0; m < deck->num; ++m) {
        free(deck->cards[m]);
    }
    free(deck);

    free_players(pPlayer);
    free(pPlayer);
    fclose(fp);
}

// the meaning of the return value of demo_strike(): 0 means unsuccessfully used strike,
// 1  means successfully used . END_OF_GAME means the end of the game
int demo_strike(Player **pPlayer, Player *attack, Player *defense, int *num_of_alive, int num_of_player, FILE *fp,
                Deck **discard_pile,
                Handnode *strike) {
    int range = 1;
    int reply = 0;
    Handnode *current = *defense->hand;
    if (attack->equip[2] != NULL)
        range++;
    if (distance(attack, defense) <= range && attack->use_of_strike == 0) {
        discard(attack, strike, discard_pile);
        free(strike);
        attack->use_of_strike++;
        fprintf(fp, "Player%d uses a \"Strike\" to Player%d.\n", attack->index, defense->index);
        if (current != NULL)
            for (int i = 0; i < defense->num_of_hand; ++i, current = current->next) {
                if (current->card->iname == 18) {
                    reply = 1;
                    break;
                }
            }
        if (reply == 1) {
            discard(defense, current, discard_pile);
            free(current);
            fprintf(fp, "Player%d uses a \"Dodge\" to prevent damage.\n", defense->index);
        } else {
            if (attack->use_of_wine == 1)
                defense->blood -= 2;
            else
                defense->blood--;
            fprintf(fp, "Player%d doesn't have a \"Dodge\" and takes %d damage.\n"
                        "Player%d's health becomes %d.\n", defense->index,
                    attack->use_of_wine + 1, defense->index, defense->blood);
            if (defense->blood <= 0)
                if (demo_dying(pPlayer, attack, defense, num_of_alive, num_of_player, fp, discard_pile) ==
                    END_OF_GAME) {
                    return END_OF_GAME;
                }

        }
    } else
        return 0;
    return 1;
}


int demo_dying(Player **pPlayer, Player *main, Player *die, int *num_of_alive, int num_of_player, FILE *fp,
               Deck **discard_pile) {
    fprintf(fp, "Player%d enters dying phase.\n", die->index);
    Player *current = main;
    Handnode *curr;
    int reply = 0;
    Handnode *temp;
    for (int i = 0; i < num_of_player; ++i, current = current->next) {
        if (current->dead_or_alive == 0)
            continue;
        //if the current player is not the player who is dying.
        if (current != die) {
            curr = *current->hand;
            if (curr != NULL)
                for (int j = 0; j < current->num_of_hand; ++j) {
                    if (curr->card->iname == 17) {//if the player has a peach on his hand
                        reply = rand() % 2;// he decides whether to use it or not.
                        if (reply == 1) {
                            fprintf(fp, "Player%d uses a \"Peach\" to try to save player%d.\n", current->index,
                                    die->index);
                            discard(current, curr, discard_pile);
                            temp = curr;
                            curr = curr->next;
                            free(temp);
                            die->blood++;
                            if (die->blood >= 1) {
                                fprintf(fp, "Player%d is saved.\n", die->index);
                                return 0;
                            }
                        } else
                            curr = curr->next;
                    } else
                        curr = curr->next;
                }
        }//if the current player is not the player who is dying.

            //if the current player is the one who is dying.
        else {
            curr = *current->hand;
            if (curr != NULL)
                for (int j = 0; j < current->num_of_hand; ++j) {
                    if (curr->card->iname == 17 || curr->card->iname == 19) {
                        discard(current, curr, discard_pile);
                        fprintf(fp, "Player%d uses a \"%s\" to try to save himself.\n", current->index,
                                curr->card->name);
                        temp = curr;
                        curr = curr->next;
                        free(temp);
                        current->blood++;
                        if (die->blood >= 1) {
                            fprintf(fp, "Player%d is saved.\n", die->index);
                            return 0;
                        }
                    } else {
                        curr = curr->next;
                    }
                }
        }
    }
    fprintf(fp, "Player%d dies.\n", die->index);
    player_die(pPlayer, die, discard_pile, num_of_alive);
    if (Game_over(pPlayer, num_of_player, num_of_alive) == END_OF_GAME)
        return END_OF_GAME;
    return 1;
}

int demo_dismantle(Player *attack, Player *defense, Deck **discard_deck, FILE *fp, Handnode *dismantle) {
    int num_of_equip = 0;
    int array[3] = {-1, -1, -1};
    for (int i = 0; i < 3; ++i) {
        if (defense->equip[i] != NULL) {
            array[num_of_equip] = i;
            num_of_equip++;
        }
    }
    if (num_of_equip == 0)
        return 0;
    discard(attack, dismantle, discard_deck);
    free(dismantle);
    int index_to_discard = rand() % num_of_equip;
    push_deck(discard_deck, defense->equip[array[index_to_discard]]);
    defense->equip[array[index_to_discard]] = NULL;
    fprintf(fp, "Player%d uses a \"Dismantle\" against player%d and discards his ", attack->index, defense->index);
    switch (array[index_to_discard]) {
        case 0:
            fprintf(fp, "Horse.\n");
            break;
        case 1:
            fprintf(fp, "Bow.\n");
            break;
        case 2:
            fprintf(fp, "Binocular.\n");
            break;
        default:
            break;
    }
    return 1;
}

int demo_snatch(Player *attack, Player *defense, Deck **discard_deck, FILE *fp, Handnode *snatch) {
    if (distance(attack, defense) > 1)
        return 0;
    int num_of_equip = 0;
    int array[3] = {-1, -1, -1};
    for (int i = 0; i < 3; ++i) {
        if (defense->equip[i] != NULL) {
            array[num_of_equip] = i;
            num_of_equip++;
        }
    }
    if (num_of_equip == 0)
        return 0;
    discard(attack, snatch, discard_deck);
    free(snatch);
    int index_to_steal = rand() % num_of_equip;
    attack->num_of_hand++;
    append_handnode(attack->hand, defense->equip[array[index_to_steal]]);
    defense->equip[array[index_to_steal]] = NULL;
    fprintf(fp, "Player%d uses a \"Snatch\" against player%d and steals his ", attack->index, defense->index);
    switch (array[index_to_steal]) {
        case 0:
            fprintf(fp, "Horse.\n");
            break;
        case 1:
            fprintf(fp, "Bow.\n");
            break;
        case 2:
            fprintf(fp, "Binocular.\n");
            break;
        default:
            break;
    }
}

//1 for no one dies ;2 for other player die ; 3 for self dead;END_OF_GAME for the end of the game;
int
demo_duel(Player **pPlayer, Player *attack, Player *defense, Deck **discard_deck, int *num_of_alive, int num_of_player,
          FILE *fp,
          Handnode *duel) {
    int end = 0;
    int change = 0;
    int dead;
    Player *pcurrent = defense;
    discard(attack, duel, discard_deck);
    free(duel);
    fprintf(fp, "Player%d use a \"Duel\" against player%d.\n", attack->index, defense->index);
    Handnode *handcurrent;
    while (end == 0) {
        handcurrent = *pcurrent->hand;
        change = 0;
        if (handcurrent != NULL)
            for (int i = 0; i < pcurrent->num_of_hand; ++i, handcurrent = handcurrent->next) {
                if (handcurrent->card->iname == 16) {//It's a strike
                    discard(pcurrent, handcurrent, discard_deck);
                    free(handcurrent);
                    fprintf(fp, "Player%d uses a \"Strike\".\n", pcurrent->index);
                    change = 1;
                    break;
                }
            }
        if (change == 0)
            end = 1;
        else if (pcurrent == attack)
            pcurrent = defense;
        else if (pcurrent == defense)
            pcurrent = attack;
    }
    fprintf(fp, "Player%d doesn't have a \"Strike\" and takes one damage.\n"
                "Player%d's health becomes %d.\n", pcurrent->index, pcurrent->index, pcurrent->blood - 1);
    pcurrent->blood--;
    if (pcurrent == attack)// the attack player is dying
        dead = 0;
    else// the defense player is dying .
        dead = 1;
    if (pcurrent->blood <= 0) {
        if (demo_dying(pPlayer, attack, pcurrent, num_of_alive, num_of_player, fp, discard_deck) == 0)//no one dead
            return 1;
        else {
            if (Game_over(pPlayer, num_of_player, num_of_alive) == END_OF_GAME)
                return END_OF_GAME;
            else if (dead == 0)
                return 3;
            else
                return 2;
        }
    }
    return 1;
}

int demo_some_for_no(Player *player, Deck **pDeck, Deck **discard_deck, FILE *fp, Handnode *some_for_no) {
    discard(player, some_for_no, discard_deck);
    free(some_for_no);
    fprintf(fp, "Player%d uses a \"Something for nothing\" and draws two cards.\n", player->index);
    int n = draw(player, pDeck, 2);
    if (n != -1) {
        deck_reshuffle(pDeck, discard_deck);
        draw(player, pDeck, 2 - n);
    }
    return 1;
}

int demo_AOE(Player **pPlayer, Player *main, Deck **discard_deck, FILE *fp, int *num_of_alive, int num_of_player,
             Handnode *aoe) {
    int card = 0;
    int counter = 0;
    int survive = 0;
    char a[30] = {'\0'};
    strcpy(a, aoe->card->name);
    if (aoe->card->iname == 5)//arrow barrage
    {
        card = 5;
        strcpy(a, "Dodge");
    } else {
        card = 6;
        strcpy(a, "Strike");
    }//barbarian invasion
    if (card == 5) {
        counter = 18;//Dodge
    } else//strike
        counter = 16;
    fprintf(fp, "Player%d uses a \"%s\".\n", main->index, aoe->card->name);
    discard(main, aoe, discard_deck);
    free(aoe);
    Player *pcurrent = main->next;
    Handnode *handcurr;
    for (int i = 0; i < num_of_player - 1; i++) {
        survive = 0;
        if (pcurrent->dead_or_alive == 1) {
            handcurr = *pcurrent->hand;
            if (handcurr != NULL)
                for (int j = 0; j < pcurrent->num_of_hand; ++j, handcurr = handcurr->next) {
                    if (handcurr->card->iname == counter) {
                        fprintf(fp, "Player%d uses a \"%s\" to prevent damage.\n", pcurrent->index,
                                handcurr->card->name);
                        discard(pcurrent, handcurr, discard_deck);
                        free(handcurr);
                        survive = 1;
                        break;
                    }
                }
            pcurrent = pcurrent->next;
            if (survive == 0) {
                pcurrent->prev->blood--;
                fprintf(fp, "Player%d doesn't have a \"%s\" and takes one damage.\n"
                            "Player%d's health becomes %d.\n", pcurrent->prev->index, a, pcurrent->prev->index,
                        pcurrent->prev->blood);
                if (pcurrent->prev->blood <= 0) {
                    if (demo_dying(pPlayer, main, pcurrent->prev, num_of_alive, num_of_player, fp, discard_deck) == 0)
                        continue;
                    else if (Game_over(pPlayer, num_of_player, num_of_alive) == END_OF_GAME)
                        return END_OF_GAME;
                }
            }
        }
    }
    return 1;
}

int demo_peach(Player *player, Handnode *peach, Deck **discard_deck, FILE *fp) {//0 means failure ;1 means success
    if (player->blood == player->max_blood)
        return 0;
    player->blood++;
    discard(player, peach, discard_deck);
    free(peach);
    fprintf(fp, "Player%d uses a \"Peach\" to restore one health.\n", player->index);
    return 1;
}

int demo_wine(Player *player, Handnode *wine, Deck **discard_deck, FILE *fp) {//0 means failure ;1 means success
    if (player->use_of_wine == 1)
        return 0;
    player->use_of_wine++;
    discard(player, wine, discard_deck);
    free(wine);
    fprintf(fp, "Player%d uses a \"Wine\". His next Strike this turn will cause two damage.\n", player->index);
    return 1;
}

int demo_equip(Player *player, Handnode *equip, Deck **discard_deck, FILE *fp) {//1 for success, 0 for fail
    int index;
    switch (equip->card->iname) {
        case 13://Binocular
            index = 2;
            break;
        case 14://Bow
            index = 1;
            break;
        case 15://Horse
            index = 0;
            break;
        default:
            return 0;
    }
    fprintf(fp, "Player%d equips a \"%s\"", player->index, equip->card->name);
    if (player->equip[index] != NULL) {
        push_deck(discard_deck, player->equip[index]);
        fprintf(fp, " and discards his original \"%s\".\n", equip->card->name);
    } else
        fprintf(fp, ".\n");
    player->equip[index] = equip->card;
    delete_handnode(player->hand, equip);
    free(equip);
    player->num_of_hand--;
    return 1;
}

int demo_lightning(Player *player, Handnode *lightning, FILE *fp) {
    for (int i = 0; i < player->top_fate; ++i) {
        if (player->fate[i]->iname == 10)//there is a lightning existing
            return 0;
    }
    push_fatecard(player, lightning->card);
    delete_handnode(player->hand, lightning);
    free(lightning);
    player->num_of_hand--;
    fprintf(fp, "Player%d uses a \"Lightning\" and put it into his own fate zone.\n", player->index);
    return 1;
}

int demo_drown(Player *attack, Player *defense, Handnode *drown, FILE *fp) {
    for (int i = 0; i < defense->top_fate; ++i) {
        if (defense->fate[i]->iname == 11) {//there exists a drown
            return 0;
        }
    }
    push_fatecard(defense, drown->card);
    delete_handnode(attack->hand, drown);
    free(drown);
    attack->num_of_hand--;
    fprintf(fp, "Player%d uses a \"Drown in happiness\" and put it into player%d's fate zone.\n", attack->index,
            defense->index);
    return 1;
}

int demo_starve(Player *attack, Player *defense, FILE *fp, Handnode *starve) {
    if (distance(attack, defense) > 1)
        return 0;
    for (int i = 0; i < defense->top_fate; ++i) {
        if (defense->fate[i]->iname == 12) {//there exists a starve
            return 0;
        }
    }
    push_fatecard(defense, starve->card);
    delete_handnode(attack->hand, starve);
    free(starve);
    attack->num_of_hand--;
    fprintf(fp, "Player%d uses a \"Starvation\" and put it into player%d's fate zone.\n", attack->index,
            defense->index);
    return 1;
}

int demo_peach_garden(Player *main, int num_of_player, Handnode *peach_garden, Deck **discard_deck, FILE *fp) {
    fprintf(fp, "Player%d uses a \"Peach garden\".\n", main->index);
    discard(main, peach_garden, discard_deck);
    free(peach_garden);
    Player *current = main;
    for (int i = 0; i < num_of_player; ++i, current = current->next) {
        if (current->dead_or_alive == 0)
            continue;
        if (current->blood < current->max_blood) {
            fprintf(fp, "Player%d recovers one health.\n", current->index);
            current->blood++;
        }
    }
    return 1;
}

int demo_harvest(Player *main, int num_of_player, Handnode *harvest, Deck **draw_deck, Deck **discard_deck, FILE *fp) {
    discard(main, harvest, discard_deck);
    free(harvest);
    fprintf(fp, "Player%d uses a \"Bountiful harvest\".\n", main->index);
    Player *current = main;
    Card *temp;
    for (int i = 0; i < num_of_player; ++i, current = current->next) {
        if (current->dead_or_alive == 0) {
            continue;
        }
        if ((*draw_deck)->num == 0) {
            deck_reshuffle(draw_deck, discard_deck);
        }
        temp = pop_deck(draw_deck);
        fprintf(fp, "Player%d gets \"%s\".\n", current->index, temp->name);
        append_handnode(current->hand, temp);
        current->num_of_hand++;
    }
    return 1;
}

int demo_borrow_sword(Player **pPlayer, Player *user, Player *attack, Player *defense, Deck **discard_deck, FILE *fp,
                      int *num_of_alive, int num_of_player,
                      Handnode *borrow) {//0 for unsuccessful, 1 for successful, END_OF_GAME
    int range = 2;
    int whether_to_strike;
    if (attack->equip[1] == NULL || distance(attack, defense) <= range)
        return 0;
    discard(user, borrow, discard_deck);
    free(borrow);
    fprintf(fp, "Player%d uses a \"Borrowed sword\", ordering player%d to strike player%d.\n", user->index, attack->index,
            defense->index);
    Handnode *handcurr = *attack->hand;
    if (handcurr != NULL)
        for (int i = 0; i < attack->num_of_hand; ++i, handcurr = handcurr->next) {
            if (handcurr->card->iname == 16) {//it's a strike
                whether_to_strike = rand() % 2;//decide whether to strike or not
                // 0 for no ; 1 for yes;
                if (whether_to_strike == 1) {
                    if (demo_strike(pPlayer, attack, defense, num_of_alive, num_of_player, fp, discard_deck,
                                    handcurr) == END_OF_GAME)
                        return END_OF_GAME;
                    else if (user->dead_or_alive == 0){
                        attack->use_of_strike=0;
                        return 2;
                    }
                    else{
                        attack->use_of_strike=0;
                        return 1;
                    }
                } else
                    break;
            }
        }//below will deal with those who doesn't have a strike or don't want to use a strike.
    fprintf(fp, "Player%d doesn't use a \"Strike\" and gives his \"Bow\" to player%d.\n", attack->index, user->index);
    user->num_of_hand++;
    append_handnode(user->hand, attack->equip[1]);
    attack->equip[1] = NULL;
    return 1;
}

int
deal_fate_cards(Player **pPlayer, Player *player, Deck **discard_deck, Deck **draw_deck, FILE *fp, int *num_of_alive,
                int num_of_player) {
    Card *temp = pop_fatezone(player);
    Card *fate;
    Player *curr;
    for (; temp != NULL; temp = pop_fatezone(player)) {
        fprintf(fp, "Player%d has a \"%s\" in his fate zone.\n", player->index, temp->name);
        switch (temp->iname) {
            case 10://lightning
                fate = pop_deck(draw_deck);
                if (fate == NULL) {
                    deck_reshuffle(draw_deck, discard_deck);
                    fate = pop_deck(draw_deck);
                }
                fprintf(fp, "The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                if (strcmp(fate->suit, "Spade") == 0 && fate->inum >= 1 && fate->inum <= 8) {
                    push_deck(discard_deck, fate);
                    push_deck(discard_deck, temp);
                    fprintf(fp, "Player%d receives 3 damage.\n", player->index);
                    player->blood -= 3;
                    if (player->blood <= 0)
                        switch (demo_dying(pPlayer, player, player, num_of_alive, num_of_player, fp, discard_deck)) {
                            case 0:
                                continue;//player was saved.
                            case 1:
                                return 1;// player dead
                            case END_OF_GAME:
                                return END_OF_GAME;
                            default:
                                break;
                        }
                } else {
                    push_deck(discard_deck, fate);
                    for (curr = player->next; curr->dead_or_alive == 0; curr = curr->next);
                    push_fatecard(curr, temp);
                    fprintf(fp, "Player%d doesn't get damage and passes the \"Lightning\" to the next player.\n",
                            player->index);
                }
                break;
            case 11://drown
                fate = pop_deck(draw_deck);
                if (fate == NULL) {
                    deck_reshuffle(draw_deck, discard_deck);
                    fate = pop_deck(draw_deck);
                }
                fprintf(fp, "The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                if (strcmp(fate->suit, "Heart") != 0) {
                    player->iplay = 0;
                    fprintf(fp, "Player%d won't have the play phase this turn.\n", player->index);
                } else {
                    fprintf(fp, "Player%d is not affected.\n", player->index);
                }
                push_deck(discard_deck, fate);
                push_deck(discard_deck, temp);
                break;
            case 12://starve
                fate = pop_deck(draw_deck);
                if (fate == NULL) {
                    deck_reshuffle(draw_deck, discard_deck);
                    fate = pop_deck(draw_deck);
                }
                fprintf(fp, "The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                if (strcmp(fate->suit, "Club") != 0) {
                    player->idraw = 0;
                    fprintf(fp, "Player%d won't have the draw phase this turn.\n", player->index);
                } else {
                    fprintf(fp, "Player%d is not affected.\n", player->index);
                }
                push_deck(discard_deck, fate);
                push_deck(discard_deck, temp);
                break;
            default:
                break;
        }
    }
    return 0;
}