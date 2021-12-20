#include "evaluation.h"
#include "tables.h"

Score simpleEval(Position *pos){
    // We will return the sum of the material of the board, from the player to move perspective.
    Score score = 0;

    score += popCount(pos->bitboards[P]) * 100;
    score -= popCount(pos->bitboards[p]) * 100;

    score += popCount(pos->bitboards[N]) * 320;
    score -= popCount(pos->bitboards[n]) * 320;

    score += popCount(pos->bitboards[B]) * 330;
    score -= popCount(pos->bitboards[b]) * 330;

    score += popCount(pos->bitboards[R]) * 500;
    score -= popCount(pos->bitboards[r]) * 500;

    score += popCount(pos->bitboards[Q]) * 900;
    score -= popCount(pos->bitboards[q]) * 900;

    score *= 1 - 2 * pos->turn;

    return score;
}

Score mgTables[12][64] = {{0}};
Score egTables[12][64] = {{0}};

Score *mgPestoTables[6] = {
    (Score *)mgPawnTable,
    (Score *)mgKnightTable,
    (Score *)mgBishopTable,
    (Score *)mgRookTable,
    (Score *)mgQueenTable,
    (Score *)mgKingTable};

Score *egPestoTables[6] = {
    (Score *)egPawnTable,
    (Score *)mgKnightTable,
    (Score *)mgBishopTable,
    (Score *)mgRookTable,
    (Score *)mgQueenTable,
    (Score *)mgKingTable};

void initTables(){
    for (Piece piece = P; piece <= K; piece++){
        forEachSquare(square){
            std::cout << "Initializing PeSTo tables: " << square + piece*64 + 1<< "/384\r";
            // White
            mgTables[piece][square] = mgPestoTables[piece][square] + mgValues[piece];
            egTables[piece][square] = egPestoTables[piece][square] + egValues[piece];
            // Black
            mgTables[piece + 6][square] = mgPestoTables[piece][flipSquare(square)] + mgValues[piece];
            egTables[piece + 6][square] = egPestoTables[piece][flipSquare(square)] + egValues[piece];
        }
    }
    std::cout << std::endl;
}
#define MG_PAWNMOB 3
#define MG_PAWN_PASSER_BONUS 5
#define EG_PAWNMOB 5
#define EG_PAWN_PASSER_BONUS 15
#define MG_DOUBLEDPAWN -15
#define MG_ISOLATEDPAWN -5
#define MG_BACKWARDPAWN -5
#define EG_DOUBLEDPAWN -40
#define EG_ISOLATEDPAWN -15
#define EG_BACKWARDPAWN -15

#define MG_KNIGHTMOB 4
#define EG_KNIGHTMOB 5
#define MG_ATTACKEDKNIGHTMOB 2
#define EG_ATTACKEDKNIGHTMOB 2

#define MG_BISHOPMOB 2
#define EG_BISHOPMOB 3
#define MG_ATTACKEDBISHOPMOB 1
#define EG_ATTACKEDBISHOPMOB 2

#define MG_ROOKMOB 1
#define EG_ROOKMOB 3
#define MG_ATTACKEDROOKMOB 0
#define EG_ATTACKEDROOKMOB 1
#define MG_ROOKSEMIOPENFILE 5
#define MG_ROOKOPENFILE 10
#define EG_ROOKSEMIOPENFILE 10
#define EG_ROOKOPENFILE 20
#define MG_ROOKSEVENTH 15
#define EG_ROOKSEVENTH 20

#define MG_QUEENMOB 1
#define EG_QUEENMOB 5
#define MG_ATTACKEDQUEENMOB 0
#define EG_ATTACKEDQUEENMOB 2


#define MOBILITYEVAL true



Score pestoEval(Position *pos){
    Score mg[2] = { 0, 0 };
    Score eg[2] = { 0, 0 };
    Score gamePhase = 0;

    BitBoard whiteOccupancy = pos->bitboards[P] | pos->bitboards[N] | pos->bitboards[B] | pos->bitboards[R] | pos->bitboards[Q] | pos->bitboards[K];
    BitBoard blackOccupancy = pos->bitboards[p] | pos->bitboards[n] | pos->bitboards[b] | pos->bitboards[r] | pos->bitboards[q] | pos->bitboards[k];
    BitBoard occupancy = whiteOccupancy | blackOccupancy;
    BitBoard freeSquares = ~occupancy;

    BitBoard attackedByWhitePawns = 0ULL;
    BitBoard attackedByBlackPawns = 0ULL;

    // White pawns P, N, B, R, Q, K - p, n, b, r, q, k
    {
        BitBoard pawns = pos->bitboards[P]; // We need this copy because we will modify it
        BitBoard pawnsBB = pawns; // We need this copy to evaluate positional scores of pawns
        while (pawns){
            unsigned long pawn;
            bitScanForward(&pawn, pawns);
            clearBit(pawns, pawn);

            mg[WHITE] += mgTables[P][pawn];
            eg[WHITE] += egTables[P][pawn];
            gamePhase = gamephaseInc[P];
#if MOBILITYEVAL
            // we also will account for the mobility of the pawn
            // we will add the number of squares that the pawn can move to
            // we will add the number of squares that the pawn can attack
            // 1: can the pawn move forward?
            mg[WHITE] += ((squaresAhead[WHITE][pawn] & freeSquares) > 0) * MG_PAWNMOB;
            eg[WHITE] += ((squaresAhead[WHITE][pawn] & freeSquares) > 0) * EG_PAWNMOB;
            // 2: is the pawn a passed pawn?
            mg[WHITE] += ((squaresAhead[WHITE][pawn] & blackOccupancy) == 0) * MG_PAWN_PASSER_BONUS;
            eg[WHITE] += ((squaresAhead[WHITE][pawn] & blackOccupancy) == 0) * EG_PAWN_PASSER_BONUS;
            // 3: is the pawn a doubled pawn?
            mg[WHITE] += ((pawnsBB & squaresAhead[WHITE][pawn]) > 0) * MG_DOUBLEDPAWN;
            eg[WHITE] += ((pawnsBB & squaresAhead[WHITE][pawn]) > 0) * EG_DOUBLEDPAWN;
            // 4: is the pawn an isolated pawn?
            mg[WHITE] += ((pawnsBB & adjacentFiles[fileOf(pawn)]) == 0) * MG_ISOLATEDPAWN;
            eg[WHITE] += ((pawnsBB & adjacentFiles[fileOf(pawn)]) == 0) * EG_ISOLATEDPAWN;
            // 5: is the pawn a backward pawn?
            mg[WHITE] += ((pawnsBB & backwardPawnMask[WHITE][pawn]) == 0) * MG_BACKWARDPAWN;
            eg[WHITE] += ((pawnsBB & backwardPawnMask[WHITE][pawn]) == 0) * EG_BACKWARDPAWN;

            // we update the attackedByWhitePawns bitboard
            attackedByWhitePawns |= pawnAttacks[WHITE][pawn];
#endif
        }
    }
    // Black pawns
    {
        BitBoard pawns = pos->bitboards[p];
        while (pawns){
            unsigned long pawn;
            bitScanForward(&pawn, pawns);
            clearBit(pawns, pawn);

            mg[BLACK] += mgTables[p][pawn];
            eg[BLACK] += egTables[p][pawn];
            gamePhase = gamephaseInc[p];

#if MOBILITYEVAL
            // we also will account for the mobility of the pawn
            // we will add the number of squares that the pawn can move to
            // we will add the number of squares that the pawn can attack
            // 1: can the pawn move forward?
            mg[BLACK] += ((squaresAhead[BLACK][pawn] & freeSquares) > 0) * MG_PAWNMOB;
            eg[BLACK] += ((squaresAhead[BLACK][pawn] & freeSquares) > 0) * EG_PAWNMOB;
            // 2: is the pawn a passed pawn?
            mg[BLACK] += ((squaresAhead[BLACK][pawn] & whiteOccupancy) == 0) * MG_PAWN_PASSER_BONUS;
            eg[BLACK] += ((squaresAhead[BLACK][pawn] & whiteOccupancy) == 0) * EG_PAWN_PASSER_BONUS;
            // 3: is the pawn a doubled pawn?
            mg[BLACK] += ((pawns & squaresAhead[BLACK][pawn]) > 0) * MG_DOUBLEDPAWN;
            eg[BLACK] += ((pawns & squaresAhead[BLACK][pawn]) > 0) * EG_DOUBLEDPAWN;
            // 4: is the pawn an isolated pawn?
            mg[BLACK] += ((pawns & adjacentFiles[fileOf(pawn)]) == 0) * MG_ISOLATEDPAWN;
            eg[BLACK] += ((pawns & adjacentFiles[fileOf(pawn)]) == 0) * EG_ISOLATEDPAWN;
            // 5: is the pawn a backward pawn?
            mg[BLACK] += ((pawns & backwardPawnMask[BLACK][pawn]) == 0) * MG_BACKWARDPAWN;
            eg[BLACK] += ((pawns & backwardPawnMask[BLACK][pawn]) == 0) * EG_BACKWARDPAWN;
            
            // we update the attackedByBlackPawns bitboard
            attackedByBlackPawns |= pawnAttacks[BLACK][pawn];
#endif
        }
    }
    // White knights
    {
        BitBoard knights = pos->bitboards[N];
        while (knights){
            unsigned long knight;
            bitScanForward(&knight, knights);
            clearBit(knights, knight);

            mg[WHITE] += mgTables[N][knight];
            eg[WHITE] += egTables[N][knight];
            gamePhase = gamephaseInc[N];

#if MOBILITYEVAL
            // we also will account for the mobility of the knight
            // we will add the number of squares that the knight can move to (and give them a score based on if they are attacked by an enemy pawn)
            BitBoard moves = knightAttacks[knight];

            mg[WHITE] += popCount((moves & attackedByBlackPawns)) * MG_ATTACKEDKNIGHTMOB;
            mg[WHITE] += popCount((moves & ~attackedByBlackPawns)) * MG_KNIGHTMOB;
            eg[WHITE] += popCount((moves & attackedByBlackPawns)) * EG_ATTACKEDKNIGHTMOB;
            eg[WHITE] += popCount((moves & ~attackedByBlackPawns)) * EG_KNIGHTMOB;
#endif
        }
    }
    // Black knights
    {
        BitBoard knights = pos->bitboards[n];
        while (knights){
            unsigned long knight;
            bitScanForward(&knight, knights);
            clearBit(knights, knight);

            mg[BLACK] += mgTables[n][knight];
            eg[BLACK] += egTables[n][knight];
            gamePhase = gamephaseInc[n];

#if MOBILITYEVAL
            // we also will account for the mobility of the knight
            // we will add the number of squares that the knight can move to (and give them a score based on if they are attacked by an enemy pawn)
            BitBoard moves = knightAttacks[knight];

            mg[BLACK] += popCount((moves & attackedByWhitePawns)) * MG_ATTACKEDKNIGHTMOB;
            mg[BLACK] += popCount((moves & ~attackedByWhitePawns)) * MG_KNIGHTMOB;  
            eg[BLACK] += popCount((moves & attackedByWhitePawns)) * EG_ATTACKEDKNIGHTMOB;
            eg[BLACK] += popCount((moves & ~attackedByWhitePawns)) * EG_KNIGHTMOB;
#endif
        }
    }
    // White bishops
    {
        BitBoard bishops = pos->bitboards[B];
        while (bishops){
            unsigned long bishop;
            bitScanForward(&bishop, bishops);
            clearBit(bishops, bishop);

            mg[WHITE] += mgTables[B][bishop];
            eg[WHITE] += egTables[B][bishop];
            gamePhase = gamephaseInc[B];

#if MOBILITYEVAL
            // we also will account for the mobility of the bishop
            // we will add the number of squares that the bishop can move to (and give them a score based on if they are attacked by an enemy pawn)
            BitBoard moves = getBishopAttack(bishop, occupancy) & ~whiteOccupancy;

            mg[WHITE] += popCount((moves & attackedByBlackPawns)) * MG_ATTACKEDBISHOPMOB;
            mg[WHITE] += popCount((moves & ~attackedByBlackPawns)) * MG_BISHOPMOB;
            eg[WHITE] += popCount((moves & attackedByBlackPawns)) * EG_ATTACKEDBISHOPMOB;
            eg[WHITE] += popCount((moves & ~attackedByBlackPawns)) * EG_BISHOPMOB;
#endif
        }
    }
    // Black bishops
    {
        BitBoard bishops = pos->bitboards[b];
        while (bishops){
            unsigned long bishop;
            bitScanForward(&bishop, bishops);
            clearBit(bishops, bishop);

            mg[BLACK] += mgTables[b][bishop];
            eg[BLACK] += egTables[b][bishop];
            gamePhase = gamephaseInc[b];

#if MOBILITYEVAL
            // we also will account for the mobility of the bishop
            // we will add the number of squares that the bishop can move to (and give them a score based on if they are attacked by an enemy pawn)
            BitBoard moves = getBishopAttack(bishop, occupancy) & ~blackOccupancy;

            mg[BLACK] += popCount((moves & attackedByWhitePawns)) * MG_ATTACKEDBISHOPMOB;
            mg[BLACK] += popCount((moves & ~attackedByWhitePawns)) * MG_BISHOPMOB;
            eg[BLACK] += popCount((moves & attackedByWhitePawns)) * EG_ATTACKEDBISHOPMOB;
            eg[BLACK] += popCount((moves & ~attackedByWhitePawns)) * EG_BISHOPMOB;
#endif
        }
    }
    // White rooks
    {
        BitBoard rooks = pos->bitboards[R];
        while (rooks){
            unsigned long rook;
            bitScanForward(&rook, rooks);
            clearBit(rooks, rook);

            mg[WHITE] += mgTables[R][rook];
            eg[WHITE] += egTables[R][rook];
            gamePhase = gamephaseInc[R];

#if MOBILITYEVAL
            // we also will account for the mobility of the rook
            // we will add the number of squares that the rook can move to (and give them a score based on if they are attacked by an enemy pawn)
            BitBoard moves = getRookAttack(rook, occupancy) & ~whiteOccupancy;

            // mg[WHITE] += popCount((moves & attackedByBlackPawns)) * MG_ATTACKEDROOKMOB; (Since MG_ATTACKEDROOKMOB is 0)
            mg[WHITE] += popCount((moves & ~attackedByBlackPawns)) * MG_ROOKMOB;
            eg[WHITE] += popCount((moves & attackedByBlackPawns)) * EG_ATTACKEDROOKMOB;
            eg[WHITE] += popCount((moves & ~attackedByBlackPawns)) * EG_ROOKMOB;

            // We will give a bonus to the rook if it is on a semi-open file
            mg[WHITE] += ((squaresAhead[WHITE][rook] & blackOccupancy) == 0) * MG_ROOKSEMIOPENFILE;
            eg[WHITE] += ((squaresAhead[WHITE][rook] & blackOccupancy) == 0) * EG_ROOKSEMIOPENFILE;
            mg[WHITE] += ((squaresAhead[WHITE][rook] & occupancy) == 0) * MG_ROOKOPENFILE;
            eg[WHITE] += ((squaresAhead[WHITE][rook] & occupancy) == 0) * EG_ROOKOPENFILE;
            mg[WHITE] += (rankOf(rook) <= 1) * MG_ROOKSEVENTH;
            eg[WHITE] += (rankOf(rook) <= 1) * EG_ROOKSEVENTH;
#endif
        }
    }
    // Black rooks
    {
        BitBoard rooks = pos->bitboards[r];
        while (rooks){
            unsigned long rook;
            bitScanForward(&rook, rooks);
            clearBit(rooks, rook);

            mg[BLACK] += mgTables[r][rook];
            eg[BLACK] += egTables[r][rook];
            gamePhase = gamephaseInc[r];

#if MOBILITYEVAL
            // we also will account for the mobility of the rook
            // we will add the number of squares that the rook can move to (and give them a score based on if they are attacked by an enemy pawn)   
            BitBoard moves = getRookAttack(rook, occupancy) & ~blackOccupancy;

            // mg[BLACK] += popCount((moves & attackedByWhitePawns)) * MG_ATTACKEDROOKMOB; (Since MG_ATTACKEDROOKMOB is 0)
            mg[BLACK] += popCount((moves & ~attackedByWhitePawns)) * MG_ROOKMOB;
            eg[BLACK] += popCount((moves & attackedByWhitePawns)) * EG_ATTACKEDROOKMOB;
            eg[BLACK] += popCount((moves & ~attackedByWhitePawns)) * EG_ROOKMOB;

            // We will give a bonus to the rook if it is on a semi-open file
            mg[BLACK] += ((squaresAhead[BLACK][rook] & whiteOccupancy) == 0) * MG_ROOKSEMIOPENFILE;
            eg[BLACK] += ((squaresAhead[BLACK][rook] & whiteOccupancy) == 0) * EG_ROOKSEMIOPENFILE;
            mg[BLACK] += ((squaresAhead[BLACK][rook] & occupancy) == 0) * MG_ROOKOPENFILE;
            eg[BLACK] += ((squaresAhead[BLACK][rook] & occupancy) == 0) * EG_ROOKOPENFILE;
            mg[BLACK] += (rankOf(rook) >= 6) * MG_ROOKSEVENTH;
            eg[BLACK] += (rankOf(rook) >= 6) * EG_ROOKSEVENTH;
#endif
        }
    }
    // White queens
    {
        BitBoard queens = pos->bitboards[Q];
        while (queens){
            unsigned long queen;
            bitScanForward(&queen, queens);
            clearBit(queens, queen);

            mg[WHITE] += mgTables[Q][queen];
            eg[WHITE] += egTables[Q][queen];
            gamePhase = gamephaseInc[Q];

#if MOBILITYEVAL
            // we also will account for the mobility of the queen
            // we will add the number of squares that the queen can move to (and give them a score based on if they are attacked by an enemy pawn)
            BitBoard moves = getQueenAttack(queen, occupancy) & ~whiteOccupancy;

            // mg[WHITE] += popCount((moves & attackedByBlackPawns)) * MG_ATTACKEDQUEENMOB; (Since MG_ATTACKEDQUEENMOB is 0)
            mg[WHITE] += popCount((moves & ~attackedByBlackPawns)) * MG_QUEENMOB;
            eg[WHITE] += popCount((moves & attackedByBlackPawns)) * EG_ATTACKEDQUEENMOB;
            eg[WHITE] += popCount((moves & ~attackedByBlackPawns)) * EG_QUEENMOB;
#endif
        }
    }
    //Black queens
    {
        BitBoard queens = pos->bitboards[q];
        while (queens){
            unsigned long queen;
            bitScanForward(&queen, queens);
            clearBit(queens, queen);

            mg[BLACK] += mgTables[q][queen];
            eg[BLACK] += egTables[q][queen];
            gamePhase = gamephaseInc[q];

#if MOBILITYEVAL
            // we also will account for the mobility of the queen
            // we will add the number of squares that the queen can move to (and give them a score based on if they are attacked by an enemy pawn)
            BitBoard moves = getQueenAttack(queen, occupancy) & ~blackOccupancy;

            // mg[BLACK] += popCount((moves & attackedByWhitePawns)) * MG_ATTACKEDQUEENMOB; (Since MG_ATTACKEDQUEENMOB is 0)
            mg[BLACK] += popCount((moves & ~attackedByWhitePawns)) * MG_QUEENMOB;
            eg[BLACK] += popCount((moves & attackedByWhitePawns)) * EG_ATTACKEDQUEENMOB;
            eg[BLACK] += popCount((moves & ~attackedByWhitePawns)) * EG_QUEENMOB;
#endif
        }
    }
    // White King (no need to iterate, since there is only one king per side)
    {
        unsigned long king;
        bitScanForward(&king, pos->bitboards[K]);

        mg[WHITE] += mgTables[K][king];
        eg[WHITE] += egTables[K][king];
        gamePhase = gamephaseInc[K];
    }
    // Black King (no need to iterate, since there is only one king per side)
    {
        unsigned long king;
        bitScanForward(&king, pos->bitboards[k]);

        mg[BLACK] += mgTables[k][king];
        eg[BLACK] += egTables[k][king];
        gamePhase = gamephaseInc[k];
    }

    // Tapered eval
    Side side = pos->turn;
    Side other = side ^ 1;

    // add a tempo bonus
    mg[side] += 23;
    eg[side] += 11;

    Score mgScore = mg[side] - mg[other];
    Score egScore = eg[side] - eg[other];

    // phase is capped to 24, in case of early promotion
    Score mgPhase = min(24, gamePhase);
    Score egPhase = 24 - mgPhase;
 
    // We add a tempo bonus to the score, meaning that we will give a bonus to the side that has to move (10 cp)
    // Score tempoBonus = (side == WHITE) ? 10 : -10;

    return (mgScore * mgPhase + egScore * egPhase) / 24;

}
