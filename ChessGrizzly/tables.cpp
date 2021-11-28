#include "BBmacros.h"
#include "tables.h"

BitBoard pawnAttacks[2][64];

BitBoard knightAttacks[64] = {  0x20400, 0x50800, 0xa1100, 0x142200, 0x284400, 0x508800, 0xa01000, 0x402000,
                                0x2040004, 0x5080008, 0xa110011, 0x14220022, 0x28440044, 0x50880088, 0xa0100010, 0x40200020,
                                0x204000402, 0x508000805, 0xa1100110a, 0x1422002214, 0x2844004428, 0x5088008850, 0xa0100010a0, 0x4020002040,
                                0x20400040200, 0x50800080500, 0xa1100110a00, 0x142200221400, 0x284400442800, 0x508800885000, 0xa0100010a000, 0x402000204000,
                                0x2040004020000, 0x5080008050000, 0xa1100110a0000, 0x14220022140000, 0x28440044280000, 0x50880088500000, 0xa0100010a00000, 0x40200020400000,
                                0x204000402000000, 0x508000805000000, 0xa1100110a000000, 0x1422002214000000, 0x2844004428000000, 0x5088008850000000, 0xa0100010a0000000, 0x4020002040000000,
                                0x400040200000000, 0x800080500000000, 0x1100110a00000000, 0x2200221400000000, 0x4400442800000000, 0x8800885000000000, 0x100010a000000000, 0x2000204000000000,
                                0x4020000000000, 0x8050000000000, 0x110a0000000000, 0x22140000000000, 0x44280000000000, 0x88500000000000, 0x10a00000000000, 0x20400000000000
                            };

BitBoard kingAttacks[64] = {    0x302, 0x705, 0xe0a, 0x1c14, 0x3828, 0x7050, 0xe0a0, 0xc040,
                                0x30203, 0x70507, 0xe0a0e, 0x1c141c, 0x382838, 0x705070, 0xe0a0e0, 0xc040c0,
                                0x3020300, 0x7050700, 0xe0a0e00, 0x1c141c00, 0x38283800, 0x70507000, 0xe0a0e000, 0xc040c000,
                                0x302030000, 0x705070000, 0xe0a0e0000, 0x1c141c0000, 0x3828380000, 0x7050700000, 0xe0a0e00000, 0xc040c00000,
                                0x30203000000, 0x70507000000, 0xe0a0e000000, 0x1c141c000000, 0x382838000000, 0x705070000000, 0xe0a0e0000000, 0xc040c0000000,
                                0x3020300000000, 0x7050700000000, 0xe0a0e00000000, 0x1c141c00000000, 0x38283800000000, 0x70507000000000, 0xe0a0e000000000, 0xc040c000000000,
                                0x302030000000000, 0x705070000000000, 0xe0a0e0000000000, 0x1c141c0000000000, 0x3828380000000000, 0x7050700000000000, 0xe0a0e00000000000, 0xc040c00000000000,
                                0x203000000000000, 0x507000000000000, 0xa0e000000000000, 0x141c000000000000, 0x2838000000000000, 0x5070000000000000, 0xa0e0000000000000, 0x40c0000000000000
                            };

BitBoard maskPawnAttacks(Color color, Square square)
{
    BitBoard result = 0;
    BitBoard bbSquare = squareBitBoard(square);
    if (color == WHITE)
    {
        result |= (bbSquare >> 7) & bbNotFileA;
        result |= (bbSquare >> 9) & bbNotFileH;
    }
    else
    {
        result |= (bbSquare << 7) & bbNotFileH;
        result |= (bbSquare << 9) & bbNotFileA;
    }
    return result;
}

void initPawnAttacks(){
    for (Square square = a8; square <= h1; square++){
        pawnAttacks[WHITE][square] = maskPawnAttacks(WHITE, square);
        pawnAttacks[BLACK][square] = maskPawnAttacks(BLACK, square);
    }
}

BitBoard maskBishopAttacks(Square square){
    // The result bitboard
    BitBoard attacks = 0ULL;

    // init rank and file
    int targetRank = rankOf(square);
    int targetFile = fileOf(square);

    // mask relevant bishop occupancy bits
    for (int rank = targetRank + 1, file = targetFile + 1; rank <= 6 && file <= 6; rank++, file++) attacks |= squareBitBoard(makeSquare(file, rank));
    for (int rank = targetRank + 1, file = targetFile - 1; rank <= 6 && file >= 1; rank++, file--) attacks |= squareBitBoard(makeSquare(file, rank));
    for (int rank = targetRank - 1, file = targetFile + 1; rank >= 1 && file <= 6; rank--, file++) attacks |= squareBitBoard(makeSquare(file, rank));
    for (int rank = targetRank - 1, file = targetFile - 1; rank >= 1 && file >= 1; rank--, file--) attacks |= squareBitBoard(makeSquare(file, rank));
    return attacks;
}

BitBoard maskRookAttacks(Square square){
    // The result bitboard
    BitBoard attacks = 0ULL;

    // init rank and file
    int targetRank = rankOf(square);
    int targetFile = fileOf(square);

    // mask relevant rook occupancy bits
    for (int rank = targetRank + 1; rank <= 6; rank++) attacks |= squareBitBoard(makeSquare(targetFile, rank));
    for (int rank = targetRank - 1; rank >= 1; rank--) attacks |= squareBitBoard(makeSquare(targetFile, rank));
    for (int file = targetFile + 1; file <= 6; file++) attacks |= squareBitBoard(makeSquare(file, targetRank));
    for (int file = targetFile - 1; file >= 1; file--) attacks |= squareBitBoard(makeSquare(file, targetRank));
    return attacks;
}

BitBoard generateBishopAttacksOnTheFly(Square square, BitBoard occupancy){
    // The result bitboard
    BitBoard attacks = 0ULL;

    // init rank and file
    int targetRank = rankOf(square);
    int targetFile = fileOf(square);

    // mask relevant bishop occupancy bits
    for (int rank = targetRank + 1, file = targetFile + 1; rank <= 7 && file <= 7; rank++, file++) {
        attacks |= squareBitBoard(makeSquare(file, rank));
        if (occupancy & squareBitBoard(makeSquare(file, rank))) break;
    }
    for (int rank = targetRank + 1, file = targetFile - 1; rank <= 7 && file >= 0; rank++, file--) {
        attacks |= squareBitBoard(makeSquare(file, rank));
        if (occupancy & squareBitBoard(makeSquare(file, rank))) break;
    }
    for (int rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file <= 7; rank--, file++) {
        attacks |= squareBitBoard(makeSquare(file, rank));
        if (occupancy & squareBitBoard(makeSquare(file, rank))) break;
    }
    for (int rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--) {
        attacks |= squareBitBoard(makeSquare(file, rank));
        if (occupancy & squareBitBoard(makeSquare(file, rank))) break;
    }

    // return the result
    return attacks;
}

BitBoard generateRookAttacksOnTheFly(Square square, BitBoard occupancy){
    // The result bitboard
    BitBoard attacks = 0ULL;

    // init rank and file
    int targetRank = rankOf(square);
    int targetFile = fileOf(square);

    // mask relevant rook occupancy bits
    for (int rank = targetRank + 1; rank <= 7; rank++) {
        attacks |= squareBitBoard(makeSquare(targetFile, rank));
        if (occupancy & squareBitBoard(makeSquare(targetFile, rank))) break;
    }
    for (int rank = targetRank - 1; rank >= 0; rank--) {
        attacks |= squareBitBoard(makeSquare(targetFile, rank));
        if (occupancy & squareBitBoard(makeSquare(targetFile, rank))) break;
    }
    for (int file = targetFile + 1; file <= 7; file++) {
        attacks |= squareBitBoard(makeSquare(file, targetRank));
        if (occupancy & squareBitBoard(makeSquare(file, targetRank))) break;
    }
    for (int file = targetFile - 1; file >= 0; file--) {
        attacks |= squareBitBoard(makeSquare(file, targetRank));
        if (occupancy & squareBitBoard(makeSquare(file, targetRank))) break;
    }

    // return the result
    return attacks;
}

BitBoard setOccupancy(int index, BitBoard bitsInMask, BitBoard attackMask){
    BitBoard occupancy = 0ULL;
    for (int i = 0; i < bitsInMask; i++){
        // pop the least significant bit
        unsigned long square;
        bitScanForward(&square, attackMask);
        clearBit(attackMask, square);
        // make sure that occupancy is on board
        if (index & squareBitBoard(i)){
            occupancy |= squareBitBoard(i);
        }
    }
    return occupancy;
}