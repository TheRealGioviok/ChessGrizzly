// ChessGrizzly.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include "BBmacros.h"
#include "Move.h"
#include "tables.h"
#include "Position.h"
#include <intrin.h>
int main()
{
    std::cout << "Chess Grizzly by Gioviok\n";
    initAll();

    Position pos;
    
    pos.printBoard();

    MoveList moveList;

    pos.generateMoves(&moveList);

    //printMoveList(&moveList);
}