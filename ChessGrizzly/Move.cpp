#include "Move.h"
#include <iostream>
#include "BBmacros.h"

#include "Game.h"

void printMove(Move move){
    std::cout << squareNames[sourceSquare(move)] << squareNames[targetSquare(move)];
    if(promotionOf(move) != EMPTY){
        std::cout << promotionPieces[promotionOf(move)];
    }
}

void addMoveToList(ScoredMove move, MoveList *list){
    // In order to sort the moves, we will put a score in the upper 32 bits of the move.
    // The lower 32 bits will be the move itself.

    // The score is dependant on:
    // 1. TT hit (to be implemented)
    // 2. MVV/LVA
    // 3. Killer moves
    // 4. Counter moves (to be implemented)
    // 5. History heuristic

    // 1. TT hit

    // 2. MVV/LVA
    U32 score = 0;
    Piece capture;
    if((capture = pieceCaptured(move)) != EMPTY){
        score += MvvLvaTable[pieceMoved(move)][capture] << 24;
    }

    Piece promotion = promotionOf(move);
    if(promotion != EMPTY){
        score += ((promotion % 6) + 1) << 20;
    }

    Move oMove = onlyMove(move);

    // 3. Killer moves
    if (oMove == killerMoves[0][ply]) {
        score += 1 << 15;
    }
    // 2 killers from this ply, 1 from 2 plies ago
    else if (oMove == killerMoves[1][ply]) {
        score += 1 << 14;
    }
    else if (oMove == killerMoves[0][ply - 2]) {
        score += 1 << 13;
    }
    else if (oMove == killerMoves[1][ply - 2]) {
        score += 1 << 11;
    }
    // counter move
    else if (oMove == counterMoves[ply][sourceSquare(move)][targetSquare(move)]) {
        score += 1 << 12;
    }
    else {
        score += 1 << (isCheckMove(move) * 5);
        score += mainHistory[ply][pieceMoved(move)][targetSquare(move)];
    }
    
    list->moves[list->count++] = (score << 32) | move;
}

void printMoveList(MoveList *moveList, bool printScore){
    for(int i = 0; i < moveList->count; i++){
        if(moveList->moves[i] == 0){
            continue;
        }
        if(printScore){
            std::cout << scoreOfMove(moveList->moves[i]) << " ";
        }
        printMove(onlyMove(moveList->moves[i]));
        std::cout << std::endl;
    }
}