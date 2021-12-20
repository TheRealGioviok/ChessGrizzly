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