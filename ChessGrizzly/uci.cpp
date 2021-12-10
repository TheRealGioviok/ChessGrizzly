#include "uci.h"

int executeCommand(Game *game, char* command){
    // Technically speaking, each possible command is present only once in the command, so we can look for the first character
    // Command supported:
    // - isready : returns "readyok" if the engine is ready to accept commands
    // - uci : returns engine info, followed by "uciok"
    // - setoption name <option_name> value <option_value> (To implement)
    // - ucinewgame : resets the engine to its initial state
    // - position fen <fen_string> : sets the position to the given fen string
    // - position startpos moves <move_list> : sets the position to the given move list
    // - go [depth / movetime / infinite] <num>: starts the engine to search for the best move using the parameters specified in the command
    // - stop : stops the engine from searching
    // - quit : exits the engine
    // - print : prints the current position
    // - eval : prints the static evaluation of the current position

    // we will search for the first occurrence of a command
    char* isReady = strstr((char *)command, "isready");
    char* uci = strstr((char *)command, "uci");
    char* setOption = strstr((char *)command, "setoption");
    char* uciNewGame = strstr((char *)command, "ucinewgame");
    char* position = strstr((char *)command, "position");
    char* go = strstr((char *)command, "go");
    char* stop = strstr((char *)command, "stop");
    char* quit = strstr((char *)command, "quit");
    char* print = strstr((char *)command, "print");
    char* eval = strstr((char *)command, "eval");

    // The order of execution is the following:
    // - isready
    // - uci
    // - setoption
    // - ucinewgame
    // - position
    // - go
    // - stop
    // - quit
    // - print
    // - eval

    if (isReady){
        std::cout << "readyok" << std::endl;
    }

    if (uci){
        std::cout << "id name " << "Chess Grizzly" << std::endl;
        std::cout << "id author " << "G.M. Manduca" << std::endl;
        std::cout << "uciok" << std::endl;
    }

    if (setOption){
        std::cout << "The setoption command is not yet implemented" << std::endl;
        // TODO: implement setoption
    }

    if (uciNewGame){
        game->reset();
        std::cout << "uciok" << std::endl;
    }

    if (position){
        positionCommand(game, command);
    }

    if (go){
        goCommand(game, command);
    }

    if (stop){
        game->stopped = true;
    }

    if (quit){
        return 1;
    }

    if (print){
        game->print();
    }

    if (eval){
        std::cout << "The eval command is not yet implemented" << std::endl;
    }

    return 0;
}

int positionCommand(Game *game, char* command){
    // the position command is composed by the following subcommands:
    // - fen <fen_string> : sets the position to the specified fen string
    // - startpos : sets the position to the starting position
    // - moves <move_list> : sets the position to the specified move list

    // we will search for the first occurrence of a command
    char* fen = strstr((char *)command, "fen");
    char* startposCMD = strstr((char *)command, "startpos");
    char* moves = strstr((char *)command, "moves");

    if(startposCMD){
        game->parseFEN(startpos);
    }

    if(fen){
        game->parseFEN(fen + 4);
    }

    if(moves){
        game->executeMoveList(moves + 6);
    }

    return 0;
}

int goCommand(Game* game, char* command){
    // the go command is composed by the following subcommands:
    // - depth <num> : sets the depth to the specified value
    // - movetime <num> : sets the movetime to the specified value
    // - infinite : sets the movetime to infinite
    // - wtime <num> : sets the white time to the specified value
    // - btime <num> : sets the black time to the specified value
    // - winc <num> : sets the white increment to the specified value
    // - binc <num> : sets the black increment to the specified value

    // we will search for the first occurrence of a command
    char* depth = strstr((char *)command, "depth");
    char* movetime = strstr((char *)command, "movetime");
    char* infinite = strstr((char *)command, "infinite");
    char* wtime = strstr((char *)command, "wtime");
    char* btime = strstr((char *)command, "btime");
    char* winc = strstr((char *)command, "winc");
    char* binc = strstr((char *)command, "binc");

    if(depth){
        game->depth = atoi(depth + 6);
    }

    if(movetime){
        game->moveTime = atoi(movetime + 9);
    }

    if(infinite){
        game->moveTime = 0xFFFFFFFFFFFFFFFF;
        game->depth = 64;
    }

    if(wtime){
        game->wtime = atoi(wtime + 6);
    }

    if(btime){
        game->btime = atoi(btime + 6);
    }

    if(winc){
        game->winc = atoi(winc + 6);
    }

    if(binc){
        game->binc = atoi(binc + 6);
    }

    game->startSearch();

    return 0;
}