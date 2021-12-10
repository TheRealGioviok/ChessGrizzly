#pragma once

#include "BBmacros.h"
#include "tables.h"
#include "Move.h"
// The Position.h file contains the Position class, which stores information about the position of the chessboard.
// It can also generate moves and check if a move is legal.


class Position {
public:

    // The bitboards array contains a bitboard for each piece type.
    BitBoard bitboards[12];

    // The turn variable stores which side is to move.
    Side turn;

    // The enPassantSquare variable stores the square where a pawn can be captured en passant.
    Square enPassantSquare;

    // The castlingRights variable stores which castling rights are available.
    CastlingRights castlingRights;

    // The halfMoveClock variable stores the number of half moves since the last pawn move or capture. We can use a U8 to store this, since the maximum number of half moves is 100 (draw by 50 move rule).
    U8 halfMoveClock;

    // The fullMoveNumber variable stores the number of the full move.
    U8 fullMoveNumber;

    /**
     * @brief The printBoard function prints the board to the console.
     */
    void printBoard();

    // The default constructor instantiates the position with the standard chess starting position.
    Position();

    // The FEN constructor instantiates the position with the FEN string.
    Position(const char *fen);

    // The moveList constructor instantiates the position with the move list.
    // Position(moveList *moves);

    /**
     * @brief The placePiece function places a piece on the board.
     * @param piece The piece to place.
     * @param square The square to place the piece on.
     * @return True the previous piece on the square, referring to the pieces enum: 0 to 11 if there was a piece, 12 if there was no piece.
     */
    Piece placePiece(Piece piece, Square square);

    /**
     * @brief The fastPlacePiece function places a piece on the board.
     * @param piece The piece to place.
     * @param square The square to place the piece on. 
     */
    void fastPlacePiece(Piece piece, Square square);

    /**
     * @brief The pieceOn function returns the piece on the square.
     * @param square The square to check.
     * @return The piece on the square, according to the pieces enum: 0 to 11 if there was a piece, 12 if there was no piece.
     */
    Piece pieceOn(Square square);

    /**
     * @brief The isEmpty function tests if there is any piece on the square.
     * @param square The square to check.
     * @return True if there is no piece on the square, false otherwise.
     */
    bool isEmpty(Square square);

    /**
     * @brief The isEmpty function tests if there is any piece on the square, given a side.
     * @param square The square to check.
     * @param side The side to check.
     * @return True if there is a piece of given side on the square, false otherwise.
     */
    bool isEmpty(Square square, Side side);

    /**
     * @brief The isOccupied function tests if there is any piece of given a side on a gives square.
     * @param square The square to check.
     * @return True if there is a piece on the square, false otherwise.
     */
    bool isOccupied(Square square);

    /**
     * @brief The isOccupied function tests if there is any piece of given a side on a gives square.
     * @param square The square to check.
     * @param side The side to check.
     * @return True if there is a piece of given side on the square, false otherwise.
     */
    bool isOccupied(Square square, Side side);

    /**
     * @brief The removeSpecificPiece function removes a piece from the board.
     * @param piece The piece to remove.
     * @param square The square to remove the piece from.
     */
    void removeSpecificPiece(Piece piece, Square square);

    /**
     * @brief The fastRemovePiece function removes a piece from the board.
     * @param square The square to remove the piece from.
     */
    void fastRemovePiece(Square square);

    /**
     * @brief The removePiece function removes a piece from the board.
     * @param square The square to remove the piece from.
     * @return The piece removed, according to the pieces enum: 0 to 11 if there was a piece, 12 if there was no piece.
     */
    Piece removePiece(Square square);

    /**
     * @brief The parseFEN function parses a FEN string, and places the pieces on the board. It also sets the turn, castling rights, en passant square, half move clock and full move number.
     * It is filled with sanity checks, and will print an error message containing the error if it finds one.
     * @param fen The FEN string to parse.
     * @return True if the FEN string was parsed successfully, false otherwise (state of the position is undefined). 
     */
    bool parseFEN(const char *fen);

    /**
     * @brief The wipe function wipes the position.
     */
    void wipe();

    /**
     * @brief The isSquareAttacked function tests if a square is attacked by a given side.
     * @param square The square to check.
     * @param side The side to check for attacks.
     * @return True if the square is attacked, false otherwise.
     */
    bool isSquareAttacked(Square square, Side side);

    /**
     * @brief The countSquaresAttacks function counts how many pieces of a given side attack a given square.
     * @param square The square to check.
     * @param side The side to check for attacks.
     * @return The number of pieces of the given side attacking the square.
     */
    U8 countSquaresAttacks(Square square, Side side);

    /**
     * @brief The generateMoves function generates all the pseudo-legal moves for the current position.
     * @param moves The move list to add the moves to.
     * @note Moves are pseudo-legal, meaning that they are legal if the king is not in check. This will be checked by the search routine.
     */
    void generateMoves(MoveList *moves);

    /**
     * @brief The getLegalOf function gets a move string and compares to the list of legal moves.
     * @param move The move to check.
     * @return The legal move, or NULL if the move is not legal.
     */
    Move getLegalOf(char * move);
};