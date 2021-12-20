#include "uci.h"
#include "Game.h"
#include <io.h>
#include <Windows.h>

int quit = 0;

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
    // - divide : prints the perft results for the current position, divided for each move
    // - movelist : wrapper for "divide 1"

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
    char* divide = strstr((char *)command, "divide");
    char* moveList = strstr((char *)command, "movelist");

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
    // - divide
    // - movelist

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
        std::cout << "Static evaluation for current position (from side to move perspective): " << pestoEval(&(game->pos)) << std::endl;
    }

    if (divide){
        std::cout << "Perft results for current position:" << std::endl;
        game->divide(atoi(divide + 7));
    }

    if (moveList){
        game->divide(1);
    }
    return 0;
}

int positionCommand(Game *game, char* command){
    // the position command is composed by the following subcommands:
    // - fen <fen_string> : sets the position to the specified fen string
    // - startpos : sets the position to the starting position
    // - trickypos : sets the position to the tricky position (debugging purposes)
    // - moves <move_list> : sets the position to the specified move list

    // we will search for the first occurrence of a command

    game->pos.wipe();

    char* fen = strstr((char *)command, "fen");
    char* startposCMD = strstr((char *)command, "startpos");
    char* trickyposCMD = strstr((char *)command, "trickypos");
    char* moves = strstr((char *)command, "moves");

    if(startposCMD){
        game->parseFEN(startpos);
    }

    if(trickyposCMD){
        game->parseFEN(trickyPosition);
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
    else {
        // if no movetime is specified, we set the movetime to infinite
        game->moveTime = -1; // (-1 underflows to max uint64_t)
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
        game->winc = atoi(winc + 5);
    }

    if(binc){
        game->binc = atoi(binc + 5);
    }

    game->startSearch();

    return 0;
}

#define MAX_INPUT_LENGTH 2000
void uciLoop(Game* game){

    // Print engine prompt
    std::cout << "Chess Grizzly> ";

    // The uciLoop is implemented in a way that it can be interrupted by the user, even if the engine is searching
    // The user can type "quit" to exit the engine

    std::cout.flush();
    std::cin.clear();
    std::cin.ignore(500, '\n');
    std::cin.sync();

    // define user / GUI input buffer
    char userInput[MAX_INPUT_LENGTH];

    // print the engine info
    std::cout << "id name Chess Grizzly 0.1" << std::endl;
    std::cout << "id author G.M. Manduca" << std::endl;
    std::cout << "uciok" << std::endl;

    std::string input = "";

    // loop
    while(1){
        // print the engine prompt
        std::cout << std::endl << "Chess Grizzly> ";

        // reset the input string
        input = "";

        // read the user input
        std::cin.getline(userInput, MAX_INPUT_LENGTH);

        //print received input
        std::cout << "Received input: " << userInput << std::endl;

        // if recived quit during search
        if (quit){
            return;
        }


        // make sure the user input is not new line
        if (userInput[0] == '\n' || userInput == ""){
            continue;
        }

        // execute the command
        if(executeCommand(game, userInput) == 1 || userInput == "quit") {
            break;
        }

        // flush the output
        std::cout.flush();

        std::cout << std::flush;

        // clear the input buffer
        std::cin.clear();

        // sync the input buffer
        std::cin.sync();

        // clear the user input buffer
        memset(userInput, 0, MAX_INPUT_LENGTH);
    }
}

int inputWaiting(){
    static bool initialized = false;
    static int pipe;
    static HANDLE inputHandle;
    DWORD dw;

    if (!initialized) {
        inputHandle = GetStdHandle(STD_INPUT_HANDLE);
        pipe = !GetConsoleMode(inputHandle, &dw);
        initialized = true;
        if (!pipe){
            SetConsoleMode(inputHandle, dw & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(inputHandle);
        }
    }

    if (pipe) {
        if (!PeekNamedPipe(inputHandle, NULL, 0, NULL, &dw, NULL))
            return dw;
    }
    else {
        GetNumberOfConsoleInputEvents(inputHandle, &dw);
        return dw <= 1 ? 0 : dw;
    }


}

void readInput(Game *game){
    int bytes;
    char buffer[256] = "";
    char *endc;

    // listen to stdin
    if (inputWaiting()){
        game->stopped = true;

        do {
            bytes = _read(_fileno(stdin), buffer, 256);
        } while (bytes < 0);
        
        endc = strchr(buffer, '\n');
        if (endc) endc = nullptr;

        if (strlen(buffer) > 0){
            if (!strcmp(buffer, "quit")){
                quit = 1;
                return;
            }
            if (!strcmp(buffer, "stop")){
                quit = 1;
                return;
            }
        }
    }
}

void communicate(Game *game){
    // if time is up, stop the search
    if (getTime64() > game->moveTime){
        game->stopped = true;
    }
    readInput(game);
}