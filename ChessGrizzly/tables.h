// The tables.h file contains all the table used in the engine.
// It also contains the reference to some dynamically allocated table (the hash table).
#include "BBmacros.h"

// Files and ranks
#define bbFileA 0x0101010101010101
#define bbFileB 0x0202020202020202
#define bbFileC 0x0404040404040404
#define bbFileD 0x0808080808080808
#define bbFileE 0x1010101010101010
#define bbFileF 0x2020202020202020
#define bbFileG 0x4040404040404040
#define bbFileH 0x8080808080808080

#define bbRank1 0x00000000000000FF
#define bbRank2 0x000000000000FF00
#define bbRank3 0x0000000000FF0000
#define bbRank4 0x00000000FF000000
#define bbRank5 0x000000FF00000000
#define bbRank6 0x0000FF0000000000
#define bbRank7 0x00FF000000000000
#define bbRank8 0xFF00000000000000

#define bbNotFileA 0xFEFEFEFEFEFEFEFE
#define bbNotFileB 0xFDFDFDFDFDFDFDFD
#define bbNotFileC 0xFBFBFBFBFBFBFBFB
#define bbNotFileD 0xF7F7F7F7F7F7F7F7
#define bbNotFileE 0xEFEFEFEFEFEFEFEF
#define bbNotFileF 0xDFDFDFDFDFDFDFDF
#define bbNotFileG 0xBFBFBFBFBFBFBFBF
#define bbNotFileH 0x7F7F7F7F7F7F7F7F

#define bbNotRank1 0xFFFFFFFFFFFFFF00
#define bbNotRank2 0xFFFFFFFFFFFF00FF
#define bbNotRank3 0xFFFFFFFFFF00FFFF
#define bbNotRank4 0xFFFFFFFF00FFFFFF
#define bbNotRank5 0xFFFFFF00FFFFFFFF
#define bbNotRank6 0xFFFF00FFFFFFFFFF
#define bbNotRank7 0xFF00FFFFFFFFFFFF
#define bbNotRank8 0x00FFFFFFFFFFFFFF

#define bbWhiteSquares 0x55AA55AA55AA55AA
#define bbBlackSquares 0xAA55AA55AA55AA55

const BitBoard bbFile[8] = { bbFileA, bbFileB, bbFileC, bbFileD, bbFileE, bbFileF, bbFileG, bbFileH };
const BitBoard bbRank[8] = {bbRank1, bbRank2, bbRank3, bbRank4, bbRank5, bbRank6, bbRank7, bbRank8};
const BitBoard bbNotFile[8] = {bbNotFileA, bbNotFileB, bbNotFileC, bbNotFileD, bbNotFileE, bbNotFileF, bbNotFileG, bbNotFileH};
const BitBoard bbNotRank[8] = {bbNotRank1, bbNotRank2, bbNotRank3, bbNotRank4, bbNotRank5, bbNotRank6, bbNotRank7, bbNotRank8};
const BitBoard bbColors[2] = {bbWhiteSquares, bbBlackSquares};

// The attack tables
extern BitBoard pawnAttacks[2][64];
extern BitBoard knightAttacks[64];
extern BitBoard kingAttacks[64];

/**
 * @brief The maskPawnAttacks function populates the pawn attack table for a given color and square.
 * @param color The color of the pawn.
 * @param square The square of the pawn.
 * @return The pawn attack table for the given color and square.
 * @note This function is used at the beginning of the program. It shouldn't be called at any other time.
 */
BitBoard maskPawnAttacks(Color color, Square square);

/**
 * @brief The initPawnAttacks function populates the pawn attack table for a given color.
 * @note This function is called at the beginning of the program. It shouldn't be called at any other time.
 */
void initPawnAttacks();

/**
 * @brief The maskBishopAttacks function will mask the relevant occupancy bits for the bishop on a given square.
 * @param square The square of the bishop.
 * @return The relevant occupancy bits for the bishop on a given square.
 * @note This function is used at the beginning of the program. It shouldn't be called at any other time.
 */
BitBoard maskBishopAttacks(Square square);

/**
 * @brief The maskRookAttacks function will mask the relevant occupancy bits for the rook on a given square.
 * @param square The square of the rook.
 * @return The relevant occupancy bits for the rook on a given square.
 * @note This function is used at the beginning of the program. It shouldn't be called at any other time.
 */
BitBoard maskRookAttacks(Square square);

/**
 * @brief The generateBishopAttacksOnTheFly function will generate the bishop attack table for a given square and occupancy.
 * @param square The square of the bishop.
 * @param occupancy The occupancy of the board.
 * @return The bishop attack table for a given square.
 * @note This function is called at the beginning of the program. It shouldn't be called at any other time.
 */
BitBoard generateBishopAttacksOnTheFly(Square square, BitBoard occupancy);

/**
 * @brief The generateRookAttacksOnTheFly function will generate the rook attack table for a given square and occupancy.
 * @param square The square of the rook.
 * @param occupancy The occupancy of the board.
 * @return The rook attack table for a given square.
 * @note This function is called at the beginning of the program. It shouldn't be called at any other time.
 */
BitBoard generateRookAttacksOnTheFly(Square square, BitBoard occupancy);

/**
 * @brief The setOccupancy function will return the occupancy board for a given index, bitsIndex and occupancy.
 * The idea of this function is to be used to generate all the possible occupancy variations.
 * The maximum number of occupancy boards is actually just 4096 (for the rooks) and 512 (for the bishops).
 * @param index The index of the occupancy board.
 * @param bitsInMask The bits in mask of the occupancy board.
 * @param attackMask The attack mask of the occupancy board.
 * @return The occupancy board for a given index, bitsInMask and attackMask.
 * @note This function is called at the beginning of the program. It shouldn't be called at any other time.
 */
BitBoard setOccupancy(int index, BitBoard bitsInMask, BitBoard attackMask);
