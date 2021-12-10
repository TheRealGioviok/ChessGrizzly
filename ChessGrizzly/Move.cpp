#include "Move.h"
#include <iostream>
#include "BBmacros.h"

void printMove(Move move){
    std::cout << squareNames[sourceSquare(move)] << squareNames[targetSquare(move)];
    if(promotionOf(move) != EMPTY){
        std::cout << promotionPieces[promotionOf(move)];
    }
}

void addMoveToList(ScoredMove move, MoveList *list){
    list->moves[list->count++] = move;
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