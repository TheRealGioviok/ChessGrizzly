# The program attempts to solve the Brunix-Square puzzle, with color number = 6 and size = 4x4.
# The Brunix-Square puzzle is a square puzzle with a square of colors numbered 1 to 6.
# The color sequence is the same as the number sequence. When the color goes up to 6, it goes back to 1.
# The player can choose a square to cycle its color, as well as the square immediately to its corners.

# Being there 16 squares, there are 16 possible moves. Each move adds 1 to a cross of squares as described above.
# import numpy as np
import numpy as np
import random
import time
from scipy.linalg import lu
import scipy
import pprint
# The move class contains the move number and the squares involved in the move.
class Move:
    def __init__(self, moveNumber, squares):
        self.move_number = moveNumber
        self.squares = squares
    
    def parse(self):
        arr = []
        for i in range(16):
            if i in self.squares:
                arr.append(1)
            else:
                arr.append(0)
        self.squares = arr

# The board is a list of squares. Each square can be a color from 1 to 6.
class Board:
    def __init__(self, startColor = 1):
        self.board = []
        for i in range(16):
            self.board.append(startColor)

# We would like to find the sequence of moves that can change the color of one single square, if it exists. We can compute 
# the solution to a system of 16 linear equations (the squares) in 16 unknowns(the moves).

moveList = [
    Move(1, [0, 5]),
    Move(2, [1, 4, 6]),
    Move(3, [1, 4, 9]),
    Move(4, [0, 2, 5, 8, 10]),
    
    Move(5, [2, 5, 7]),
    Move(6, [3, 6]),
    Move(7, [1, 3, 6, 9, 11]),
    Move(8, [2, 7, 10]),

    Move(9, [5, 8, 13]),
    Move(10, [4, 6, 9, 12, 14]),
    Move(11, [9, 12]),
    Move(12, [8, 10, 13]),

    Move(13, [5, 7, 10, 13, 15]),
    Move(14, [6, 11, 14]),
    Move(15, [9, 11, 14]),
    Move(16, [10, 15])
]

# We can solve the system of linear equations by using the Gauss-Jordan method.
# The Gauss-Jordan method is a method for solving systems of linear equations.
# The method is based on the elimination of the first row of the matrix.
# The elimination of the first row of the matrix is done by dividing the first row by the first element of the first row.
# Then, the first row is eliminated from the rest of the matrix.
# The elimination of the first row of the matrix is repeated until the matrix is reduced to a triangular matrix.
# The triangular matrix is then used to find the solution of the system of linear equations.

for move in moveList:
    move.parse()


A = np.array( [moveList[i].squares for i in range(16)] )
B = np.array([1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1])

print(A)

print(X)
