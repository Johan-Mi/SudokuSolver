#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_board(char const board[9][9]) {
    for (unsigned char i = 0; i < 9; i++) {
        for (unsigned char j = 0; j < 9; j++) {
            putchar(board[i][j] ? board[i][j] + '0' : ' ');
            if (j == 2 || j == 5) {
                printf("│");
            }
        }
        putchar('\n');
        if (i == 2 || i == 5) {
            puts("───┼───┼───");
        }
    }
    putchar('\n');
}

bool solve(char board[9][9], unsigned char x, unsigned char y) {
    while (board[y][x]) {
        if (x == 8 && y == 8) {
            print_board(board);
            return true;
        }
        if (++x == 9) {
            y++;
            x = 0;
        }
    }

    bool possible[9];
    memset(possible, true, sizeof(possible));

    for (unsigned char i = 0; i < 9; i++) {
        possible[board[y][i] - 1] &= !board[y][i];
        possible[board[i][x] - 1] &= !board[i][x];
    }

    for (unsigned char i = x - x % 3; i < x - x % 3 + 3; i++) {
        for (unsigned char j = y - y % 3; j < y - y % 3 + 3; j++) {
            possible[board[j][i] - 1] &= !board[j][i];
        }
    }

    unsigned char const next_x = (x + 1) % 9;
    unsigned char const next_y = y + (x == 8);

    for (unsigned char i = 0; i < 9; i++) {
        if (possible[i]) {
            board[y][x] = i + 1;
            if (x == 8 && y == 8) {
                print_board(board);
                return true;
            }
            if (solve(board, next_x, next_y)) {
                return true;
            }
        }
    }

    board[y][x] = 0;

    return false;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s (name of sudoku file)\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        perror(argv[1]);
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = calloc(1, size + 1);
    if (!buffer) {
        fclose(fp);
        fputs("Memory allocation failed\n", stderr);
        return 1;
    }

    if (fread(buffer, size, 1, fp) != 1) {
        fclose(fp);
        free(buffer);
        fputs("Read failed\n", stderr);
        return 1;
    }

    char board[9][9];

    for (unsigned char i = 0; i < 9; i++) {
        for (unsigned char j = 0; j < 9; j++) {
            char const curr_char = buffer[i * 10 + j];
            switch (curr_char) {
            case ' ':
            case '.':
            case '-':
            case '_':
                board[i][j] = 0;
                break;
            case '0' ... '9':
                board[i][j] = curr_char - '0';
                break;
            default:
                fputs("Invalid character in input file\n", stderr);
                return 1;
            }
        }
    }

    fclose(fp);
    free(buffer);

    print_board(board);

    if (!solve(board, 0, 0)) {
        puts("No solution found");
    }
}
