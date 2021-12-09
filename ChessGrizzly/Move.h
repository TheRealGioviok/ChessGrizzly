#pragma once

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
typedef unsigned int Move;

#define sourceSquare(move) ((move) & 0x3F)
#define targetSquare(move) (((move) >> 6) & 0x3F)
#define pieceMoved(move) (((move) >> 12) & 0xF)
#define pieceCaptured(move) (((move) >> 16) & 0xF)
#define promotion(move) (((move) >> 20) & 0xF)
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

/**
 * @brief The printMove function prints a move in a human readable format (UCI)
 * @param move The move to print
 */
void printMove(Move move);