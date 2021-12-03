// The BBmacros.h file is a collection of macros that are used in the engine.
// The macros are used to make the code more readable and easier to maintain.
// The macros are also used to make the code more efficient.

#pragma once
#include <stdio.h>
#include <iostream>
#include <intrin.h>

// DATA TYPES
typedef unsigned long long U64;
typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;

typedef long long S64;
typedef int S32;
typedef short S16;
typedef char S8;

typedef U64 BitBoard;
typedef U64 BB;
typedef U64 BitMask;
typedef U8 Square;
typedef U8 Piece;
typedef S16 Score;
typedef U32 Move;
typedef U64 ScoredMove;
typedef U64 HashKey;


// BIT MANIPULATION
#define testBit(board, bit) (((board) >> (bit)) & 1)
#define setBit(board, bit) ((board) |= (1ULL << (bit)))
#define clearBit(board, bit) ((board) &= ~(1ULL << (bit)))
#define toggleBit(board, bit) ((board) ^= (1ULL << (bit)))
#define squareBitBoard(square) (1ULL << (square))
#define popCount(board) __popcnt64(board)
#define bitScanForward _BitScanForward64 //LS1B
#define bitScanReverse _BitScanReverse64 //MS1B
#define rankOf(square) ((square) >> 3)
#define fileOf(square) ((square) & 7)
#define makeSquare(rank, file) ((rank) * 8 + (file))
#define forEachSquare(square) for (Square square = a8; square <= h1; square++)
#define forEachRank(rank) for (int rank = 0; rank <= 8; rank++)
#define forEachFile(file) for (int file = 0; file <= 8; file++)
#define forEachPiece(piece) for (Piece piece = P; piece <= k; piece++)
#define forEachColor(color) for (Color color = WHITE; color <= BLACK; color++)
// UTILITY FUNCTIONS
// The assign macro assigns a value to two variables at once
#define assign(value, var1, var2) \
    (var1) = (value);             \
    (var2) = (value);

// The color macro returns the color of a piece. It assumes that the piece is not empty.
#define color(piece) ((piece) < 6)

// The pieceType macro returns the type of a piece. It assumes that the piece is not empty.
#define pieceType(piece) ((piece) % 6)

/**
 * @brief The printBitBoard function prints a bitboard in a readable format.
 * 
 * @param board The bitboard to print.
 */
void printBitBoard(BitBoard board);


// Enums
enum Color {
    WHITE,
    BLACK,
    BOTH
};

enum PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum Pieces{
    P,
    N,
    B,
    R,
    Q,
    K,
    p,
    n,
    b,
    r,
    q,
    k,
    EMPTY
};

enum Squares{
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, noSquare
};

const std::string squareNames[64] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

