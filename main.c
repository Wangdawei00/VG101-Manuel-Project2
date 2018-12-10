#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "deck.h"
#include "player.h"
#include "demo_game.h"
#include "multi_game.h"

int main(int argc, char **argv) {
//    printf("Hello, World!\n");
    srand((unsigned int) time(NULL));
    int c;
    Deck *draw_deck;
    if (argc > 1)
        draw_deck = deck_initialize_file(argv[2]);
    else
        draw_deck = deck_initialize_stdin();
    puts("Please select the mode of the game. ");
    puts("1---Demo mode, where a whole game is played, all the decision are randomly taken, and a file named \"demo file\" displays "
         "all the details of the game. ");
    puts("2---Multi-player mode, where human players make all the decisions manually.");
    int mode;
    int success = 0;
    while (success != 1) {
        scanf("%d", &mode);
        switch (mode) {
            case 1:
                demo_Game(draw_deck);
//                printf("Press Enter to continue.\n");
//                while ((c = getchar()) != '\n' && c != EOF);
//                getchar();
//                printf("%d", draw_deck->cards[50]->inum);
                success = 1;
                puts("The game has been successfully run, please check all the information in the file \"demo file.txt\".");
                break;
            case 2:
                multi_Game(draw_deck);
                success = 1;
//                puts("Game over!");
                break;
            default:
                puts("INVALID MODE!");
                puts("Please input 1 or 2.");
        }
    }
    return 0;
}