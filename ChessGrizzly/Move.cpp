#include "Move.h"
#include <iostream>
#include "BBmacros.h"
void printMove(Move move){
    std::cout << squareNames[sourceSquare(move)] << squareNames[targetSquare(move)];
    if(promotion(move) != EMPTY){
        std::cout << promotionPieces[promotion(move)];
    }
}