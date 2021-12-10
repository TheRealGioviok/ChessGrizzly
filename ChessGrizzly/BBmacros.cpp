#include "BBmacros.h"

void printBitBoard(BitBoard board){
    // loop over ranks
    for (int rank = 0; rank < 8; rank++){
        // loop over files
        std::cout << " " << 7 - rank + 1 << "  ";
        for (int file = 0; file < 8; file++){
            Square s = rank * 8 + file;
            // print the bitboard
            std::cout << testBit(board, s) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << std::hex << "value: "<< board << std::dec << std::endl;
}

Piece pieceFromChar(char pieceChar){
    switch (pieceChar){
        case 'P':
            return P;
        case 'N':
            return N;
        case 'B':
            return B;
        case 'R':
            return R;
        case 'Q':
            return Q;
        case 'K':
            return K;
        case 'p':
            return p;
        case 'n':
            return n;
        case 'b':
            return b;
        case 'r':
            return r;
        case 'q':
            return q;
        case 'k':
            return k;
        default:
            return EMPTY;
    }
}

Square squareFromName(const char *squareName){
    // We expect square names to be in the form of "a1"
    // so we can just use the ASCII values to get the file and rank
    // and then multiply by 8 to get the square number
    int rank = 7 - (squareName[1] - '1');
    int file = squareName[0] - 'a';
    if (rank < 0 || rank > 7 || file < 0 || file > 7){
        return noSquare;
    }
    return rank * 8 + file;
}