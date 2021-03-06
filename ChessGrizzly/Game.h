#pragma once

// The game class handles the game logic, making moves, checking for wins, etc.
#include "Position.h"
#include "tables.h"
#include "evaluation.h"

const U8 maxPly = 64;
extern U32 mainHistory[maxPly][12][64];
extern Move counterMoves[2][64][64];
extern Move killerMoves[2][maxPly];
extern U16 ply;

extern U8 pvLenght[maxPly];
extern Move pvTable[maxPly][maxPly];

class Game {
public:
    /**
     * @brief The _perft function is a helper function for the perft function.
     * @param depth The depth of the search.
     * @return The number of nodes at the given depth.
     */
    U64 _perft(int depth);
    
    U64 nodes = 0;
    Position pos;
    Depth depth = 0;
    U64 moveTime = 0;
    U32 wtime = 0;
    U32 btime = 0;
    U32 winc = 0;
    U32 binc = 0;
    volatile bool stopped = false;

    

    // The bestMove variable is used to store the best move found by the last completed search.
    Move bestMove = 0;

    /**
     * @brief The startSearch function is the main function for the search. It calls the search function with the internal parameters.
     * @note The parameters are set via the UCI protocol.
     */
    void startSearch();


    /**
     * @brief The print function prints the internal position to stdout.
     */
    void print();

    /**
     * @brief The parseFEN function parses a FEN string and sets the internal position accordingly.
     * @param fen The FEN string to parse.
     * @return true if the FEN string was parsed successfully, false otherwise.
     */
    bool parseFEN(const char *fen);

    /**
     * @brief The generate move function generates a move for the current position and stores it in the given MoveList.
     * @param ml The MoveList to store the generated move in.
     */
    void generateMoves(MoveList *ml);

    /**
     * @brief The perft function calculates the number of nodes in the current position for a given depth.
     * @param depth The depth to calculate the nodes for.
     * @return The number of nodes in the current position for the given depth.
     * @note This function is used for debugging. It will reset the node count.
     */
    U64 perft(int depth);

    /**
     * @brief The makeMove function makes a move on the internal position.
     * @param m The move to make.
     * @return true if the move was made successfully, false otherwise.
     */
    bool makeMove(Move m);

    /**
     * @brief The 
     function will execute the perft function for each move in the current position at the given depth.
     * @param depth The depth to divide the position at.
     * @return The number of nodes in the current position for the given depth.
     */
    U64 divide(int depth);

    // The game constructor.
    Game();

    // The FEN constructor.
    Game(const char *fen);

    /**
     * @brief The negaMax function is the main search function. It will search the current position for the given depth.
     * It will also use alpha-beta pruning to reduce the search space.
     * @param alpha The alpha value for the alpha-beta pruning.
     * @param beta The beta value for the alpha-beta pruning.
     * @param depth The depth to search at.
     */
    Score negaMax(Score alpha, Score beta, Depth depth);

    /**
     * @brief The quiescence function is a special search function for the quiescence search. It will search the current position for the given depth.
     * It will also use alpha-beta pruning to reduce the search space.
     * @param alpha The alpha value for the alpha-beta pruning.
     * @param beta The beta value for the alpha-beta pruning.
     */
    Score quiescence(Score alpha, Score beta);

    /**
     * @brief The getLegalOf function is a wrapper for the internal getLegalOf of the internal position.
     * @param move the move string
     * @return The complete move encoding if the string corresponds to a legal move in the current position, 0 (null move) otherwise.
     */
    Move getLegalOf(char* move);

    /**
     * @brief The executeMoveList function executes a list of moves on the internal position. It is used for the UCI interface.
     * @param ml The null terminated string containing the list of moves to execute.
     * @return true if the move list was executed successfully, false otherwise. (State of the position is undefined in this case.)
     * @note This function is used for the UCI interface.
     */
    bool executeMoveList(char* ml);

    /**
     * @brief The reset function resets the internal position to the starting position, as well as resetting all the internal variables.
     * @note This function is used for the UCI interface.
     */
    void reset();

    /**
     * @brief The makeNullMove function makes a null move on the internal position.
     */
    void makeNullMove();
};