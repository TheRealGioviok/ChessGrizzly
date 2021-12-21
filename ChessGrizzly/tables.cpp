#include "BBmacros.h"
#include "tables.h"
#include "evaluation.h"
#include <cassert>


U32 state = 1804289383;

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

// The squaresAhead array contains the bitmask of squares ahead of a piece for a given side
BitBoard squaresAhead[2][64] = { { 0 } };

// The adjacentFiles array contains the bitmask of adjacent files for a given file (used for pawn evaluation)
BitBoard adjacentFiles[8] = { 0 };

// The backwardPawnMask array contains the bitmask of squares that, if not occupied by any other pawn, render the pawn on the current square a backward pawn
BitBoard backwardPawnMask[2][64] = { { 0 } };

// The bishop's attack mask
BitBoard bishopMask[64];

// The rook's attack mask
BitBoard rookMask[64];

// The bishop's attack table [square][occupancy]
U64 bishopAttackTable[64][512];

// The rook's attack table [square][occupancy]
U64 rookAttackTable[64][4096];

// The LMR reduction table
U8 reductionTable[128][128] = { { 0 } };

// The random piece keys
HashKey pieceKeys[12][64] = { { 0 } };

// The enpassant keys
HashKey enPassantKeys[65] = { 0 };

// The castling keys
HashKey castlingKeys[16] = { 0 };

// The side to move key
HashKey sideKey = 0;

U32 randomNumber32()
{
    // get state
    U32 currState = state;

    // XOR shift algorithm (www.wikipedia.org/wiki/Xorshift)
    currState ^= currState << 13;
    currState ^= currState >> 17;
    currState ^= currState << 5;

    // update state
    state = currState;

    // return the random number
    return currState;
}

U64 randomNumber64(){
    U64 n1, n2, n3, n4;
    n1 = randomNumber32() & 0xFFFF;
    n2 = randomNumber32() & 0xFFFF;
    n3 = randomNumber32() & 0xFFFF;
    n4 = randomNumber32() & 0xFFFF;
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 generateMagicNumber(){


    U64 n1 = randomNumber64();
    U64 n2 = randomNumber64();
    U64 n3 = randomNumber64();

    return n1 & n2 & n3;
}

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
    forEachSquare(square){
        pawnAttacks[WHITE][square] = maskPawnAttacks(WHITE, square);
        pawnAttacks[BLACK][square] = maskPawnAttacks(BLACK, square);
        std::cout << "Initializing pawn attacks: " << square + 1 << "/64\r";
    }
    std::cout << std::endl;
}

BitBoard maskBishopAttacks(Square square){
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant bishop occupancy bits
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
        attacks |= (1ULL << (r * 8 + f));

    // return attack map
    return attacks;
}

BitBoard maskRookAttacks(Square square){
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant rook occupancy bits
    for (r = tr + 1; r <= 6; r++)
        attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
        attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
        attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
        attacks |= (1ULL << (tr * 8 + f));

    // return attack map
    return attacks;
}

BitBoard generateBishopAttacksOnTheFly(Square square, BitBoard occupancy){
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // generate bishop atacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & occupancy)
            break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & occupancy)
            break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & occupancy)
            break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & occupancy)
            break;
    }

    // return attack map
    return attacks;
}

BitBoard generateRookAttacksOnTheFly(Square square, BitBoard occupancy){
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // generate rook attacks
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & occupancy)
            break;
    }

    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & occupancy)
            break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & occupancy)
            break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & occupancy)
            break;
    }

    // return attack map
    return attacks;
}

BitBoard setOccupancy(int index, BitBoard bitsInMask, BitBoard attackMask){
    // occupancy map
    U64 occupancy = 0ULL;

    // loop over the range of bits within attack mask
    for (int count = 0; count < bitsInMask; count++)
    {
        // get LS1B index of attacks mask
        unsigned long square;
        bitScanForward(&square, attackMask);

        // pop LS1B in attack map
        clearBit(attackMask, square);

        // make sure occupancy is on board
        if (index & (1 << count))
            // populate occupancy map
            occupancy |= (1ULL << square);
    }

    // return occupancy map
    return occupancy;
}

U64 findMagicNumber(Square square, U8 relevantBitCount, bool slider){

    // init all the occupancy bitboards
    BitBoard* occupancy = new BitBoard[4096];
    BitBoard* attacks = new BitBoard[4096];
    BitBoard* usedAttacks = new BitBoard[4096];

    // Init attack mask
    BitBoard attackMask = slider ? maskBishopAttacks(square) : maskRookAttacks(square);

    // init occypancy indices
    int occupancyIndex = 1 << relevantBitCount;

    // loop over occupancy indices
    for (int index = 0; index < occupancyIndex; index++){
        // init occupancy bitboard
        occupancy[index] = setOccupancy(index, relevantBitCount, attackMask);
        // init attacks
        attacks[index] = slider ? generateBishopAttacksOnTheFly(square, occupancy[index]) : generateRookAttacksOnTheFly(square, occupancy[index]);
    }
    // test magic numbers loop
    #define BIGNUMBER 100000000
    for (int count = 0; count < BIGNUMBER; count++){
        // generate magic number candidate
        U64 magicNumber = generateMagicNumber();

        // skip inappropriate magic numbers ( those with too few bits set )
        if (popCount(magicNumber) < 6) continue;

        // init used attacks
        std::fill(usedAttacks, usedAttacks + 4096, 0ULL);

        // init index
        int index = 0;

        // fail flag

        bool fail = false;

        // test the magic index loop
        for(index = 0; !fail && index < occupancyIndex; index++){
            // init magic index
            int magicIndex = (magicNumber * occupancy[index]) >> (64 - relevantBitCount);
            
            // If magic index works
            if (usedAttacks[magicIndex] == 0ULL){
                // set used attacks
                usedAttacks[magicIndex] = attacks[index];
            }
            else if (usedAttacks[magicIndex] != attacks[index]){
                // magic index doesn't work
                fail = true;
            }
        }

        // If magic number works
        if (!fail){
            // return magic number
            delete[] occupancy;
            delete[] attacks;
            delete[] usedAttacks;
            return magicNumber;
        }
    }

    // If no magic number works ( this should never happen ) 
    std::cout << "No magic number found" << std::endl;
    delete [] occupancy;
    delete [] attacks;
    delete [] usedAttacks;
    exit(1);
}

void initMagicNumbers() {
    // loop over squares
    std::cout << "Bishop magic numbers: \n";
    for (Square square = 0; square <= h1; square++)
        std::cout << std::hex << findMagicNumber(square, bishopRelevantBits[square], true) << "\n";
    std::cout << "\n\nRook magic numbers: \n";
    for (Square square = 0; square <= h1; square++)
        std::cout << std::hex << findMagicNumber(square, rookRelevantBits[square], false) << "\n";
}

void initBishopAttacks(){
    // Iterate through all squares
    forEachSquare(square){
        // Init mask
        bishopMask[square] = maskBishopAttacks(square);

        // Init current Mask
        BitBoard attackMask = bishopMask[square];

        // relevant bit count
        int relevantBitCount = bishopRelevantBits[square];

        // Init occupancy index
        int occupancyIndex = 1 << relevantBitCount;

        // loop over occupancy indices
        for (int index = 0; index < occupancyIndex; index++){
            // init occupancy bitboard
            BitBoard occupancy = setOccupancy(index, relevantBitCount, attackMask);
            // init magic index
            int magicIndex = (bishopMagicNumbers[square] * occupancy) >> (64 - relevantBitCount);
            // init attacks
            bishopAttackTable[square][index] = generateBishopAttacksOnTheFly(square, occupancy);
        }
    }
}

void initRookAttacks(){
    // Iterate through all squares
    forEachSquare(square){
        // Init mask
        rookMask[square] = maskRookAttacks(square);

        // Init current Mask
        BitBoard attackMask = rookMask[square];

        // relevant bit count
        int relevantBitCount = rookRelevantBits[square];

        // Init occupancy index
        int occupancyIndex = 1 << relevantBitCount;

        // loop over occupancy indices
        for (int index = 0; index < occupancyIndex; index++){
            // init occupancy bitboard
            BitBoard occupancy = setOccupancy(index, relevantBitCount, attackMask);
            // init magic index
            int magicIndex = (rookMagicNumbers[square] * occupancy) >> (64 - relevantBitCount);
            // init attacks
            rookAttackTable[square][index] = generateRookAttacksOnTheFly(square, occupancy);
        }
    }
}

void initSliders(bool bishop){

    // loop over squares
    for (Square square = 0; square <= h1; square++){
        std::cout << "Initializing " << (bishop ? "bishop" : "rook") << " moves : " << square + 1 << "/64\r";
        // loop over 64 board squares
        for (int square = 0; square < 64; square++) {
            // init bishop & rook masks
            bishopMask[square] = maskBishopAttacks(square);
            rookMask[square] = maskRookAttacks(square);

            // init current mask
            U64 attackMask = bishop ? bishopMask[square] : rookMask[square];

            // init relevant occupancy bit count
            int relevant_bits_count = popCount(attackMask);

            // init occupancy indicies
            int occupancy_indicies = (1 << relevant_bits_count);
            
            // loop over occupancy indicies
            for (int index = 0; index < occupancy_indicies; index++)
            {
                // bishop
                if (bishop)
                {
                    // init current occupancy variation
                    U64 occupancy = setOccupancy(index, relevant_bits_count, attackMask);

                    // init magic index
                    int magicIndex = (occupancy * bishopMagicNumbers[square]) >> (64 - bishopRelevantBits[square]);

                    // init bishop attacks
                    bishopAttackTable[square][magicIndex] = generateBishopAttacksOnTheFly(square, occupancy);
                }

                // rook
                else
                {
                    // init current occupancy variation
                    U64 occupancy = setOccupancy(index, relevant_bits_count, attackMask);

                    // init magic index
                    int magicIndex = (occupancy * rookMagicNumbers[square]) >> (64 - rookRelevantBits[square]);

                    // init bishop attacks
                    rookAttackTable[square][magicIndex] = generateRookAttacksOnTheFly(square, occupancy);
                }
            }
        }
    }
    std::cout << std::endl;
}

inline BitBoard getBishopAttack(Square square, BitBoard occupancy) {
    // Get right occupancy
    occupancy &= bishopMask[square];
    occupancy *= bishopMagicNumbers[square];
    occupancy >>= 64 - bishopRelevantBits[square];

    // Return the attack table
    return bishopAttackTable[square][occupancy];
}

inline BitBoard getRookAttack(Square square, BitBoard occupancy) {
    // Get right occupancy
    occupancy &= rookMask[square];
    occupancy *= rookMagicNumbers[square];
    occupancy >>= 64 - rookRelevantBits[square];

    // Return the attack table
    return rookAttackTable[square][occupancy];
}

inline BitBoard getQueenAttack(Square square, BitBoard occupancy) {
    // Return rook attacks unioned with bishop attacks
    return getBishopAttack(square, occupancy) | getRookAttack(square, occupancy);
}


void initLMRTable(){
    for (int depth = 1; depth < 128; depth++) {
        for (int move = 0; move < 128; move++) {
            U8 result = std::log((std::log(depth) * std::log(move) / 1.95));
            reductionTable[depth][move] = result;
        }
    }
    // account for reductionTable[2][2], since the logarithm causes it to be 255
    reductionTable[2][2] = 0;
}

void initUtilityMasks(){
    // squares ahead of a given square for a given side
    forEachSquare(square) {
        // White: Squares ahead are all the bits to the right of the square, & with the file mask
        // Black: Squares ahead are all the bits to the left of the square, & with the file mask
        squaresAhead[WHITE][square] = (squareBitBoard(square) - 1) & bbFile[fileOf(square)];
        squaresAhead[BLACK][square] = bbFile[fileOf(square)] ^ (squaresAhead[WHITE][square] ^ squareBitBoard(square));
    }

    // adjacent files
    forEachFile(file){
        if (file)
            adjacentFiles[file] |= bbFile[file - 1];
        if (file < 7)
            adjacentFiles[file] |= bbFile[file + 1];
    }

    // backward pawn
    forEachSquare(square) {
        int file = fileOf(square);
        if (file) {
            backwardPawnMask[WHITE][square] = squaresAhead[BLACK][square - 1];
            backwardPawnMask[BLACK][square] = squaresAhead[WHITE][square - 1];
        }
        if (file < 7) {
            backwardPawnMask[WHITE][square] |= squaresAhead[BLACK][square + 1];
            backwardPawnMask[BLACK][square] |= squaresAhead[WHITE][square + 1];
        }
    }
}

void initHashKeys(){
    // piece/square keys
    forEachPiece(piece) {
        forEachSquare(square) {
            pieceKeys[piece][square] = randomNumber64();
        }
    }

    // en passant keys
    forEachSquare(square){
        enPassantKeys[square] = randomNumber64();
    }

    // castling keys
    for(int i = 0; i < 16; i++){
        castlingKeys[i] = randomNumber64();
    }

    // side to move key
    sideKey = randomNumber64();
}


void initAll(){

    U64 timeStart = getTime64();

    initPawnAttacks();
    initSliders(true);
    initSliders(false);
    initTables();
    initLMRTable();
    initUtilityMasks();
    initHashKeys();


    std::cout << "Initialization completed in " << (getTime64() - timeStart) / 1000 << " ms" << std::endl;
    std::cout << "Type 'isReady' to continue" << std::endl << std::endl;
    // initBishopAttacks();
    // initRookAttacks();
}

