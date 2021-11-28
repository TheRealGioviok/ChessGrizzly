#include "BBmacros.h"

void printBitBoard(BitBoard board){
    // loop over ranks
    for (int rank = 0; rank < 8; rank++){
        // loop over files
        std::cout << " " << 7 - rank + 1 << "  ";
        for (int file = 0; file < 8; file++){
            Square s = (rank << 3) + file;
            // print the bitboard
            std::cout << testBit(board, s) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << std::hex << "value: "<< board << std::dec << std::endl;
}

