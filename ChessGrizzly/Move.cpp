#include "Move.h"
#include <iostream>
#include "BBmacros.h"

void printMove(Move move){
    std::cout << squareNames[sourceSquare(move)] << squareNames[targetSquare(move)];
    if(promotion(move) != EMPTY){
        std::cout << promotionPieces[promotion(move)];
    }
}

void addMoveToList(ScoredMove move, MoveList *list){
    list->moves[list->count++] = move;
}

void printMoveList(MoveList *moveList, bool printScore){
    for(int i = 0; i < moveList->count; i++){
        if(printScore){
            std::cout << scoreOfMove(moveList->moves[i]) << " ";
        }
        printMove(onlyMove(moveList->moves[i]));
        std::cout << std::endl;
    }
}