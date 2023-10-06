#include <stdio.h>
#include <assert.h>

#include "test.h"

/*
 * The status of a line.
 */
enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * board[14] = color of 56
 */
typedef char board_t[15];
typedef char player_t; /* A player should be RED or BLUE. */
void init_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        board[i] = NO;
    }
}
player_t other_player(player_t player)
{
    switch (player) {
    case RED : return BLUE;
    case BLUE : return RED;
    }
}

int has_won(board_t board, player_t player)
{   
    player_t otherplayer=other_player(player);
    int triplets[20][3] = {
        {0, 1, 5}, {0, 2, 6}, {0, 3, 7}, {0, 4, 8}, {1, 2, 9},
        {1, 3, 10}, {1, 4, 11}, {2, 3, 12}, {2, 4, 13}, {3, 4, 14},
        {5, 6, 9}, {5, 7, 10}, {5, 8, 11}, {6, 7, 12},
        {6, 8, 13}, {7, 8, 14}, {9, 10, 12}, {9, 11, 13}, {10, 11, 14}, {12, 13, 14}
    };

    for (int i = 0; i < 20; i++) {
        int a = triplets[i][0];
        int b = triplets[i][1];
        int c = triplets[i][2];
        if (board[a] == otherplayer && board[b] == otherplayer && board[c] == otherplayer) {
            return 1;
        }
    }

    return 0;
}

int is_full(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            return 0;
        }
    }
    return 1;
}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

move_t best_move(board_t board, player_t player)
{
    move_t response;
    move_t candidate;
    int no_candidate = 1;

    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
            if (has_won(board, player)) {
                board[i] = NO;
                return (move_t) {
                    .line = i,
                    .score = 1
                };
            }
            board[i] = NO;
        }
    }

    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
            response = best_move(board, other_player(player));
            board[i] = NO;
            if (response.score == -1) {
                return (move_t) {
                    .line = i,
                    .score = 1
                };
            } else if (response.score == 0) {
                candidate = (move_t) {
                    .line = i,
                    .score = 0
                };
                no_candidate = 0;
            } else { 
                if (no_candidate) {
                    candidate = (move_t) {
                        .line = i,
                        .score = -1
                    };
                    no_candidate = 0;
                }
            }
        }
    }
    return candidate;
}




void print_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
        case RED: printf("R  "); break;
        case BLUE: printf("B  "); break;
        case NO: printf(".  "); break;
        }
    }
    printf("\n");
}
void print_board_normal(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        printf("%d  ", board[i]);
    }
    printf("\n");
}

int main()
{
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    /*assuming red is the first player and blue is the second player*/
    board_t board;
    player_t current_player;
    player_t computer_player;
    init_board(board);
    move_t response;
    printf("Enter your choice of color (R for RED(1st player), B for BLUE(2nd player)): ");
    char color;
    scanf(" %c", &color); // Add a space before %c to consume any leading whitespace.

    if (color == 'R' || color == 'r') {
        current_player = RED;
        computer_player = BLUE;
    } else if (color == 'B' || color == 'b') {
        current_player = BLUE;
        computer_player = RED;
    } else {
        printf("Invalid choice of color");
        return 0;
    }

    while (1) {
        if (current_player == RED) {
            print_board(board);
            printf("Enter your move: ");
            int move;
            scanf("%d", &move);
            if (move < 0 || move > 14) {
                printf("Invalid move try again");
                continue;
            }
            if (board[move] != NO) {
                printf("Invalid move try again");
                continue;
            }
            
            board[move] = other_player(computer_player);
        } else {
            response = best_move(board, computer_player);
            board[response.line] = computer_player;
        }

        if (has_won(board, current_player)) {
            print_board(board);
            printf("\nPlayer %s has won!\n", (current_player==RED) ? "red" : "blue");
            break;
        } 
        else if (has_won(board,computer_player)){
            print_board(board);
            printf("\nPlayer %s has won!\n", (computer_player==RED) ? "red" : "blue");
            break;
        }
        else if (is_full(board)) {
            print_board(board);
            printf("\nDraw.\n");
            break;
        }
        current_player = other_player(current_player);
    }

    return 0;
}
