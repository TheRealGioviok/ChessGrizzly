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
    std::cout << "Chess Grizzly by Gioviok\n";
    initAll();

    Game game;


    // We test the uci command parser
    game.print();
    while (1) {
        std::string command;
        std::getline(std::cin, command);
        executeCommand(&game, (char*)command.c_str());
        game.print();
    }
    return 0;
}