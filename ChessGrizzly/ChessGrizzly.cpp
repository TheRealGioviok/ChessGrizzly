// ChessGrizzly.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include "BBmacros.h"
#include "Move.h"
#include "tables.h"
#include "Game.h"
#include "Position.h"
#include <intrin.h>
int main() {
    std::cout << "Chess Grizzly by Gioviok\n";
    initAll();

    Game game;
    game.parseFEN("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1");

    std::cout << encodeMove(d5, c6, P, p, EMPTY, true, false, false, false) << " = ";
    printMove(encodeMove(d5, c6, P, p, EMPTY, true, false, false, false));
    std::cout << "\n";

    getchar();
    //game.makeMove(encodeMove(d5, c6, P, p, EMPTY, true, false, false, false));
    game.print();
    for (int i = 1; i <= 7; i++) {
        std::cout << game.divide(i) << "\n";
        std::cout << "==================================" << std::endl;
        getchar();
    }
    

    return 0;
}