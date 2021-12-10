// The BBmacros.h file is a collection of macros that are used in the engine.
// The macros are used to make the code more readable and easier to maintain.
// The macros are also used to make the code more efficient.

#pragma once
#include <stdio.h>
#include <iostream>
#include <intrin.h>
#include <windows.h>


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
typedef U64 HashKey;
typedef U8 Side;
typedef U8 Depth;
typedef U8 Ply;
typedef U8 CastlingRights;
typedef wchar_t pieceChar;

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
 * @param board The bitboard to print.
 */
void printBitBoard(BitBoard board);

/**
 * @brief The pieceFromChar function returns the piece corresponding to a character.
 * @param pieceChar The character to convert.
 * @return The piece corresponding to the character (EMPTY if the character is not a piece).
 */
Piece pieceFromChar(char pieceChar);

/**
 * @brief The squareFromName function returns the square corresponding to a name.
 * @param squareName The name of the square.
 * @return The square corresponding to the name (noSquare if the name is not a square).
 */
Square squareFromName(const char* squareName);

/**
 * @brief The getTime64 function returns the current time in milliseconds.
 * @return The current time in milliseconds.
 */
inline U64 getTime64(){
    return GetTickCount64();
}

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

const char charPieces[13] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', '.'};
const char promotionPieces[13] = {'p', 'n', 'b', 'r', 'q', 'k', 'p', 'n', 'b', 'r', 'q', 'k', ' '};
const pieceChar pieceChars[13] = {'♙', '♘', '♗', '♖', '♕', '♔', '♟', '♞', '♝', '♜', '♛', '♚', '.'};

enum cstlRights{
    WKCastle = 1,
    WQCastle = 2,
    BKCastle = 4,
    BQCastle = 8,
    NoCastle = 0
};

const std::string squareNames[65] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "noSquare"
};

// Debug FENs
#define emptyBoard "8/8/8/8/8/8/8/8 w - - "
#define startpos "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define trickyPosition "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killerPosition "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmkPosition "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 " // Courtesy of Code Monkey King