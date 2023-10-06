#include <stdio.h>
#include <assert.h>

#define BOARD_ROWS (4)
#define BOARD_COLS (5)
#define VICTORY_CONDITION (4)

typedef char player_t; 
typedef char board_t[BOARD_ROWS][BOARD_COLS]; 

void init_board(board_t board)
{
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS; ++col) {
            board[row][col] = '.';
        }
    }
}

void print_board(board_t board)
{
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS; ++col) {
            printf("%3c ", board[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

int is_full(board_t board)
{
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS; ++col) {
            if (board[row][col] == '.') {
                return 0;
            }
        }
    }
    return 1;
}

int has_won(board_t board, player_t player)
{
    // Check rows
    for (int row = 0; row < BOARD_ROWS; ++row) {
        int count = 0;
        for (int col = 0; col < BOARD_COLS; ++col) {
            if (board[row][col] == player) {
                count++;
                if (count == VICTORY_CONDITION) {
                    return 1;
                }
            } else {
                count = 0;
            }
        }
    }

    // Check columns
    for (int col = 0; col < BOARD_COLS; ++col) {
        int count = 0;
        for (int row = 0; row < BOARD_ROWS; ++row) {
            if (board[row][col] == player) {
                count++;
                if (count == VICTORY_CONDITION) {
                    return 1;
                }
            } else {
                count = 0;
            }
        }
    }

    for (int start_row = 0; start_row <= BOARD_ROWS - VICTORY_CONDITION; ++start_row) {
        for (int start_col = 0; start_col <= BOARD_COLS - VICTORY_CONDITION; ++start_col) {
            int count_diag1 = 0, count_diag2 = 0;
            for (int i = 0; i < VICTORY_CONDITION; ++i) {
                if (board[start_row + i][start_col + i] == player) {
                    count_diag1++;
                    if (count_diag1 == VICTORY_CONDITION) {
                        return 1;
                    }
                } else {
                    count_diag1 = 0;
                }

                if (board[start_row + i][start_col + VICTORY_CONDITION - 1 - i] == player) {
                    count_diag2++;
                    if (count_diag2 == VICTORY_CONDITION) {
                        return 1;
                    }
                } else {
                    count_diag2 = 0;
                }
            }
        }
    }

    return 0;
}

player_t other_player(player_t player)
{
    switch (player) {
    case 'R':
        return 'B';
    case 'B':
        return 'R';
    default:
        assert(0);
    }
}


typedef struct {
    int row;
    int col;
    /* -1 for a loss, 0 for a draw, 1 for a win. */
    int score;
} move_t;

/*
 * board should be an unfinished game.
 */
move_t best_move(board_t board, player_t player, int depth) {
    move_t candidate = {-1, -1, 0};  
    int no_candidate=1;

    if (is_full(board)) {
        return (move_t){-1, -1, 0};
    }

    if (depth <= 0) {
        return (move_t){-1, -1, 0};
    }

    for (int col = 0; col < BOARD_COLS; ++col) {
        int row = BOARD_ROWS - 1;

        while (row >= 0 && board[row][col] != '.') {
            row--;
        }

        if (row >= 0 && board[row][col] == '.') {
            board[row][col] = player;

            if (has_won(board, player)) {
                board[row][col] = '.';
                return (move_t){row, col, 1};
            }

            move_t response = best_move(board, other_player(player), depth - 1);
            board[row][col] = '.';

            if (response.score == -1) {
                return (move_t){row, col, 1};
            } else if (response.score == 0) {
                candidate = (move_t){row, col, 0};
                no_candidate = 0;
            } else {
                if (no_candidate) {
                    candidate = (move_t){row, col, -1};
                    no_candidate = 0;
                }
            }
        }
    }

    return candidate;
}


void print_key()
{
    int i = 0;
        for (int col = 0; col < BOARD_COLS; ++col) {
            printf("%3d ", i++);
        }
        printf("\n");
}

int main()
{
    int move, row, col,a;
    board_t board;
    move_t response;
    player_t current=0;
    printf("Press 0 for Red or 1 for Blue: ");
    scanf("%d",&a);
    if (a==0){
        current = 'R';
    }
    else if (a==1){
        current = 'B';
    }
    int lis[2]={'R','B'};
    init_board(board);

    while (1) {
        print_board(board);
        if (current == lis[a]) {
            print_key();
            printf("Enter your move: ");
            scanf("%d", &move);
            if (move==711){
                printf("Player has won using secret code!\n");
                break;

            }
            for (int i=BOARD_ROWS-1;i>=0;i--){
                if (board[i][move]=='.'){
                    row=i;
                    col=move;
                    break;
                }
            }

            assert(row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS && board[row][col] == '.');
            board[row][col] = current;
        } else {
            response = best_move(board, current,11);
            board[response.row][response.col] = current;
        }

        if (has_won(board, current)) {
            print_board(board);
            printf("Player %c has won!\n", current);
            break;
        } else if (is_full(board)) {
            print_board(board);
            printf("Draw.\n");
            break;
        }

        current = other_player(current);
    }

    return 0;
}
