#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void printBoard(char const board[9][9]) {
	for(unsigned char i = 0; i < 9; i++) {
		for(unsigned char j = 0; j < 9; j++) {
			putchar(board[i][j] ? board[i][j] + '0' : ' ');
			if(j == 2 || j == 5) {
				printf("│");
			}
		}
		putchar('\n');
		if(i == 2 || i == 5) {
			puts("───┼───┼───");
		}
	}
	putchar('\n');
}

bool solve(char const board[9][9], unsigned char x, unsigned char y) {
	while(board[y][x]) {
		if(x == 8 && y == 8) {
			printBoard(board);
			return true;
		}
		if(++x == 9) {
			y++;
			x = 0;
		}
	}

	char newBoard[9][9];
	memcpy(newBoard, board, 81);

	unsigned char const nextX = (x + 1) % 9;
	unsigned char const nextY = y + (x == 8);

	bool possible[9];
	memset(possible, true, sizeof(possible));

	for(unsigned char i = 0; i < 9; i++) {
		possible[newBoard[y][i] - 1] &= !newBoard[y][i];
		possible[newBoard[i][x] - 1] &= !newBoard[i][x];
	}

	for(unsigned char i = x - x % 3; i < x - x % 3 + 3; i++) {
		for(unsigned char j = y - y % 3; j < y - y % 3 + 3; j++) {
			possible[newBoard[j][i] - 1] &= !newBoard[j][i];
		}
	}

	for(unsigned char i = 0; i < 9; i++) {
		if(possible[i]) {
			newBoard[y][x] = i + 1;
			if(x == 8 && y == 8) {
				printBoard(newBoard);
				return true;
			}
			if(solve(newBoard, nextX, nextY)) {
				return true;
			}
		}
	}

	return false;
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage: %s (name of sudoku file)\n", argv[0]);
		return 1;
	}

	FILE *fp = fopen(argv[1], "rb");
	if(!fp) {
		perror(argv[1]);
		return 1;
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	char *buffer = calloc(1, size + 1);
	if(!buffer) {
		fclose(fp);
		fputs("Memory allocation failed\n", stderr);
		return 1;
	}

	if(fread(buffer, size, 1, fp) != 1) {
		fclose(fp);
		free(buffer);
		fputs("Read failed\n", stderr);
		return 1;
	}

	char board[9][9];

	for(unsigned char i = 0; i < 9; i++) {
		for(unsigned char j = 0; j < 9; j++) {
			char const currChar = buffer[i * 10 + j];
			switch(currChar) {
				case ' ':
				case '.':
				case '-':
				case '_':
					board[i][j] = 0;
					break;
				case '0' ... '9':
					board[i][j] = currChar - '0';
					break;
				default:
					fputs("Invalid character in input file\n", stderr);
					return 1;
			}
		}
	}

	fclose(fp);
	free(buffer);

	printBoard(board);

	if(!solve(board, 0, 0)) {
		puts("No solution found");
	}
}
