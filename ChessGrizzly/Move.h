#pragma once
#include "BBmacros.h"
#include "tables.h"

// The structure of a move is as follows:
// From: The square the piece is moving from
// To: The square the piece is moving to
// Piece: The piece that is moving
// Captured: The piece that is captured
// Promotion: The piece that is promoted to
// EnPassant: Whether or not the move is an en passant
// Castling: Whether or not the move is a castling move
// DoublePawnPush: Whether or not the move is a double pawn push
// Check: Whether or not the move is a check

/*
    0000 0000 0000 0000 0000 0000 00xx xxxx - From
    0000 0000 0000 0000 0000 xxxx xx00 0000 - To
    0000 0000 0000 0000 xxxx 0000 0000 0000 - Piece
    0000 0000 0000 xxxx 0000 0000 0000 0000 - Captured
    0000 0000 xxxx 0000 0000 0000 0000 0000 - Promotion
    0000 000x 0000 0000 0000 0000 0000 0000 - EnPassant
    0000 00x0 0000 0000 0000 0000 0000 0000 - Castling
    0000 0x00 0000 0000 0000 0000 0000 0000 - DoublePawnPush
    0000 x000 0000 0000 0000 0000 0000 0000 - Check
*/

// The move is therefore stored as a 32 bit integer
typedef U32 Move;

#define sourceSquare(move) ((move) & 0x3F)
#define targetSquare(move) (((move) >> 6) & 0x3F)
#define pieceMoved(move) (((move) >> 12) & 0xF)
#define pieceCaptured(move) (((move) >> 16) & 0xF)
#define promotionOf(move) (((move) >> 20) & 0xF)
#define isEnPassantMove(move) (((move) >> 24) & 0x1)
#define isCastlingMove(move) (((move) >> 25) & 0x1)
#define isDoublePawnPush(move) (((move) >> 26) & 0x1)
#define isCheckMove(move) (((move) >> 27) & 0x1)

#define encodeMove(from, to, piece, captured, promotion, enPassant, castling, doublePawnPush, check) \
    (((from) & 0x3F) | \
    (((to) & 0x3F) << 6) | \
    (((piece) & 0xF) << 12) | \
    (((captured) & 0xF) << 16) | \
    (((promotion) & 0xF) << 20) | \
    (((enPassant) & 0x1) << 24) | \
    (((castling) & 0x1) << 25) | \
    (((doublePawnPush) & 0x1) << 26) | \
    (((check) & 0x1) << 27))

#define okToReduce(move) (pieceCaptured(move) == EMPTY && promotionOf(move) == EMPTY && isCheckMove(move) == 0)

/**
 * @brief The printMove function prints a move in a human readable format (UCI)
 * @param move The move to print
 */
void printMove(Move move);

// The move will be scored in order to be ordered for the A/B search.
// We will store the score in an unsigned int. So the ScoredMove can be represented by a 64 bit unsigned int.
// The score is stored in the upper 32 bits and the move in the lower 32 bits.
typedef U64 ScoredMove;

#define scoreOfMove(move) ((move) >> 32)

#define onlyMove(move) ((move) & ((1 << 27) - 1))

// All the moves are stored in the MoveList structure.
struct MoveList {
    // The number of moves in the list
    U8 count = 1; // The count starts from 1 so that we reserve the first element for the ttMove
    // It has been found that the maximum number of moves is 109, so 128 is a good size (since there may be illegal moves)
    ScoredMove moves[128] = { 0ULL };
};

// If needed, #include "Position.h"

/**
 * @brief The printMoveList function prints a move list in a human readable format (UCI)
 * @param moveList The move list to print
 * @param printScore Whether or not to print the score of the move 
 */
void printMoveList(MoveList *moveList, bool printScore = false);

const U8 castlingRightsMask[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

/**
 * @brief The lmrReduction function returns the reduction for a move based on the depth and the move number
 * @param depth The depth of the search
 * @param moveNumber The move number
 * @return The reduction
 */
extern inline U8 lmrReduction(U8 depth, U8 moveNumber);