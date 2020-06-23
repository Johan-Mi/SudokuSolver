#!/usr/bin/env python3
"""This module solves sudokus."""

import sys
from pathlib import Path
from copy import deepcopy


def print_board(board):
    """Prints a 9x9 list of numbers as a sudoku board."""
    for i in range(9):
        for j in range(9):
            print(board[i][j] if board[i][j] else ' ', end='')
            if j in (2, 5):
                print('│', end='')
        print('')
        if i in (2, 5):
            print('───┼───┼───')
    print('')


def solve(board, x_pos, y_pos):
    """Solves a sudoku board, given the x and y coordinates to start on."""
    while board[y_pos][x_pos]:
        if x_pos == 8 and y_pos == 8:
            print_board(board)
            return True
        x_pos += 1
        if x_pos == 9:
            y_pos += 1
            x_pos = 0

    possible = set(range(1, 10))

    for i in range(9):
        possible.discard(board[y_pos][i])
        possible.discard(board[i][x_pos])

    for i in range(x_pos - x_pos % 3, x_pos - x_pos % 3 + 3):
        for j in range(y_pos - y_pos % 3, y_pos - y_pos % 3 + 3):
            possible.discard(board[j][i])

    next_x = (x_pos + 1) % 9
    next_y = y_pos + (x_pos == 8)
    new_board = deepcopy(board)

    for num in possible:
        new_board[y_pos][x_pos] = num
        if x_pos == 8 and y_pos == 8:
            print_board(new_board)
            return True
        if solve(new_board, next_x, next_y):
            return True

    return False


def main():
    """Reads a sudoku board from a specified file and solves it."""
    if len(sys.argv) != 2:
        print('Usage: %s (name of sudoku file)' % sys.argv[0])
        sys.exit()

    in_file = Path(sys.argv[1]).read_text()

    board = [[0 for i in range(9)] for j in range(9)]

    for i in range(9):
        for j in range(9):
            curr_char = in_file[i * 10 + j]
            board[i][j] = 0 if curr_char in ' .-_' else int(curr_char)

    print_board(board)

    if not solve(board, 0, 0):
        print('No solution found')

if __name__ == "__main__":
    main()
