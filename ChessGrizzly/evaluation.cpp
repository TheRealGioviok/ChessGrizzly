#include "evaluation.h"

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

Score pestoEval(Position *pos){
    Score mg[2] = { 0, 0 };
    Score eg[2] = { 0, 0 };
    Score gamePhase = 0;

    // White pawns
    {
        BitBoard pawns = pos->bitboards[P];
        while (pawns){
            unsigned long pawn;
            bitScanForward(&pawn, pawns);
            clearBit(pawns, pawn);

            mg[WHITE] += mgTables[P][pawn];
            eg[WHITE] += egTables[P][pawn];
            gamePhase = gamephaseInc[P];
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

    Score mgScore = mg[side] - mg[other];
    Score egScore = eg[side] - eg[other];

    // phase is capped to 24, in case of early promotion
    Score mgPhase = min(24, gamePhase);
    Score egPhase = 24 - mgPhase;

    return (mgScore * mgPhase + egScore * egPhase) / 24;

}
