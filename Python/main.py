#!/usr/bin/env python3
"""This module solves sudokus."""

import sys


def print_board(board):
    """Prints a 9x9 list of numbers as a sudoku board."""
    for i in range(9):
        for j in range(9):
            print(board[i][j] or " ", end="")
            if j in (2, 5):
                print("│", end="")
        print()
        if i in (2, 5):
            print("───┼───┼───")
    print()


def solve(board, x_pos, y_pos):
    """Solves a sudoku board, given the x and y coordinates to start on."""
    while board[y_pos][x_pos]:
        if x_pos == 8 and y_pos == 8:
            return True
        x_pos += 1
        if x_pos == 9:
            y_pos += 1
            x_pos = 0

    possible = set(range(1, 10))

    for i in range(9):
        possible.discard(board[y_pos][i])
        possible.discard(board[i][x_pos])
        possible.discard(
            board[y_pos - y_pos % 3 + i % 3][x_pos - x_pos % 3 + i // 3])

    next_x = (x_pos + 1) % 9
    next_y = y_pos + (x_pos == 8)

    for num in possible:
        board[y_pos][x_pos] = num
        if (x_pos == 8 and y_pos == 8) or solve(board, next_x, next_y):
            return True

    board[y_pos][x_pos] = 0

    return False


def main():
    """Reads a sudoku board from a specified file and solves it."""
    if len(sys.argv) != 2:
        sys.exit(f"Usage: {sys.argv[0]} (name of sudoku file)")

    with open(sys.argv[1]) as sudoku_file:
        file_content = sudoku_file.read()

    board = [[None for i in range(9)] for j in range(9)]

    for i in range(9):
        for j in range(9):
            curr_char = file_content[i * 10 + j]
            board[i][j] = 0 if curr_char in " .-_" else int(curr_char)

    print_board(board)

    if solve(board, 0, 0):
        print_board(board)
    else:
        sys.exit("No solution found")

if __name__ == "__main__":
    main()
