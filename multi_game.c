//
// Created by Wangdawei on 2018/11/11.
//

#include <stdlib.h>
#include <string.h>
#include "multi_game.h"
#include "game.h"

#define HISTORY "History.txt"
//void discard(Player *player, Handnode *handnode, Deck **discard_deck);

void multi_Game(Deck *deck) {
    FILE *fp_history = fopen(HISTORY, "w");
    int reveal_or_not;
    int identity_index = 4;
    deck_shuffle(&deck);
    int end_of_game = 0;
    int hand_uplimit;
    int choice;
    int target_choice;
    int hand_choice;
    int discard_choice;
    int num_of_player = 0;
    Handnode *handcurr;
    Deck *discard_deck = calloc(1, sizeof(Deck));
    int selfdead;
    int c;
    int temp_draw;
    Player *target1;
    Player *target2;
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
    do {
        printf("Please input the number of players(2-10): ");
        scanf("%d", &num_of_player);
        if (!(num_of_player >= 2 && num_of_player <= 10)) {
            printf("Invalid input! Please input an integer greater than 2 and less than 10.\n");
        }
    } while (!(num_of_player >= 2 && num_of_player <= 10));
    int num_of_alive = num_of_player;
    Player **pPlayer = calloc(1, sizeof(Player *));
    Player *new;
    for (int i = 0; i < num_of_player; ++i) {
        new = initialize_player();
        new->index = i + 1;
        player_append(pPlayer, new);
    }
    Player *curr = *pPlayer;
    for (int j = 0; j < num_of_player; ++j, curr = curr->next) {
        draw(curr, &deck, 4);
    }
    puts("Now game started.");
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
    cls();
    for (Player *pcurrent = *pPlayer; end_of_game != END_OF_GAME; pcurrent = pcurrent->next) {
        selfdead = 0;
        if (pcurrent->dead_or_alive == 0)
            continue;
        change_player(pcurrent);
        switch (deal_fate_cards_multi(pPlayer, pcurrent, &discard_deck, &deck, fp_history, &num_of_alive,
                                      num_of_player)) {
            case 1://player dead
                continue;
            case 0://
                break;
            case END_OF_GAME:
                end_of_game = END_OF_GAME;
                continue;
            default:
                break;
        }// fate phase

        if (pcurrent->idraw == 1) {
            temp_draw = draw(pcurrent, &deck, 2);
            if (temp_draw != -1) {
                deck_reshuffle(&deck, &discard_deck);
                draw(pcurrent, &deck, 2 - temp_draw);
            }
            fprintf(fp_history, "Player%d draws 2 cards.\n", pcurrent->index);
        } else {
            fprintf(fp_history, "Player%d skips his draw phase because of \"Starvation\".\n", pcurrent->index);
            printf("You skip your draw phase because of \"Starvation\".\n");
            pcurrent->idraw = 1;
        }//draw phase

        if (pcurrent->iplay == 1) {
            choice = -1;
            printf("You enter your play phase.\n");
            do {
                while (1) {
                    printf("You have the following options.\n");
                    printf("0) End your play phase.\n");
                    printf("1) Show the information about all the players alive.\n");
                    printf("2) Use a card from your hand.(This will show the cards on your hand).\n");
                    printf("3) Reveal your identity.\n");
                    printf("4) Show the history of the game.\n");
                    printf("5) Clear the screen.\n");
                    printf("Choice: ");
                    scanf("%d", &choice);
                    if (choice >= 0 && choice <= 5)
                        break;
                    puts("That's not a valid choice!");
                    puts("Please input a number between 0 and 3!");
                }
                if (choice == 0) {
                    if(confirm()==0)
                        continue;
                    printf("You have ended your play phase.\n");
                    break;
                } else if (choice == 1) {
                    show_field(pcurrent, num_of_player);
                } else if (choice == 3) {
                    if (pcurrent->country_reveal == 0) {
//                        reveal_or_not = rand() % 2;
                        while (1) {
                            printf("You haven't revealed your identity yet.\n");
                            puts("Input 1 to reveal or 0 to quit.");
                            scanf("%d", &reveal_or_not);
                            if (reveal_or_not == 1 || reveal_or_not == 0)
                                break;
                            printf("Invalid Input!\n");
                            printf("Please input 0 or 1.\n");
                        }
                        if (reveal_or_not == 0)
                            continue;
                        else {
                            if (reveal_identity(pPlayer, pcurrent, num_of_player) == 1) {
                                pcurrent->country = "Mer";
                                pcurrent->icountry = identity_index;
                                identity_index++;
                                printf("You have revealed your identity as a Mercenary.\n");
                                printf("Your goal is to kill all other players.\n");
                                fprintf(fp_history, "Player%d reveals his identity as a Mercenary.\n", pcurrent->index);
                            } else {
                                fprintf(fp_history, "Player%d reveals his identity as a General of \"%s\".\n",
                                        pcurrent->index,
                                        pcurrent->country);
                                printf("You have revealed your identity as a General of \"%s\".\n", pcurrent->country);
                                printf("Your goal is to ally with other generals of \"%s\" and eliminate all players "
                                       "that are not the same country with you.\n", pcurrent->country);
                            }
                            continue;
                        }
                    } else {
                        printf("You have already revealed your identity.\n");
                        printf("Please choose another option.\n");
                    }
                } else if (choice == 2) {//use of hand
                    hand_choice = -1;
                    while (1) {
                        disp_hand(pcurrent);
                        printf("Please input the number before the card you want to use.\n");
                        puts("Input 0 to go back.");
                        printf("Choice: ");
                        scanf("%d", &hand_choice);
                        if (hand_choice >= 0 && hand_choice <= pcurrent->num_of_hand)
                            break;
                        puts("Invalid input!");
                        printf("Please input a number between 0 and %d.\n", pcurrent->num_of_hand);
                    }
                    if (hand_choice == 0) {
                        continue;
                    }
                    handcurr = *pcurrent->hand;
                    for (int i = 1; i < hand_choice; ++i, handcurr = handcurr->next);
                    switch (handcurr->card->iname) {
                        case 1://duel
                            switch (multi_duel(pPlayer, pcurrent, &discard_deck, &num_of_alive, num_of_player,
                                               fp_history,
                                               handcurr)) {
                                case END_OF_GAME:
                                    end_of_game = END_OF_GAME;
                                    break;
                                case 3:
                                    selfdead = 1;
                                default:
                                    break;
                            }
                            continue;
                        case 2://dismantle
                            while (1) {
                                target_choice = -1;
                                puts("Who do you want to dismantle?");
                                printf("Please input the index of the player.\n");
                                puts("Input 0 to give up dismantling.");
                                printf("Choice: ");
                                scanf("%d", &target_choice);
                                if (target_choice < 0 || target_choice > num_of_player) {
                                    puts("Invalid input!");
                                    printf("Please input a number between 0 and %d.\n", num_of_player);
                                    continue;
                                } else if (target_choice == 0) {
                                    if (confirm() == 0)
                                        continue;
                                    break;
                                } else {
                                    target1 = *pPlayer;
                                    for (int i = 1; i < target_choice; ++i, target1 = target1->next);
                                    if (target1 == pcurrent) {
                                        printf("You can't dismantle yourself.\n");
                                        continue;
                                    } else if (target1->dead_or_alive == 0) {
                                        printf("Player%d is a dead man.\nYou cannot dismantle a dead man!\n",
                                               target1->index);
                                        continue;
                                    } else if (confirm() == 0) {
                                        continue;
                                    } else {
                                        multi_dismantle(pcurrent, target1, &discard_deck, fp_history, handcurr);
                                        break;
                                    }
                                }
                            }
                            continue;
                        case 3://snatch
                            while (1) {
                                target_choice = -1;
                                puts("Who do you want to snatch?");
                                printf("Please input the index of the player.\n");
                                puts("Input 0 to give up snatching.");
                                printf("Choice: ");
                                scanf("%d", &target_choice);
                                if (target_choice < 0 || target_choice > num_of_player) {
                                    puts("Invalid input!");
                                    printf("Please input a number between 0 and %d.\n", num_of_player);
                                    continue;
                                } else if (target_choice == 0) {
                                    if (confirm() == 0)
                                        continue;
                                    break;
                                } else {
                                    target1 = *pPlayer;
                                    for (int i = 1; i < target_choice; ++i, target1 = target1->next);
                                    if (target1 == pcurrent) {
                                        printf("You can't snatch yourself.\n");
                                        continue;
                                    } else if (target1->dead_or_alive == 0) {
                                        printf("Player%d is a dead man.\nYou cannot snatch a dead man!\n",
                                               target1->index);
                                        continue;
                                    } else if (confirm() == 0) {
                                        continue;
                                    } else {
                                        multi_snatch(pcurrent, target1, &discard_deck, fp_history, handcurr);
                                        break;
                                    }
                                }
                            }
                            continue;
                        case 4://borrow sword
                            switch (multi_borrow_sword(pPlayer, pcurrent, &discard_deck, fp_history, &num_of_alive,
                                                       num_of_player, handcurr)) {
                                case END_OF_GAME:
                                    end_of_game = END_OF_GAME;
                                    continue;
                                case 2:
                                    selfdead = 1;
                                default:
                                    break;
                            }
                            continue;
                        case 5:
                        case 6://AOE
                            if (confirm() != 0) {
                                if (multi_AOE(pPlayer, pcurrent, &discard_deck, fp_history, num_of_player,
                                              &num_of_alive, handcurr) == END_OF_GAME) {
                                    end_of_game = END_OF_GAME;
                                    continue;
                                }
//                                change_player(pcurrent);
                            }
                            continue;
                        case 7://peach garden
                            if (confirm() != 0)
                                multi_peach_garden(pcurrent, num_of_player, handcurr, &discard_deck, fp_history);
                            continue;
                        case 8://something for nothing
                            if (confirm() != 0)
                                multi_some_for_no(pcurrent, &deck, &discard_deck, fp_history, handcurr);
                            continue;
                        case 9://bountiful harvest
                            if (confirm() != 0) {
                                multi_harvest(pcurrent, num_of_player, num_of_alive, &deck, &discard_deck, fp_history,
                                              handcurr);
                                change_player(pcurrent);
                            }
                            continue;
                        case 10://lightning
                            if (confirm() != 0) {
                                multi_lightning(pcurrent, handcurr, fp_history);
                            }
                            continue;
                        case 11://drown
                            while (1) {
                                target_choice = -1;
                                puts("Who do you want to use \"Drown in happiness\" against?");
                                printf("Please input the index of the player.\n");
                                puts("Input 0 to give up using \"Drown in happiness\".");
                                printf("Choice: ");
                                scanf("%d", &target_choice);
                                if (target_choice < 0 || target_choice > num_of_player) {
                                    puts("Invalid input!");
                                    printf("Please input a number between 0 and %d.\n", num_of_player);
                                    continue;
                                } else if (target_choice == 0) {
                                    if (confirm() == 0)
                                        continue;
                                    break;
                                } else {
                                    target1 = *pPlayer;
                                    for (int i = 1; i < target_choice; ++i, target1 = target1->next);
                                    if (target1 == pcurrent) {
                                        printf("You can't use \"Drown in happiness\" to yourself.\n");
                                        continue;
                                    } else if (target1->dead_or_alive == 0) {
                                        printf("Player%d is a dead man.\nYou cannot do that!\n",
                                               target1->index);
                                        continue;
                                    } else if (confirm() == 0) {
                                        continue;
                                    } else {
                                        multi_drown(pcurrent, target1, handcurr, fp_history);
                                        break;
                                    }
                                }
                            }
                            continue;
                        case 12://starve
                            while (1) {
                                target_choice = -1;
                                puts("Who do you want to use \"Starvation\" against?");
                                printf("Please input the index of the player.\n");
                                puts("Input 0 to give up using \"Starvation\".");
                                printf("Choice: ");
                                scanf("%d", &target_choice);
                                if (target_choice < 0 || target_choice > num_of_player) {
                                    puts("Invalid input!");
                                    printf("Please input a number between 0 and %d.\n", num_of_player);
                                    continue;
                                } else if (target_choice == 0) {
                                    if (confirm() == 0)
                                        continue;
                                    break;
                                } else {
                                    target1 = *pPlayer;
                                    for (int i = 1; i < target_choice; ++i, target1 = target1->next);
                                    if (target1 == pcurrent) {
                                        printf("You can't use \"Starvation\" to yourself.\n");
                                        continue;
                                    } else if (target1->dead_or_alive == 0) {
                                        printf("Player%d is a dead man.\nYou cannot do that!\n",
                                               target1->index);
                                        continue;
                                    } else {
                                        multi_starve(pcurrent, target1, fp_history, handcurr);
                                        break;
                                    }
                                }
                            }
                            continue;
                        case 13:
                        case 14:
                        case 15://equip
                            if (confirm() != 0) {
                                multi_equip(pcurrent, handcurr, &discard_deck, fp_history);
                            }
                            continue;
                        case 16://Strike
                            if (multi_strike(pPlayer, pcurrent, &discard_deck, &num_of_alive, num_of_player, fp_history,
                                             handcurr) == END_OF_GAME) {
                                end_of_game = END_OF_GAME;
                            }
                            continue;
                        case 17://Peach
                            if (confirm() != 0)
                                multi_peach(pcurrent, handcurr, &discard_deck, fp_history);
                            continue;
                        case 19://Wine
                            if (confirm() != 0)
                                multi_wine(pcurrent, handcurr, &discard_deck, fp_history);
                            continue;
                        default:
                            printf("You can't use a \"Dodge\".\n");
                    }
                } else if (choice == 4) {// show history
                    show_history(HISTORY, &fp_history);
                } else {
                    cls();
                }
            } while (end_of_game != END_OF_GAME && selfdead != 1);
            if (end_of_game == END_OF_GAME || selfdead == 1)
                continue;
            fprintf(fp_history, "Player%d ends play phase.\n", pcurrent->index);
//            printf("You have ended your play phase.\n");
        } else {
            fprintf(fp_history, "Player%d skipped play phase because of \"Drown in happiness\".\n", pcurrent->index);
            pcurrent->iplay = 1;
            printf("You skip your play phase because of \"Drown in happiness\".\n");
        }
        pcurrent->use_of_strike = 0;
        pcurrent->use_of_wine = 0;
        fprintf(fp_history, "Player%d's health is %d and has %d card on hand.\n", pcurrent->index, pcurrent->blood,
                pcurrent->num_of_hand);
        printf("Your health is %d and has %d card on your hand.\n", pcurrent->blood, pcurrent->num_of_hand);
        if (pcurrent->num_of_hand <= pcurrent->blood) {
            fprintf(fp_history, "Player%d doesn't need to discard any card.\n", pcurrent->index);
            printf("You don't need to discard any card.\n");
        } else {
            hand_uplimit = pcurrent->num_of_hand - pcurrent->blood;
            for (int i = 0; i < hand_uplimit; ++i) {
                discard_choice = -1;
                do {
                    printf("You need to discard %d card(s).\n", hand_uplimit);
                    disp_hand(pcurrent);
                    printf("Please input the number before the card to discard it.(one at a time)\n");
                    printf("Choice: ");
                    scanf("%d", &discard_choice);
                    if (discard_choice > 0 && discard_choice <= pcurrent->num_of_hand)
                        break;
                    puts("Invalid choice!");
                    printf("Please input a number between 1 and %d.\n", pcurrent->num_of_hand);
                } while (1);
                handcurr = *pcurrent->hand;
                for (int j = 1; j < discard_choice; ++j, handcurr = handcurr->next);
                discard(pcurrent, handcurr, &discard_deck);
                printf("You have successfully discarded a(n) \"%s\".\n", handcurr->card->name);
                fprintf(fp_history, "Player%d discards \"%s\" in discard phase.\n", pcurrent->index,
                        handcurr->card->name);
                free(handcurr);
            }
        }//discard phase end

    }
    Handnode *temp;
    Handnode *current;
    printf("Game over!\n");
    curr = *pPlayer;
    for (int j = 0; j < num_of_player; ++j, curr = curr->next) {
        if (curr->dead_or_alive == 1) {
            printf("Player%d wins.\n", curr->index);
            current = *curr->hand;
            for (int i = 0; i < curr->num_of_hand; ++i) {
                push_deck(&discard_deck, current->card);
                delete_handnode(curr->hand, current);
                temp = current;
                current = current->next;
                free(temp);
            }
            for (int k = 0; k < 3; ++k) {
                if (curr->equip[k] != NULL)
                    push_deck(&discard_deck, curr->equip[k]);
            }
            for (int l = 0; l < 3; ++l) {
                if (curr->fate[l] != NULL)
                    push_deck(&discard_deck, curr->fate[l]);
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
    fclose(fp_history);
}

void cls() {
#ifdef _WIN32_WINNT
    system("cls");
#endif //_WIN32_WINNT
#ifdef __APPLE__
    system("clear");
#endif//__APPLE__
#ifdef linux
    system("clear");
#endif//linux
}

void show_field(Player *main, int num_of_player) {
    Player *current = main->next;
    printf("Player%d(you): ", main->index);
    show_blood(main);
    show_max_blood(main);
    printf("\tIdentity: ");
    if (main->country_reveal == 0)
        printf("a general of %s.(unknown to others)\n", main->country);
    else if (main->icountry >= 4)
        printf("Mercenary.\n");
    else
        printf("a general of %s.\n", main->country);
    disp_equip(main);
    disp_fate(main);
    disp_hand(main);
    for (int i = 1; i < num_of_player; i++, current = current->next) {
        if (current->dead_or_alive == 0)
            continue;
        printf("Player%d: ", current->index);
        disp_handnum(current);
        disp_identity(current);
        show_blood(current);
        show_max_blood(current);
        disp_equip(current);
        disp_fate(current);
    }
}


void show_history(const char *history, FILE **fp_history) {
    fclose(*fp_history);
    FILE *fp = fopen(history, "r");
    char line[500] = {'\0'};
    if (fp == NULL) {
        printf("Can't open!\n");
        exit(999);
    } else {
        puts("");
        while (fgets(line, 500, fp) != NULL) {
            printf("%s", line);
        }
        fclose(fp);
        puts("");
    }
    *fp_history = fopen(history, "a");
}

void disp_hand(Player *player) {
    Handnode *curr = *player->hand;
    if (*player->hand == NULL) {
        puts("You don't have any card on your hand!\n");
    } else {
        printf("You have %d card(s) on your hand.\n", player->num_of_hand);
        for (int i = 1; i <= player->num_of_hand; ++i, curr = curr->next) {
            printf("\t%d) %s\n", i, curr->card->name);
        }
    }
}

void disp_fate(Player *player) {
    printf("\tFate zone:\n");
    for (int i = 1; i <= player->top_fate; ++i) {
        printf("\t\t%d) %s\n", i, player->fate[i - 1]->name);
    }
    if (player->top_fate == 0)
        printf("\t\tNothing!\n");
}

void disp_equip(Player *player) {
    int bool_equip = 0;
    printf("\tEquipment zone: \n");
    for (int i = 0; i < 3; ++i) {
        if (player->equip[i] != NULL) {
            printf("\t\t%s\n", player->equip[i]->name);
            bool_equip = 1;
        }
    }
    if (bool_equip == 0)
        printf("\t\tNothing!\n");
}


void disp_identity(Player *player) {
    printf("\tIdentity: ");
    if (player->country_reveal == 0)
        printf("unknown.\n");
    else if (player->icountry >= 4)
        printf("Mercenary.\n");
    else
        printf("a general of %s.\n", player->country);
}

void show_blood(Player *player) {
    printf("\tBlood: %d\n", player->blood);
}

//0: safe; 1: player dead; END_OF_GAME: end of game;
int
deal_fate_cards_multi(Player **pPlayer, Player *player, Deck **discard_deck, Deck **draw_deck, FILE *fp,
                      int *num_of_alive,
                      int num_of_player) {
    Card *temp = pop_fatezone(player);
    Card *fate;
    Player *curr;
    for (; temp != NULL; temp = pop_fatezone(player)) {
        fprintf(fp, "You have a \"%s\" in your fate zone.\n", temp->name);
        fate = pop_deck(draw_deck);
        if (fate == NULL) {
            deck_reshuffle(draw_deck, discard_deck);
            fate = pop_deck(draw_deck);
        }
        switch (temp->iname) {
            case 10://lightning
                fprintf(fp, "The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                printf("The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                if (strcmp(fate->suit, "Spade") == 0 && fate->inum >= 1 && fate->inum <= 8) {
                    push_deck(discard_deck, fate);
                    push_deck(discard_deck, temp);
                    fprintf(fp, "Player%d receives 3 damage.\n", player->index);
                    player->blood -= 3;
                    fprintf(fp, "Player%d's health becomes %d.\n", player->index, player->blood);
                    printf("You receive 3 damage.\n");
                    printf("Your health becomes %d.\n", player->blood);
                    player->blood -= 3;
                    if (player->blood <= 0)
                        switch (multi_dying(pPlayer, player, player, num_of_alive, num_of_player, fp, discard_deck)) {
                            case 0:
                                change_player(player);
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
                fprintf(fp, "The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                printf("The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                if (strcmp(fate->suit, "Heart") != 0) {
                    player->iplay = 0;
                    fprintf(fp, "Player%d won't have the play phase this turn.\n", player->index);
                    printf("You won't have the play phase this turn.\n");
                } else {
                    fprintf(fp, "Player%d is not affected.\n", player->index);
                    printf("You are not affected.\n");
                }
                push_deck(discard_deck, fate);
                push_deck(discard_deck, temp);
                break;
            case 12://starve
                fprintf(fp, "The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                printf("The determination card for \"%s\" is %s %s.\n", temp->name, fate->suit, fate->num);
                if (strcmp(fate->suit, "Club") != 0) {
                    player->idraw = 0;
                    fprintf(fp, "Player%d won't have the draw phase this turn.\n", player->index);
                    printf("You won't have the draw phase this turn.\n");
                } else {
                    fprintf(fp, "Player%d is not affected.\n", player->index);
                    printf("You are not affected.\n");
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

// 0 means the dying player is saved, 1 means the dying player is dead.;END_OF_GAME means the end of the game;
int multi_dying(Player **pPlayer, Player *main, Player *die, int *num_of_alive, int num_of_player, FILE *fp,
                Deck **discard_pile) {
    fprintf(fp, "Player%d enters dying phase.\n", die->index);
    Player *pcurrent = main;
    Handnode *curr;
//    Handnode *temp;
    int choice;
    int choice2;
    int confirm_choice;
    for (int i = 0; i < num_of_player; ++i, pcurrent = pcurrent->next) {
        if (pcurrent->dead_or_alive == 0)
            continue;
        change_player(pcurrent);
        if (pcurrent == die) {
            choice = -1;
            printf("You enter dying phase.\n");
            while (choice != 0) {
                printf("Your current health level is %d.\n", pcurrent->blood);
                printf("You need %d \"Peach(es)\" or \"Wine(s)\" to save you.\n", 1 - pcurrent->blood);
                disp_hand(pcurrent);
                do {
                    printf("Please input the number before the card to use it.(Only \"Peach\" and \"Wine\" are available)\n");
                    printf("Input 0 to quit.\n");
                    printf("Choice: ");
                    scanf("%d", &choice);
                    if (choice < 0 || choice > pcurrent->num_of_hand) {
                        printf("Invalid input!\n");
                        printf("Choose again!\n");
                    }
                } while (choice < 0 || choice > pcurrent->num_of_hand);
                if(choice==0){
                    break;
                }
                curr = *pcurrent->hand;
                for (int j = 1; j < choice; ++j) {
                    curr = curr->next;
                }
                if (curr->card->iname == 17 || curr->card->iname == 19) {
                    pcurrent->blood++;
                    fprintf(fp, "Player%d uses a \"%s\" to save himself.\n", pcurrent->index, curr->card->name);
                    printf("You have successfully used a \"%s\" to try to save yourself.\n", curr->card->name);
                    discard(pcurrent, curr, discard_pile);
                    free(curr);
                    fprintf(fp, "Now player%d's health is %d.\n", pcurrent->index, pcurrent->blood);
                    if (pcurrent->blood > 0) {
                        fprintf(fp, "Player%d is saved.\n", pcurrent->index);
                        printf("You are saved!\n");
//                        change_player(main);
                        return 0;
                    }
                } else {
                    printf("That's not a \"Peach\" or a \"Wine\"!\n");
                    puts("Please try again!");
                }
            }
        } else {
            choice = -1;
            choice2 = -2;
            printf("Player%d enters dying phase.\n", die->index);
            while (choice != 0) {
                do {
                    printf("Player%d's health is %d.\n", die->index, die->blood);
                    printf("Player%d will need %d \"Peach(es)\" to be alive.\n", die->index, 1 - die->blood);
                    printf("You have the following options.\n");
                    printf("0) To give up saving player%d.\n", die->index);
                    printf("1) Show player%d's information.\n", die->index);
                    printf("2) Show all the information about all players alive.\n");
                    printf("3) Show history.\n");
                    printf("4) Choose a \"Peach\" in your hand and try to save player%d.\n", die->index);
                    printf("Please enter a number representing your choice: ");
                    scanf("%d", &choice);
                    if (choice < 0 || choice > 4) {
                        printf("Invalid choice!\nPlease enter a number between 0 and 3.\n");
                    }
                } while (choice < 0 || choice > 3);
                switch (choice) {
                    case 0:
                        confirm_choice = confirm();
                        if (confirm_choice != 1)
                            choice = -1;
                        break;
                    case 1:
                        printf("Player%d: \n", die->index);
                        show_blood(die);
                        disp_identity(die);
                        disp_fate(die);
                        disp_equip(die);
                        break;
                    case 2:
                        show_field(pcurrent, num_of_player);
                        break;
                    case 4://choose from one's hand.
//                        curr = *pcurrent->hand;
                        while (1) {
                            curr = *pcurrent->hand;
                            do {
                                printf("Player%d's health is %d", die->index, die->blood);
                                printf("Player%d will need %d \"Peach(es)\" to be alive.\n", die->index,
                                       1 - die->blood);
                                disp_hand(pcurrent);
                                printf("Please input the number before the card to use it.(Only \"Peach\" is available)\n");
                                printf("Input 0 to quit.\n");
                                printf("Choice: ");
                                scanf("%d", &choice2);
                                if (choice2 < 0 || choice2 > pcurrent->num_of_hand) {
                                    printf("Invalid choice!\nPlease enter a number between 0 and %d.\n",
                                           pcurrent->num_of_hand);
                                }
                            } while (choice2 < 0 || choice2 > pcurrent->num_of_hand);
                            if (choice2 == 0)
                                break;
                            else {
                                for (int j = 1; j < choice2; ++j, curr = curr->next);
                                if (curr->card->iname == 17) {//That's a peach.
                                    confirm_choice = confirm();
                                    if (confirm_choice == 1) {
                                        fprintf(fp, "Player%d uses a \"Peach\" to try to save the dying player%d.\n",
                                                pcurrent->index, die->index);
                                        discard(pcurrent, curr, discard_pile);
                                        free(curr);
                                        die->blood++;
                                        if (die->blood >= 1) {
                                            printf("You have successfully used a \"Peach\" to save player%d.\n",
                                                   die->index);
                                            fprintf(fp, "Player%d is saved.\n", die->index);
//                                            change_player(main);
                                            return 0;
                                        }
                                    }
                                } else {//That's not a peach
                                    printf("That's not a \"Peach\"!\nPlease make a valid choice!\n");
                                }
                            }
                        }
                        continue;
                    case 3:
                        show_history(HISTORY, &fp);
                        continue;
                    default:
                        break;
                }
            }
        }
    }
    change_player(die);
    fprintf(fp, "Player%d dies.\n", die->index);
    printf("You have been killed.\n");
    player_die(pPlayer, die, discard_pile, num_of_alive);
    if (Game_over(pPlayer, num_of_player, num_of_alive) == END_OF_GAME)
        return END_OF_GAME;
    return 1;
}

void change_player(Player *player) {
    int c;
    cls();
    printf("Please get player%d in front of the screen.\n", player->index);
    printf("Press Enter to continue.\n");
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();

}

void disp_handnum(Player *player) {
    printf("%d card(s) on the hand.\n", player->num_of_hand);
}

//0 to regret; 1 to confirm
int confirm() {
    int a;
    do {
        puts("Are you sure to make that choice?");
        puts("Input 1 to confirm.\nInput 0 to go back.");
        printf("Choice: ");
        scanf("%d", &a);
        if (a != 0 && a != 1) {
            printf("Invalid input!\n");
            puts("Please input 0 or 1.");
        }
    } while (a != 0 && a != 1);
    return a;
}

// 0 means unsuccessfully used strike,
// 1  means successfully used . END_OF_GAME means the end of the game,
int multi_strike(Player **pPlayer, Player *attack, Deck **discard_deck, int *num_of_alive,
                 int num_of_player, FILE *fp, Handnode *strike) {
    if (attack->use_of_strike == 1) {
        printf("You have already used a \"Strike\" this turn and you cannot use it again!\n");
        return 0;
    }
    Player *pcurrent;
    Handnode *handcurr;
    int counter_choice;
    int choice = -1;
    int range = 1;
    if (attack->equip[1] != NULL)
        range++;
    while (1) {
        pcurrent = *pPlayer;
        do {
            puts("Who do you want to strike?");
            printf("Please input the index of the player.\n");
            puts("Input 0 to give up striking.");
            printf("Choice: ");
            scanf("%d", &choice);
            if (choice < 0 || choice > num_of_player) {
                puts("Invalid input!\n");
                printf("Please input a number between 0 and %d.\n", num_of_player);
            }
        } while (choice < 0 || choice > num_of_player);
        if (choice == 0)
            break;
        else {
            for (int i = 1; i < choice; ++i, pcurrent = pcurrent->next);
            if (pcurrent->index == attack->index) {
                printf("You can't strike yourself!\n");
                puts("Make another valid choice!");
                continue;
            } else if (pcurrent->dead_or_alive == 0) {
                printf("Player%d has already been dead. You can't strike a dead man!\n", pcurrent->index);
                puts("Make another valid choice!");
                continue;
            } else if (distance(attack, pcurrent) > range) {
                printf("You can't strike player%d.\n", pcurrent->index);
                printf("Your attack range is %d, but the distance between you and player%d is %d.\n", range,
                       pcurrent->index, distance(attack, pcurrent));
                puts("Make another valid choice!");
                continue;
            } else {//successfully used strike
                if (confirm() == 0)
                    continue;
                else {
                    attack->use_of_strike++;
                    discard(attack, strike, discard_deck);
                    free(strike);
                    fprintf(fp, "Player%d uses a \"Strike\" to player%d.\n", attack->index, pcurrent->index);
                    change_player(pcurrent);
                    while (1) {
                        printf("Player%d uses a \"Strike\" to you!\n", attack->index);
                        printf("Your current health level is %d.\n", pcurrent->blood);
                        printf("The \"Strike\" will cause %d damage.\n", attack->use_of_wine + 1);
//                        handcurr = *pcurrent->hand;
                        do {
                            disp_hand(pcurrent);
                            puts("Please input the number before the card you want to use(only \"Dodge\" is available).");
                            printf("Input 0 to give up and take %d damage.\n", attack->use_of_wine + 1);
                            scanf("%d", &counter_choice);
                            if (counter_choice < 0 || counter_choice > pcurrent->num_of_hand) {
                                puts("Invalid input!");
                                printf("Please input a number between 0 and %d.\n", pcurrent->num_of_hand);
                            }
                        } while (counter_choice < 0 || counter_choice > pcurrent->num_of_hand);
                        if (counter_choice == 0) {//don't use a dodge
                            if (confirm() == 0)
                                continue;
                            fprintf(fp, "Player%d doesn't use a \"Dodge\" and takes %d damage.\n", pcurrent->index,
                                    attack->use_of_wine + 1);
                            pcurrent->blood -= (attack->use_of_wine + 1);
                            printf("You take %d damages.\n", attack->use_of_wine + 1);
                            if (pcurrent->blood <= 0) {
                                switch (multi_dying(pPlayer, attack, pcurrent, num_of_alive, num_of_player, fp,
                                                    discard_deck)) {
                                    case END_OF_GAME:
                                        return END_OF_GAME;
                                    case 1://pcurrent is dead!
                                        change_player(attack);
                                        printf("You have successfully used a \"Strike\" to player%d and killed him.\n",
                                               pcurrent->index);
                                        return 1;
                                    case 0://pcurrent is saved
                                        change_player(attack);
                                        printf("You have successfully used a \"Strike\" to player%d but he is saved.\n",
                                               pcurrent->index);
                                        return 1;
                                    default:
                                        break;
                                }
                            } else {//pcurrent doesn't enter dying phase
                                change_player(attack);
                                printf("You have successfully used a \"Strike\" to player%d and his health level becomes %d.\n",
                                       pcurrent->index, pcurrent->blood);
                                return 1;
                            }
                        } else {//try to use a dodge
                            handcurr = *pcurrent->hand;
                            for (int i = 1; i < counter_choice; ++i, handcurr = handcurr->next);
                            if (handcurr->card->iname != 18) {
                                printf("That's not a \"Dodge\"!\n");
                                puts("Make another valid choice!");
                                continue;
                            } else {
                                fprintf(fp, "Player%d uses a \"Dodge\" to avoid damage.\n", pcurrent->index);
                                discard(pcurrent, handcurr, discard_deck);
                                free(handcurr);
                                printf("You have successfully used a \"Dodge\" to prevent damage.\n");
                                change_player(attack);
                                printf("You have successfully used a \"Strike\" to player%d but he uses a \"Dodge\" to "
                                       "prevent damage.\n", pcurrent->index);
                                return 1;
                            }
                        }
                    }
                }

            }
        }
    }
    return 0;
}

//1 for no one dies ;2 for other player die ; 3 for self dead;END_OF_GAME for the end of the game;
int multi_duel(Player **pPlayer, Player *attack, Deck **discard_deck, int *num_of_alive, int num_of_player, FILE *fp,
               Handnode *duel) {
    Player *defense;
    Handnode *handcurr;
    Player *pcurrent, *pother;
    Player *temp;
    int choice = -1;
    int hand_choice = -1;
    int confirm_choice;
//    int damage = 0;
    while (1) {
        defense = *pPlayer;
        do {
            printf("Who do you want to \"Duel\" with?\n");
            printf("Please input the index of the player.\n");
            puts("Input 0 to give up dueling.");
            printf("Choice: ");
            scanf("%d", &choice);
            if (choice < 0 || choice > num_of_player) {
                puts("Invalid input!");
                printf("Please input a number between 0 and %d.\n", num_of_player);
            }
        } while (choice < 0 || choice > num_of_player);
        if (choice == 0)
            return 1;
        else {
            for (int i = 1; i < choice; ++i, defense = defense->next);
            if (defense->index == attack->index) {
                printf("You cannot duel yourself!\n");
                puts("Make another valid choice!");
                continue;
            } else if (defense->dead_or_alive == 0) {
                printf("Player%d has already been dead. You can't duel with a dead man!\n", defense->index);
                puts("Make another valid choice!");
                continue;
            } else {//a valid target
                confirm_choice = confirm();
                if (confirm_choice == 0)
                    continue;
                else {
                    discard(attack, duel, discard_deck);
                    free(duel);
                    fprintf(fp, "Player%d uses a \"Duel\" to player%d.\n", attack->index, defense->index);
                    pcurrent = attack;
                    pother = defense;
                    do {
                        temp = pcurrent;
                        pcurrent = pother;
                        pother = temp;
                        change_player(pcurrent);
                        handcurr = *pcurrent->hand;
                        do {
                            printf("You are in a duel with player%d.\n", pother->index);
                            disp_hand(pcurrent);
                            printf("Please input the number before the card to use it.(Only \"Strike\" is available)\n");
                            printf("Input 0 to quit.\n");
                            printf("Choice: ");
                            scanf("%d", &hand_choice);
                            if (hand_choice < 0 || hand_choice > pcurrent->num_of_hand) {
                                printf("Invalid input.\n");
                                printf("Please input a number between 0 and %d.\n", pcurrent->num_of_hand);
                            }
                        } while (hand_choice < 0 || hand_choice > pcurrent->num_of_hand);
                        if (hand_choice == 0) {//someone takes one damage
//                            damage = 1;
                            fprintf(fp, "Player%d takes one damage.\n", pcurrent->index);
                            pcurrent->blood--;
                            if (pcurrent->blood <= 0) {
                                switch (multi_dying(pPlayer, attack, pcurrent, num_of_alive, num_of_player, fp,
                                                    discard_deck)) {
                                    case END_OF_GAME:
                                        return END_OF_GAME;
                                    case 0:
                                        change_player(attack);
                                        return 1;
                                    case 1:
                                        if (pcurrent == attack) {
                                            return 3;
                                        } else {
                                            change_player(attack);
                                            return 2;
                                        }
                                    default:
                                        break;
                                }
                            }
                            change_player(attack);
                            return 1;
                        } else {
                            for (int i = 1; i < hand_choice; ++i, handcurr = handcurr->next);
                            if (handcurr->card->iname != 16) {
                                printf("That's not a \"Strike\"!\n");
                                printf("Please make another valid choice!\n");
                                continue;
                            } else {
                                discard(pcurrent, handcurr, discard_deck);
                                free(handcurr);
                                fprintf(fp,"Player%d uses a \"Strike\".\n",pcurrent->index);
                            }
                        }
                    } while (1);
                }
            }
        }
    }
}

//0 for unsuccessful;1 for successful
void multi_wine(Player *player, Handnode *wine, Deck **discard_deck, FILE *fp) {
    if (player->use_of_wine == 1) {
        printf("You have used a \"Wine\" this turn so you can't use it again.\n");
        return;
    }
    player->use_of_wine++;
    discard(player, wine, discard_deck);
    free(wine);
    fprintf(fp, "Player%d uses a \"Wine\". His next Strike this turn will cause two damage.\n", player->index);
    printf("You use a \"Wine\". Your next \"Strike\" this turn will cause two damages.\n");
}

void multi_peach(Player *player, Handnode *peach, Deck **discard_deck, FILE *fp) {
    if (player->blood == player->max_blood) {
        puts("Your current health level is your maximum health level and you cannot increase it.\n");
        return;
    }
    player->blood++;
    discard(player, peach, discard_deck);
    free(peach);
    fprintf(fp, "Player%d uses a \"Peach\" to restore one health.\n", player->index);
    printf("You successfully use a \"Peach\" to restore one health.\n");
}

void multi_equip(Player *player, Handnode *equip, Deck **discard_deck, FILE *fp) {
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
            return;
    }
    fprintf(fp, "Player%d equips a \"%s\"", player->index, equip->card->name);
    printf("You have equipped a \"%s\"", equip->card->name);
    if (player->equip[index] != NULL) {
        push_deck(discard_deck, player->equip[index]);
        fprintf(fp, " and discards his original \"%s\".\n", equip->card->name);
        printf(" and discarded your original \"%s\".\n", equip->card->name);
    } else {
        fprintf(fp, ".\n");
        puts(".");
    }
    player->equip[index] = equip->card;
    delete_handnode(player->hand, equip);
    free(equip);
    player->num_of_hand--;
}

void multi_lightning(Player *player, Handnode *lightning, FILE *fp) {
    for (int i = 0; i < player->top_fate; ++i) {
        if (player->fate[i]->iname == 10) {//there is a lightning existing
            printf("You already have a \"Lightning\" in your fate zone.\n");
            puts("You can't have two cards of the same name in your fate zone.");
            return;
        }
    }
    push_fatecard(player, lightning->card);
    delete_handnode(player->hand, lightning);
    free(lightning);
    player->num_of_hand--;
    fprintf(fp, "Player%d uses a \"Lightning\" and put it into his own fate zone.\n", player->index);
    printf("You have successfully placed a \"Lightning\" in your fate zone!\n");
}

void multi_some_for_no(Player *player, Deck **pDeck, Deck **discard_deck, FILE *fp, Handnode *some_for_no) {
    discard(player, some_for_no, discard_deck);
    free(some_for_no);
    fprintf(fp, "Player%d uses a \"Something for nothing\" and draws two cards.\n", player->index);
    printf("You have used a \"Something for nothing\" to draw two cards.\n");
    int n = draw(player, pDeck, 2);
    if (n != -1) {
        deck_reshuffle(pDeck, discard_deck);
        draw(player, pDeck, 2 - n);
    }
}

void multi_drown(Player *attack, Player *defense, Handnode *drown, FILE *fp) {
    for (int i = 0; i < defense->top_fate; ++i) {
        if (defense->fate[i]->iname == 11) {//there exists a drown
            printf("Player%d already has a \"Drown in happiness\" in fate zone, so you cannot put it into his fate zone again.\n",
                   defense->index);
            return;
        }
    }
    push_fatecard(defense, drown->card);
//    push_fatecard(defense, drown->card);
    delete_handnode(attack->hand, drown);
    free(drown);
    attack->num_of_hand--;
    fprintf(fp, "Player%d uses a \"Drown in happiness\" and put it into player%d's fate zone.\n", attack->index,
            defense->index);
    printf("You have successfully put a \"Drown in happiness\" into player%d's fate zone.\n", defense->index);
}

void multi_starve(Player *attack, Player *defense, FILE *fp, Handnode *starve) {
    if (distance(attack, defense) > 1) {
        printf("You cannot do that, the distance between you and player%d is %d.\n", defense->index,
               distance(attack, defense));
        puts("\"Starvation\" can only be used to those whose distance with you is 1.\n");
        return;
    }
    for (int i = 0; i < defense->top_fate; ++i) {
        if (defense->fate[i]->iname == 12) {//there exists a starve
            printf("There already exists a \"Starvation\" in player%d's fate zone.\n", defense->index);
            puts("Any player can't have two cards of the same name in their fate zone.");
            return;
        }
    }
    push_fatecard(defense, starve->card);
    delete_handnode(attack->hand, starve);
    free(starve);
    attack->num_of_hand--;
    fprintf(fp, "Player%d uses a \"Starvation\" and put it into player%d's fate zone.\n", attack->index,
            defense->index);
    printf("You successfully use a \"Starvation\" and put it into player%d's fate zone.\n", defense->index);
}

void multi_peach_garden(Player *main, int num_of_player, Handnode *peach_garden, Deck **discard_deck, FILE *fp) {
    fprintf(fp, "Player%d uses a \"Peach garden\".\n", main->index);
    printf("You have successfully used a\"Peach garden\".\n");
    puts("All the players alive will restore one health.");
    discard(main, peach_garden, discard_deck);
    free(peach_garden);
    Player *current = main;
    for (int i = 0; i < num_of_player; ++i, current = current->next) {
        if (current->dead_or_alive == 0)
            continue;
        if (current->blood < current->max_blood) {
            fprintf(fp, "Player%d restores one health.\n", current->index);
            current->blood++;
        }
    }
}

void multi_dismantle(Player *attack, Player *defense, Deck **discard_deck, FILE *fp, Handnode *dismantle) {
    int num_of_equip = 0;
//    int choice;
    int array[3] = {-1, -1, -1};
//    char str[10];
    int index_to_discard;
    for (int i = 0; i < 3; ++i) {
        if (defense->equip[i] != NULL) {
            array[num_of_equip] = i;
            num_of_equip++;
        }
    }
    if (num_of_equip == 0) {
        printf("Player%d doesn't have anything in his equipment zone.\n", defense->index);
        return;
    }
    do {
        printf("Equipment zone of player%d: \n", defense->index);
        for (int i = 0; i < num_of_equip; ++i) {
            printf("\t%d) %s\n", i + 1, defense->equip[array[i]]->name);
        }
        printf("Please input the number before the equipment you want to discard.\n");
        puts("Input 0 to give up discarding");
        printf("Input: ");
        scanf("%d", &index_to_discard);
        if (index_to_discard < 0 || index_to_discard > num_of_equip) {
            puts("Invalid input!");
            printf("Please input a number between 0 and %d.\n", num_of_equip);
        }
    } while (index_to_discard < 0 || index_to_discard > num_of_equip);
    if (index_to_discard == 0)
        return;
    discard(attack, dismantle, discard_deck);
    free(dismantle);
    push_deck(discard_deck, defense->equip[array[index_to_discard - 1]]);
    defense->equip[array[index_to_discard - 1]] = NULL;
    fprintf(fp, "Player%d uses a \"Dismantle\" against player%d and discards his ", attack->index, defense->index);
    printf("You have successfully used a \"Dismantle\" to player%d and discards his ", defense->index);
    switch (array[index_to_discard - 1]) {
        case 0:
            fprintf(fp, "Horse.\n");
            puts("Horse.");
            break;
        case 1:
            fprintf(fp, "Bow.\n");
            puts("Bow.");
            break;
        case 2:
            fprintf(fp, "Binocular.\n");
            puts("Binocular.");
            break;
        default:
            break;
    }
}

void multi_snatch(Player *attack, Player *defense, Deck **discard_deck, FILE *fp, Handnode *snatch) {
    if (distance(attack, defense) > 1) {
        printf("You cannot do that, the distance between you and player%d is %d.\n", defense->index,
               distance(attack, defense));
        puts("\"Snatch\" can only be used to those whose distance with you is 1.\n");
        return;
    }
    int num_of_equip = 0;
    int index_to_discard;
    int array[3] = {-1, -1, -1};
    for (int i = 0; i < 3; ++i) {
        if (defense->equip[i] != NULL) {
            array[num_of_equip] = i;
            num_of_equip++;
        }
    }
    if (num_of_equip == 0) {
        printf("Player%d doesn't have anything in his equipment zone.\n", defense->index);
        return;
    }
    do {
        printf("Equipment zone of player%d: \n", defense->index);
        for (int i = 0; i < num_of_equip; ++i) {
            printf("\t%d) %s\n", i + 1, defense->equip[array[i]]->name);
        }
        printf("Please input the number before the equipment you want to steal.\n");
        puts("Input 0 to give up stealing");
        printf("Input: ");
        scanf("%d", &index_to_discard);
        if (index_to_discard < 0 || index_to_discard > num_of_equip) {
            puts("Invalid input!");
            printf("Please input a number between 0 and %d.\n", num_of_equip);
        }
    } while (index_to_discard < 0 || index_to_discard > num_of_equip);
    if (index_to_discard == 0)
        return;
    discard(attack, snatch, discard_deck);
    free(snatch);
    attack->num_of_hand++;
    append_handnode(attack->hand, defense->equip[array[index_to_discard - 1]]);
    defense->equip[array[index_to_discard - 1]] = NULL;
    fprintf(fp, "Player%d uses a \"Snatch\" against player%d and steals his ", attack->index, defense->index);
    printf("You have successfully used a \"Snatch\" against player%d and stolen his ", defense->index);
    switch (array[index_to_discard - 1]) {
        case 0:
            fprintf(fp, "Horse.\n");
            puts("Horse.");
            break;
        case 1:
            fprintf(fp, "Bow.\n");
            puts("Bow.");
            break;
        case 2:
            fprintf(fp, "Binocular.\n");
            puts("Binocular.");
            break;
        default:
            break;
    }
}


void multi_harvest(Player *main, int num_of_player, int num_of_alive, Deck **draw_deck, Deck **discard_deck, FILE *fp,
                   Handnode *harvest) {
    printf("You have successfully used a \"Bountiful harvest\".\n");
    fprintf(fp, "Player%d uses a \"Bountiful harvest\".\n", main->index);
    discard(main, harvest, discard_deck);
    free(harvest);
    Player *current = main;
    Card *temp[20] = {NULL};
    int choice;
//    int available[20]={0};
    for (int i = 0; i < num_of_alive; ++i) {
        if ((*draw_deck)->num == 0) {
            deck_reshuffle(draw_deck, discard_deck);
        }
        temp[i] = pop_deck(draw_deck);
    }
    for (int j = 1; j <= num_of_player; ++j, current = current->next) {
        if (current->dead_or_alive == 0)
            continue;
        change_player(current);
        printf("Player%d has used a\"Bountiful harvest\".\n", main->index);
        do {
            puts("The current information:\n");
            show_field(current,num_of_player);
            printf("Choose the card you like!\n");
            for (int i = 1; i <= num_of_alive; ++i) {
                if (temp[i - 1] != NULL) {
                    printf("\t%d) %s\n", i, temp[i - 1]->name);
                }
            }
            printf("Choice: ");
            scanf("%d", &choice);
            if (choice <= 0 || temp[choice - 1] == NULL) {
                puts("That's an invalid choice!");
                puts("Choose the available card you want to get.");
            }
        } while (choice <= 1 || temp[choice - 1] == NULL);
        append_handnode(current->hand, temp[choice - 1]);
        temp[choice - 1] = NULL;
    }
}

int multi_AOE(Player **pPlayer, Player *main, Deck **discard_deck, FILE *fp, int num_of_player, int *num_of_alive,
              Handnode *aoe) {
    int card = 0;
    int counter = 0;
//    int survive = 0;
    char AOE[30] = {'\0'};
    char counter_AOE[30] = {'\0'};
    int choice;
    strcpy(AOE, aoe->card->name);
    if (aoe->card->iname == 5)//arrow barrage
    {
        card = 5;
        strcpy(counter_AOE, "Dodge");
    } else {
        card = 6;
        strcpy(counter_AOE, "Strike");
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
    for (int i = 0; i < num_of_player - 1; ++i, pcurrent = pcurrent->next) {
//        survive = 0;
        if (pcurrent->dead_or_alive == 0)
            continue;
        change_player(pcurrent);
        printf("Player%d uses a %s.\n", main->index, AOE);
        printf("You need to play a \"%s\" to prevent one damage.\n", counter_AOE);
        while (1) {
            do {
                disp_hand(pcurrent);
                printf("Please input the number before the card to use it.(Only \"Strike\" is available)\n");
                printf("Input 0 to quit and take one damage.\n");
                printf("Choice: ");
                scanf("%d", &choice);
                if (choice < 0 || choice > pcurrent->num_of_hand) {
                    puts("That's an invalid choice!");
                    printf("Please input a number between 0 and %d", pcurrent->num_of_hand);
                } else {
                    if (confirm() == 0) {
                        continue;
                    } else {
                        break;
                    }
                }

            } while (choice < 0 || choice > pcurrent->num_of_hand);
            if (choice == 0) {
                pcurrent->blood--;
                fprintf(fp, "Player%d doesn't use a \"%s\" and takes one damage.\n", pcurrent->index, counter_AOE);
                if (pcurrent->blood > 0)
                    break;
                else if (multi_dying(pPlayer, main, pcurrent, num_of_alive, num_of_player, fp, discard_deck) ==
                         END_OF_GAME)
                    return END_OF_GAME;
                else
                    break;
            } else {
                handcurr = *pcurrent->hand;
                for (int j = 1; j < choice; ++j, handcurr = handcurr->next);
                if (handcurr->card->iname != counter) {
                    printf("That's not a \"%s\", please make another valid choice!\n", counter_AOE);
                    continue;
                } else {
                    discard(pcurrent, handcurr, discard_deck);
                    free(handcurr);
                    fprintf(fp, "Player%d uses a \"%s\" to prevent damage.\n", pcurrent->index, counter_AOE);
                    printf("You have successfully used a \"%s\" to prevent damage.\n", counter_AOE);
                    break;
                }
            }
        }
    }
    change_player(main);
    printf("You have successfully used a \"%s\"!\n",counter_AOE);
    return 1;
}

int
multi_borrow_sword(Player **pPlayer, Player *user, Deck **discard_deck, FILE *fp, int *num_of_alive, int num_of_player,
                   Handnode *borrow) {
    int choice1, choice2;
    int range = 2;
    int hand_choice1;
    int hand_choice2;
    Player *attack, *defense;
    Handnode *handcurr;
    while (1) {
        do {
            choice1 = -1;
            choice2 = -1;
            printf("Please input the index of the player you want to order to use \"Strike\" and his target's index.\n");
            printf("Input two zeros to quit.\n");
            scanf("%d", &choice1);
            scanf("%d", &choice2);
            if (choice1 >= 0 && choice1 <= num_of_player && choice2 >= 0 && choice2 <= num_of_player)
                break;
            printf("Invalid input!\n");
            puts("Try again!");
        } while (1);
        if (choice1 == 0 && choice2 == 0) {
            return 0;
        } else if (choice1 == 0 || choice2 == 0) {
            printf("Invalid input!\n");
            puts("Try again!");
            continue;
        }
        attack = *pPlayer;
        defense = *pPlayer;
        for (int i = 1; i < choice1; ++i, attack = attack->next);
        for (int j = 1; j < choice2; ++j, defense = defense->next);
        if (attack->dead_or_alive == 0 || defense->dead_or_alive == 0) {
            printf("One of the players you ordered is already dead.\n");
            puts("Try again!");
            continue;
        }
        if (attack == user) {
            printf("You cannot order yourself to use \"Strike\"!\n");
            puts("Try again!");
            continue;
        }
        if (attack->equip[1] == NULL) {
            printf("Player%d doesn't have a weapon in equipment zone.\n", attack->index);
            puts("You cannot do that!");
            continue;
        }
        if (distance(attack, defense) > range) {
            printf("Player%d can't use a \"Strike\" to player%d because their distance is too far!\n", attack->index,
                   defense->index);
            continue;
        }
        discard(user, borrow, discard_deck);
        free(borrow);
        fprintf(fp, "Player%d uses a \"Borrowed sword\", ordering player%d to strike player%d.\n", user->index,
                attack->index,
                defense->index);
        change_player(attack);
        while (1) {
            do {
                puts("");
                printf("Player%d has used a \"Borrowed sword\", ordering you to strike player%d.\n", user->index,
                       defense->index);
                disp_hand(attack);
                puts("Please input the number before the card you want to use(only \"Strike\" is available).");
                printf("Input 0 to give up and give your weapon to player%d.\n", user->index);
                printf("Choice: ");
                scanf("%d", &hand_choice1);
                if (hand_choice1 >= 0 && hand_choice1 <= attack->num_of_hand)
                    break;
                puts("Invalid input!");
                printf("Please input a number between 0 and %d.\n", attack->num_of_hand);
            } while (1);
            if (hand_choice1 == 0) {
                user->num_of_hand++;
                append_handnode(user->hand, attack->equip[1]);
                attack->equip[1] = NULL;
                fprintf(fp, "Player%d doesn't use a \"Strike\" and gives his \"Bow\" to player%d.\n", attack->index,
                        user->index);
                change_player(user);
                printf("You have successfully used a \"Borrowed sword\" and player%d gives you his \"Bow\".\n",
                       attack->index);
                return 1;
            } else {
                handcurr = *attack->hand;
                for (int i = 1; i < hand_choice1; ++i, handcurr = handcurr->next);
                if (handcurr->card->iname != 16) {
                    printf("That's not a \"Strike\".\n");
                    puts("Try again!");
                } else {
                    if (confirm() != 0) {
                        discard(attack, handcurr, discard_deck);
                        free(handcurr);
                        fprintf(fp, "Player%d uses a \"Strike\" to player%d.\n", attack->index, defense->index);
                        change_player(defense);
                        do {
                            printf("Player%d uses a \"Strike\" to you!\n", attack->index);
                            printf("Your current health level is %d.\n", defense->blood);
                            disp_hand(defense);
                            puts("Please input the number before the card you want to use(only \"Dodge\" is available).");
                            printf("Input 0 to give up and take %d damage.\n", attack->use_of_wine + 1);
                            scanf("%d", &hand_choice2);
                            if (hand_choice2 < 0 || hand_choice2 > defense->num_of_hand) {
                                puts("Invalid input!");
                                printf("Please input a number between 0 and %d.\n", defense->num_of_hand);
                            }
                        } while (hand_choice2 < 0 || hand_choice2 > defense->num_of_hand);
                        if (hand_choice2 == 0) {//don't use a dodge
                            if (confirm() == 0)
                                continue;
                            fprintf(fp, "Player%d doesn't use a \"Dodge\" and takes %d damage.\n", defense->index,
                                    attack->use_of_wine + 1);
                            defense->blood--;
                            printf("You take %d damages.\n", attack->use_of_wine + 1);
                            if (defense->blood <= 0) {
                                switch (multi_dying(pPlayer, user, defense, num_of_alive, num_of_player, fp,
                                                    discard_deck)) {
                                    case END_OF_GAME:
                                        return END_OF_GAME;
                                    case 1://pcurrent is dead!
                                        if (defense == user)
                                            return 2;
                                        else {
                                            change_player(user);
                                            printf("You have successfully used a \"Borrowed sword\" and player%d is dead!\n",
                                                   defense->index);
                                            return 1;
                                        }
                                    case 0://pcurrent is saved
                                        change_player(user);
                                        printf("You have successfully used a \"Borrowed sword\".\n");
                                        return 1;
                                    default:
                                        break;
                                }
                            } else {//pcurrent doesn't enter dying phase
                                change_player(user);
                                printf("You have successfully used a  \"Borrowed sword\".\n");
                                return 1;
                            }
                        } else {//try to use a dodge
                            handcurr = *defense->hand;
                            for (int i = 1; i < hand_choice2; ++i, handcurr = handcurr->next);
                            if (handcurr->card->iname != 18) {
                                printf("That's not a \"Dodge\"!\n");
                                puts("Make another valid choice!");
                                continue;
                            } else {
                                fprintf(fp, "Player%d uses a \"Dodge\" to avoid damage.\n", defense->index);
                                discard(defense, handcurr, discard_deck);
                                free(handcurr);
                                printf("You have successfully used a \"Dodge\" to prevent damage.\n");
                                change_player(user);
                                printf("You have successfully used a  \"Borrowed sword\".\n");
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void show_max_blood(Player *player) {
    printf("\tMax health level: %d.\n", player->max_blood);
}