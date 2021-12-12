// ChessGrizzly.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include <fstream>
#include <string>
#include "BBmacros.h"
#include "Move.h"
#include "tables.h"
#include "Game.h"
#include "Position.h"
#include "uci.h"
#include <intrin.h>
int main() {
    std::cout << "          ----------------------------------" << std::endl;
    std::cout << "          |========= ChessGrizzly =========|" << std::endl;
    std::cout << "          |======== Version: 0.1.0 ========|" << std::endl;
    std::cout << "          |===== Author: G.M. Manduca =====|" << std::endl;
    std::cout << "          ----------------------------------" << std::endl << std::endl;

    initAll();

    Game game;


    // We test the uci command parser
    uciLoop(&game);

    executeCommand(&game, (char*)std::string("position startpos moves e2e4 e7e5").c_str());
    executeCommand(&game, (char*)std::string("print").c_str());

    return 0;
}