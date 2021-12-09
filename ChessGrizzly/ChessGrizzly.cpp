// ChessGrizzly.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include "BBmacros.h"
#include "tables.h"
#include "Position.h"
#include <intrin.h>
int main()
{
    std::cout << "Chess Grizzly by Gioviok\n";
    initAll();

    Position pos;
    pos.printBoard();

    forEachSquare(square){
        std::cout << (int)pos.countSquaresAttacks(square, WHITE) << " ";
        if (square % 8 == 7)
            std::cout << "\n";
    }
    
}