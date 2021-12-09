#include "Position.h"
#include "BBmacros.h"
#include "tables.h"
#include <windows.h>
#include <debugapi.h>


#include <cassert>


Piece Position::placePiece(Piece piece, Square square) {
    assert(piece < 12);
    assert(square < 64);
    Piece previousPiece = pieceOn(square);
    if (previousPiece < 12)
        removeSpecificPiece(previousPiece, square);
    fastPlacePiece(piece, square);
    return previousPiece;
}

void Position::fastPlacePiece(Piece piece, Square square) {
    assert(piece < 12);
    assert(square < 64);
    setBit(bitboards[piece], square);
}

Piece Position::pieceOn(Square square) {
    assert(square < 64);
    
    Piece result = 0;

    // Check for white pieces
    result += (12 - P) * testBit(bitboards[P], square);
    result += (12 - N) * testBit(bitboards[N], square);
    result += (12 - B) * testBit(bitboards[B], square);
    result += (12 - R) * testBit(bitboards[R], square);
    result += (12 - Q) * testBit(bitboards[Q], square);
    result += (12 - K) * testBit(bitboards[K], square);
    // and for black
    result += (12 - p) * testBit(bitboards[p], square);
    result += (12 - n) * testBit(bitboards[n], square);
    result += (12 - b) * testBit(bitboards[b], square);
    result += (12 - r) * testBit(bitboards[r], square);
    result += (12 - q) * testBit(bitboards[q], square);
    result += (12 - k) * testBit(bitboards[k], square);

    // if result remained 0, then the square was empty. Else, it was occupied by result - 1.
    return EMPTY - result;
}

bool Position::isEmpty(Square square) {
    assert(square < 64);
    return testBit((bitboards[P] | bitboards[N] | bitboards[B] | bitboards[R] | bitboards[Q] | bitboards[K] | bitboards[p] | bitboards[n] | bitboards[b] | bitboards[r] | bitboards[q] | bitboards[k]), square) == 0;
}

bool Position::isOccupied(Square square, Side side) {
    assert(square < 64);
    return testBit(bitboards[P + 6*side] | bitboards[N + 6*side] | bitboards[B + 6*side] | bitboards[R + 6*side] | bitboards[Q + 6*side] | bitboards[K + 6*side], square);
}


bool Position::isOccupied(Square square) {
    assert(square < 64);
    return testBit((bitboards[P] | bitboards[N] | bitboards[B] | bitboards[R] | bitboards[Q] | bitboards[K] | bitboards[p] | bitboards[n] | bitboards[b] | bitboards[r] | bitboards[q] | bitboards[k]), square) != 0;
}

void Position::removeSpecificPiece(Piece piece, Square square) {
    assert(piece < 12);
    assert(square < 64);
    clearBit(bitboards[piece], square);
}

void Position::fastRemovePiece(Square square) {
    assert(square < 64);
    clearBit(bitboards[P], square);
    clearBit(bitboards[N], square);
    clearBit(bitboards[B], square);
    clearBit(bitboards[R], square);
    clearBit(bitboards[Q], square);
    clearBit(bitboards[K], square);
    clearBit(bitboards[p], square);
    clearBit(bitboards[n], square);
    clearBit(bitboards[b], square);
    clearBit(bitboards[r], square);
    clearBit(bitboards[q], square);
    clearBit(bitboards[k], square);
}

Piece Position::removePiece(Square square){
    assert(square < 64);
    Piece piece = pieceOn(square);
    removeSpecificPiece(piece, square);
    return piece;
}

void Position::printBoard(){
    std::cout << "====================\n";
    for (int i = 0; i < 8; i++) {
        std::cout << 8 - i << "  ";
        for (int j = 0; j < 8; j++) {
            std::cout << charPieces[pieceOn(i * 8 + j)] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n   a b c d e f g h\n\n";
    std::cout << "Side to move: " << (turn ? "Black" : "White") << std::endl;
    std::cout << "En passant: " << squareNames[enPassantSquare] << std::endl;
    std::cout << "Castling rights: ";
    std::cout << (castlingRights & WKCastle ? "K" : "-");
    std::cout << (castlingRights & WQCastle ? "Q" : "-");
    std::cout << (castlingRights & BKCastle ? "k" : "-");
    std::cout << (castlingRights & BQCastle ? "q" : "-");
    std::cout << std::endl;
}

Position::Position() {
    wipe();
    parseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Position::wipe(){
    for (int i = 0; i < 12; i++) {
        bitboards[i] = 0;
    }
    castlingRights = 0;
    enPassantSquare = noSquare;
    turn = WHITE;
}

bool Position::parseFEN(const char *fen) {

    wipe();

    // The FEN string is split into 6 parts:
    // 1. Piece placement
    // 2. Active color
    // 3. Castling availability
    // 4. En passant target square
    // 5. Halfmove clock
    // 6. Fullmove number

    // We will navigate the string using the pointer fen.
    // 1. Piece placement
    Square square = a8;
    int pieceCount = 0;
    while(*fen != ' ') {
        if (*fen == '/') {
            if (pieceCount != 8) {
                std::cout << "Error: Unexpected '/' in FEN string (pieceCount was " << pieceCount << "). Can't parse FEN string" << std::endl;
                return false;
            }
            pieceCount = 0;
            fen++;
            continue;
        }
        if (*fen >= '1' && *fen <= '8') {
            pieceCount += *fen - '0';
            square += *fen - '0';
            fen++;
            continue;
        }
        if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
            Piece piece = pieceFromChar(*fen);
            if (piece == EMPTY) {
                std::cout << "Error: Invalid character in Piece Placement '" << fen[0] << "'. Can't parse FEN string" << std::endl;
                return false;
            }
            fastPlacePiece(piece, square);
            fen++;
            square++;
            pieceCount++;
            continue;
        }
        std::cout << "Error: Invalid character in Piece Placement '" << fen[0] << "'. Can't parse FEN string" << std::endl;
        return false;
    }

    // 2. Active color
    fen++;
    if (*fen == 'w') {
        turn = WHITE;
    } else if (*fen == 'b') {
        turn = BLACK;
    } else {
        std::cout << "Error: Invalid character in Active Color '" << fen[0] << "'. Can't parse FEN string" << std::endl;
        return false;
    }

    // 3. Castling availability
    fen += 2;
    castlingRights = 0;
    if (*fen != '-') {
        while (*fen != ' ') {
            if (*fen == 'K') {
                castlingRights |= WKCastle;
            } else if (*fen == 'Q') {
                castlingRights |= WQCastle;
            } else if (*fen == 'k') {
                castlingRights |= BKCastle;
            } else if (*fen == 'q') {
                castlingRights |= BQCastle;
            } else {
                std::cout << "Error: Invalid character in Castling Availability '" << fen[0] << "'. Can't parse FEN string" << std::endl;
                return false;
            }
            fen++;
        }
    }

    // 4. En passant target square
    fen++;
    if (*fen == '-') {
        enPassantSquare = noSquare;
        fen++;
    } else {
        enPassantSquare = squareFromName(fen);
        fen += 2;
        if (enPassantSquare == noSquare) {
            std::cout << "Error: Invalid character in En Passant Target Square '" << fen[0] << "'. Can't parse FEN string" << std::endl;
            return false;
        }
    }

    // 5. Halfmove clock
    fen++;
    halfMoveClock = 0;
    while (*fen >= '0' && *fen <= '9') {
        halfMoveClock = halfMoveClock * 10 + *fen - '0';
        fen++;
    }

    // 6. Fullmove number
    fen++;
    fullMoveNumber = 0;
    while (*fen >= '0' && *fen <= '9') {
        fullMoveNumber = fullMoveNumber * 10 + *fen - '0';
        fen++;
    }

    if (*fen != '\0') {
        // If there's still data left in the string, it's invalid.
        // Since the position is already set, we won't return false, but we will print a warning.
        std::cout << "Warning: FEN string contains more data than expected : \""<< fen <<"\". Ignoring it.\n";
    }

    return true;
}

bool Position::isSquareAttacked(Square square, Side side){
    // Check if the square is attacked by a piece of the given side.
    // We do this by generating attacks from the square and checking if any of them are occupied by the specific piece of the opposite side.
    BitBoard occupancy = bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5] | 
                         bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
    if (side == WHITE) {
        return 
            // White pawns
            pawnAttacks[BLACK][square] & bitboards[P] +
            // White knights
            knightAttacks[square] & bitboards[N] +
            // White bishops and queens
            getBishopAttack(square, occupancy) & (bitboards[B] | bitboards[Q]) +
            // White rooks and queens
            getRookAttack(square, occupancy) & (bitboards[R] | bitboards[Q]) +
            // White king
            kingAttacks[square] & bitboards[K];
    }
    if (side == BLACK) {
        return 
            // Black pawns
            pawnAttacks[WHITE][square] & bitboards[p] +
            // Black knights
            knightAttacks[square] & bitboards[n] +
            // Black bishops and queens
            getBishopAttack(square, occupancy) & (bitboards[b] | bitboards[q]) +
            // Black rooks and queens
            getRookAttack(square, occupancy) & (bitboards[r] | bitboards[q]) +
            // Black king
            kingAttacks[square] & bitboards[k];
    }
}

// We will use popCount
U8 Position::countSquaresAttacks(Square square, Side side){
    // Count the number of squares attacked by a piece of the given side.
    // We do this by generating attacks from the square and checking if any of them are occupied by the specific piece of the opposite side.
    BitBoard occupancy = bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5] | 
                         bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
    if (side == WHITE) {
        return 
            // White pawns
            popCount(pawnAttacks[BLACK][square] & bitboards[P]) +
            // White knights
            popCount(knightAttacks[square] & bitboards[N]) +
            // White bishops and queens
            popCount(getBishopAttack(square, occupancy) & (bitboards[B] | bitboards[Q])) +
            // White rooks and queens
            popCount(getRookAttack(square, occupancy) & (bitboards[R] | bitboards[Q])) +
            // White king
            popCount(kingAttacks[square] & bitboards[K]);
    }
    if (side == BLACK) {
        return 
            // Black pawns
            popCount(pawnAttacks[WHITE][square] & bitboards[p]) +
            // Black knights
            popCount(knightAttacks[square] & bitboards[n]) +
            // Black bishops and queens
            popCount(getBishopAttack(square, occupancy) & (bitboards[b] | bitboards[q])) +
            // Black rooks and queens
            popCount(getRookAttack(square, occupancy) & (bitboards[r] | bitboards[q])) +
            // Black king
            popCount(kingAttacks[square] & bitboards[k]);
    }
}