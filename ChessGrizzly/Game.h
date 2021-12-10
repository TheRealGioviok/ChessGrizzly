#pragma once

// The game class handles the game logic, making moves, checking for wins, etc.
#include "Position.h"
#include "tables.h"

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
    U16 ply = 0;

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
     * @brief The divide function will execute the perft function for each move in the current position at the given depth.
     * @param depth The depth to divide the position at.
     * @return The number of nodes in the current position for the given depth.
     */
    U64 divide(int depth);

    // The game constructor.
    Game();
};