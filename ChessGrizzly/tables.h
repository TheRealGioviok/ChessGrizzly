#pragma once

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

// Bonus for low depth move ordering
const int centerBonusTable10[64] = {0, 10, 20, 30, 30, 20, 10, 0,
                                    10, 20, 20, 30, 30, 20, 20, 10,
                                    0, 10, 30, 30, 30, 30, 10, 0,
                                    5, 10, 25, 30, 30, 20, 10, 5,
                                    5, 10, 20, 30, 30, 25, 10, 5,
                                    0, 10, 30, 30, 30, 30, 10, 0,
                                    10, 20, 20, 30, 30, 20, 20, 10,
                                    0, 10, 20, 30, 30, 20, 10, 0};

// LMR reduction table
extern U8 reductionTable[128][128];

// Magic bitboards stuff
const U8 bishopRelevantBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const U8 rookRelevantBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

// The bishop's magic numbers
const U64 bishopMagicNumbers[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL
};

// The rook's magic numbers
const U64 rookMagicNumbers[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL
};

const U32 MvvLvaTable[12][12] = {
    105, 205, 305, 405, 505, 605,   105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,   104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,   103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,   102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,   101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,   100, 200, 300, 400, 500, 600,

    105, 205, 305, 405, 505, 605,   105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,   104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,   103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,   102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,   101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,   100, 200, 300, 400, 500, 600
};

// The squaresAhead array contains the bitmask of squares ahead of a piece for a given side
extern BitBoard squaresAhead[2][64];

// The adjacentFiles array contains the bitmask of adjacent files for a given file (used for pawn evaluation)
extern BitBoard adjacentFiles[8];

// The backwardPawnMask array contains the bitmask of squares that, if not occupied by any other pawn, render the pawn on the current square a backward pawn
extern BitBoard backwardPawnMask[2][64];

// The bishop's attack mask
extern BitBoard bishopMask[64];

// The rook's attack mask
extern BitBoard rookMask[64];

// The bishop's attack table [square][occupancy]
extern U64 bishopAttackTable[64][512];

// The rook's attack table [square][occupancy]
extern U64 rookAttackTable[64][4096];

// Random state variable, to be used in the random generator
extern U32 state;

// The random piece keys
extern HashKey pieceKeys[12][64];

// The enpassant keys
extern HashKey enPassantKeys[65]; // The extra one is for when there is no enpassant, and it is set to 0 (doesn't change the hash)

// The castling keys
extern HashKey castlingKeys[16];

// The side to move key
extern HashKey sideKey;

/**
 * @brief The random32 function returns a random number between 0 and 2^32 - 1 using the XOR shift algorithm.
 * @return A random number between 0 and 2^32 - 1.
 * @note This function alters the state of the random generator at each call.
 */
U32 randomNumber32();

/**
 * @brief The randomNumber64 function returns a random number between 0 and 2^64 - 1 by generating 4 32-bit numbers and combining them.
 * @return A random number between 0 and 2^64 - 1.
 */
U64 randomNumber64();

/**
 * @brief The generateMagicNumber function generates a random magic number candidate.
 * A good candidate is one that has a low Hamming weight and is not a power of two.
 */
U64 generateMagicNumber();

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

/**
 * @brief The findMagicNumber function will find the magic number for a given square, relevantBitCount and slider direction.
 * @param square The square of the magic number.
 * @param relevantBitCount The number of relevant bits for the magic number.
 * @param slider If the magic number is for a bishop or a rook.
 * @return The magic number for a given square, relevantBitCount and occupancy.
 */
U64 findMagicNumber(Square square, U8 relevantBitCount, bool slider);

/**
 * @brief The initMagicNumbers function is used to generate all the magic numbers for the rooks and bishops.
 * @note This function is never called, because its return values are hardcoded.
 */
void initMagicNumbers();

/**
 * @brief The initSliders function is used to populate the attack masks and attack tables for the rooks or bishops.
 * @param bishop True if bishop, false if rook.
 * @note This function shouldn't be called at any other time other than at the beginning of the program.
 */
void initSliders();

/**
 * @brief The getBishopAttack function will return the bishop attack table for a given square and occupancy.
 * @param square The square of the bishop.
 * @param occupancy The occupancy of the board.
 * @return The bishop attack table for a given square and occupancy.
 * @note This is the function to be called by move generation.
 */
extern inline BitBoard getBishopAttack(Square square, BitBoard occupancy);

/**
 * @brief The getRookAttack function will return the rook attack table for a given square and occupancy.
 * @param square The square of the rook.
 * @param occupancy The occupancy of the board.
 * @return The rook attack table for a given square and occupancy.
 * @note This is the function to be called by move generation.
 */
extern inline BitBoard getRookAttack(Square square, BitBoard occupancy);

/**
 * @brief The getQueenAttack function will return the queen attack table for a given square and occupancy. The function is a wrapper for the rook and bishop attack tables, since the queen is a combination of the rook and bishop.
 * @param square The square of the queen.
 * @param occupancy The occupancy of the board.
 * @return The queen attack table for a given square and occupancy.
 * @note This is the function to be called by move generation.
 */
extern inline BitBoard getQueenAttack(Square square, BitBoard occupancy);

/**
 * @brief The initAll function is used to initialize all the attack masks and attack tables, as well as some helper tables for evaluating positions.
 * @note This function should be called only at the beginning of the program (in the main function).
 */
void initAll();

/**
 * @brief The initBishopAttacks function is used to populate the bishop attack table.
 * @note This function should be called only at the beginning of the program (in the main function).
 */
void initBishopAttacks();

/**
 * @brief The initRookAttacks function is used to populate the rook attack table.
 * @note This function should be called only at the beginning of the program (in the main function).
 */
void initRookAttacks();

/**
 * @brief The initLMRTable function is used to populate the LMR table.
 * @note This function should be called only at the beginning of the program (in the main function).
 */
void initLMRTable();

/**
 * @brief The initUtilityMasks function is used to populate the utility masks.
 * These mask are later used to speed up the search, as well as the evaluation.
 * @note This function should be called only at the beginning of the program.
 */
void initUtilityMasks();

/**
 * @brief The initHashKeys function populates the keys table for each piece/square, enPassant square, castling rights and side to move.
 * @note This function should be called only at the beginning of the program.
 */
void initHashKeys();
