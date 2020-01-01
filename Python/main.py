#!/usr/bin/env python3

import sys
from pathlib import Path
from copy import deepcopy

def printBoard(board):
	for i in range(9):
		for j in range(9):
			print(board[i][j] if board[i][j] else ' ', end='')
			if j == 2 or j == 5:
				print('│', end='')
		print('')
		if i == 2 or i == 5:
			print('───┼───┼───')
	print('')

def solve(board, x, y):
	while board[y][x]:
		if x == 8 and y == 8:
			printBoard(board)
			return True
		else:
			x += 1
			if x == 9:
				y += 1
				x = 0

	possible = set(range(1, 10))

	for i in range(9):
		possible.discard(board[y][i])
		possible.discard(board[i][x])

	for i in range(x - x % 3, x - x % 3 + 3):
		for j in range(y - y % 3, y - y % 3 + 3):
			possible.discard(board[j][i])

	nextX = (x + 1) % 9
	nextY = y + (x == 8)
	newBoard = deepcopy(board)

	for num in possible:
		newBoard[y][x] = num
		if x == 8 and y == 8:
			printBoard(newBoard)
			return True
		elif solve(newBoard, nextX, nextY):
			return True

	return False

if len(sys.argv) != 2:
	print('Usage: %s (name of sudoku file)' % sys.argv[0])
	sys.exit()

inFile = Path(sys.argv[1]).read_text()

board = [[0 for i in range(9)] for j in range(9)]

for i in range(9):
	for j in range(9):
		currChar = inFile[i * 10 + j]
		board[i][j] = 0 if currChar in ' .-_' else int(currChar)

printBoard(board)

if not solve(board, 0, 0):
	print('No solution found')
