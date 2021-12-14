#include "Position.h"
#include "BBmacros.h"
#include "tables.h"
#include "Move.h"
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
    lastMove = 0;
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
    else {
        fen++;
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
            (bool)(pawnAttacks[BLACK][square] & bitboards[P]) +
            // White knights
            (bool)(knightAttacks[square] & bitboards[N]) +
            // White bishops and queens
            (bool)(getBishopAttack(square, occupancy) & (bitboards[B] | bitboards[Q])) +
            // White rooks and queens
            (bool)(getRookAttack(square, occupancy) & (bitboards[R] | bitboards[Q])) +
            // White king
            (bool)(kingAttacks[square] & bitboards[K]);
    }
    if (side == BLACK) {
        return 
            // Black pawns
            (bool)(pawnAttacks[WHITE][square] & bitboards[p]) +
            // Black knights
            (bool)(knightAttacks[square] & bitboards[n]) +
            // Black bishops and queens
            (bool)(getBishopAttack(square, occupancy) & (bitboards[b] | bitboards[q])) +
            // Black rooks and queens
            (bool)(getRookAttack(square, occupancy) & (bitboards[r] | bitboards[q])) +
            // Black king
            (bool)(kingAttacks[square] & bitboards[k]);
    }
    // If side is BOTH, we are in a error state
    std::cout << "Error: Fatal Error. In CountSquaresAttacks, side wasn't WHITE or BLACK. Exiting...\n";
    exit(-1);
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

    // If side is BOTH, we are in a error state
    std::cout << "Error: Fatal Error. In CountSquaresAttacks, side wasn't WHITE or BLACK. Exiting...\n";
    exit(-1);
}

void Position::generateMoves(MoveList *moveList){
    // The move generation needs to set up some variables before it can start generating moves.
    // We will use the following variables:
    // - occupancies[3] : the occupancies of white, black and both sides
    // - us : the side to move
    // - them : the side to move against
    // - pawnCheckers : The BitBoard of squares where a pawn of our side would give check to the opponent
    // - knightCheckers : The BitBoard of squares where a knight of our side would give check to the opponent
    // - bishopCheckers : The BitBoard of squares where a bishop of our side would give check to the opponent
    // - rookCheckers : The BitBoard of squares where a rook of our side would give check to the opponent
    // - queenCheckers : The BitBoard of squares where a queen of our side would give check to the opponent
    // - ourKing : The square of the king of the side to move
    // - theirKing : The square of the king of the side to move against


    BitBoard occupancies[3];
    Side us, them;
    occupancies[WHITE] = bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5];
    occupancies[BLACK] = bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
    occupancies[BOTH] = occupancies[WHITE] | occupancies[BLACK];
    us = turn;
    them = turn ^ 1;

    unsigned long ourKing = 0;
    bitScanForward(&ourKing, bitboards[us * 6 + K]);
    unsigned long theirKing = 0;
    bitScanForward(&theirKing, bitboards[them * 6 + K]);

    BitBoard pawnCheckers = pawnAttacks[them][theirKing];
    BitBoard knightCheckers = knightAttacks[theirKing];
    BitBoard bishopCheckers = getBishopAttack(theirKing, occupancies[BOTH]);
    BitBoard rookCheckers = getRookAttack(theirKing, occupancies[BOTH]);
    BitBoard queenCheckers = bishopCheckers | rookCheckers;

    // Now we can start generating moves. We first need to differentiate between white and black moves.
    if (us == WHITE){
        // Pawn moves
        // Pawn moves are quite tricky to generate. First we try to generate the forward move. Then, if we succeed, we try to generate the double pawn push. We then generate the captures.
        // We also must account for promotions.
        {
            BitBoard pawns = bitboards[P];
            // Iterate over all pawns
            while (pawns){
                // get pawn
                unsigned long pawn;
                bitScanForward(&pawn, pawns);
                // remove pawn from bitboard
                clearBit(pawns, pawn);

                Square to = (Square)(pawn - 8);
                bool isPromotion = (to <= h8);

                // Now we generate captures
                BitBoard captures = pawnAttacks[us][pawn] & occupancies[them];
                while (captures)
                {
                    unsigned long capture;
                    bitScanForward(&capture, captures);
                    clearBit(captures, capture);
                    Piece captured = pieceOn(capture);
                    if (isPromotion)
                    {
                        // We generate all possible promotions
                        addMoveToList(encodeMove(pawn, capture, P, captured, Q, false, false, false, ((squareBitBoard(capture) & queenCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, capture, P, captured, R, false, false, false, ((squareBitBoard(capture) & rookCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, capture, P, captured, B, false, false, false, ((squareBitBoard(capture) & bishopCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, capture, P, captured, N, false, false, false, ((squareBitBoard(capture) & knightCheckers) > 0)), moveList);
                    }
                    else
                    {
                        addMoveToList(encodeMove(pawn, capture, P, captured, EMPTY, false, false, false, ((squareBitBoard(capture) & pawnCheckers) > 0)), moveList);
                    }
                }
                // Now we generate en passant captures
                if (enPassantSquare != noSquare)
                {
                    // we check if the enPassantSquare mask & attack is not empty
                    if (squareBitBoard(enPassantSquare) & pawnAttacks[us][pawn])
                    {
                        addMoveToList(encodeMove(pawn, enPassantSquare, P, p, EMPTY, true, false, false, ((squareBitBoard(enPassantSquare) & pawnCheckers) > 0)), moveList);
                    }
                }

                
                // If square ahead is clear, we can move there
                if (!testBit(occupancies[BOTH], to)){
                    if (isPromotion){
                        // We generate all possible promotions
                        addMoveToList(encodeMove(pawn, to, P, EMPTY, Q, false, false, false, ((squareBitBoard(to) & queenCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, to, P, EMPTY, R, false, false, false, ((squareBitBoard(to) & rookCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, to, P, EMPTY, B, false, false, false, ((squareBitBoard(to) & bishopCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, to, P, EMPTY, N, false, false, false, ((squareBitBoard(to) & knightCheckers) > 0)), moveList);
                    }
                    else{
                        addMoveToList(encodeMove(pawn, to, P, EMPTY, EMPTY, false, false, false, ((squareBitBoard(to) & pawnCheckers) > 0)), moveList);
                        // If we are on the second rank, we can also move two squares forward
                        U8 rank = rankOf(pawn);
                        if (rank == 6){
                            to = (Square)(pawn - 16);
                            if (!testBit(occupancies[BOTH], to)){
                                addMoveToList(encodeMove(pawn, to, P, EMPTY, EMPTY, false, false, true, ((squareBitBoard(to) & pawnCheckers) > 0)), moveList);
                            }
                        }
                    }
                }
            }
        }

        // Knight moves
        // Knight moves are trivially easy to generate, due to its leaper nature
        {
            BitBoard knights = bitboards[N];
            while (knights){
                unsigned long knight;
                bitScanForward(&knight, knights);
                clearBit(knights, knight);
                BitBoard knightCaptures = knightAttacks[knight] & occupancies[them];
                BitBoard knightQuiets = knightAttacks[knight] & ~occupancies[BOTH];

                // Captures
                while (knightCaptures)
                {
                    unsigned long attack;
                    bitScanForward(&attack, knightCaptures);
                    clearBit(knightCaptures, attack);
                    Piece captured = pieceOn(attack);
                    addMoveToList(encodeMove(knight, attack, N, captured, EMPTY, false, false, false, ((squareBitBoard(attack) & knightCheckers) > 0)), moveList);
                }

                // Quiet moves
                while (knightQuiets){
                    unsigned long quiet;
                    bitScanForward(&quiet, knightQuiets);
                    clearBit(knightQuiets, quiet);
                    addMoveToList(encodeMove(knight, quiet, N, EMPTY, EMPTY, false, false, false, ((squareBitBoard(quiet) & knightCheckers) > 0)), moveList);
                }
                
            }
        }

        // Bishop moves
        // Bishop move generation is a tad more difficult, due to it being a slider, but we have magic bitboards, so it won't be as bad
        {
            BitBoard bishops = bitboards[B];
            while (bishops){
                unsigned long bishop;
                bitScanForward(&bishop, bishops);
                clearBit(bishops, bishop);
                BitBoard bishopQuiets = getBishopAttack(bishop, occupancies[BOTH]);
                BitBoard bishopAttacks = bishopQuiets & occupancies[them];
                bishopQuiets &= ~occupancies[BOTH];

                // Captures
                while (bishopAttacks)
                {
                    unsigned long attack;
                    bitScanForward(&attack, bishopAttacks);
                    clearBit(bishopAttacks, attack);
                    Piece captured = pieceOn(attack);
                    addMoveToList(encodeMove(bishop, attack, B, captured, EMPTY, false, false, false, ((squareBitBoard(attack) & bishopCheckers) > 0)), moveList);
                }

                // Quiet moves
                while (bishopQuiets){
                    unsigned long quiet;
                    bitScanForward(&quiet, bishopQuiets);
                    clearBit(bishopQuiets, quiet);
                    addMoveToList(encodeMove(bishop, quiet, B, EMPTY, EMPTY, false, false, false, ((squareBitBoard(quiet) & bishopCheckers) > 0)), moveList);
                }
                
            }
        }

        // Rook moves
        // Rook moves are a bit more complicated, due to being a slider, but we have magic bitboards, so it won't be as bad
        {
            BitBoard rooks = bitboards[R];
            while (rooks){
                unsigned long rook;
                bitScanForward(&rook, rooks);
                clearBit(rooks, rook);
                BitBoard rookQuiets = getRookAttack(rook, occupancies[BOTH]);
                BitBoard rookAttacks = rookQuiets & occupancies[them];
                rookQuiets &= ~occupancies[BOTH];

                // Captures
                while (rookAttacks)
                {
                    unsigned long attack;
                    bitScanForward(&attack, rookAttacks);
                    clearBit(rookAttacks, attack);
                    Piece captured = pieceOn(attack);
                    addMoveToList(encodeMove(rook, attack, R, captured, EMPTY, false, false, false, ((squareBitBoard(attack) & rookCheckers) > 0)), moveList);
                }

                // Quiet moves
                while (rookQuiets){
                    unsigned long quiet;
                    bitScanForward(&quiet, rookQuiets);
                    clearBit(rookQuiets, quiet);
                    addMoveToList(encodeMove(rook, quiet, R, EMPTY, EMPTY, false, false, false, ((squareBitBoard(quiet) & rookCheckers) > 0)), moveList);
                }
                
            }
        }

        // Queen moves
        // Queen moves are a bit more complicated, due to being a slider, but we have magic bitboards, so it won't be as bad
        {
            BitBoard queens = bitboards[Q];
            while (queens){
                unsigned long queen;
                bitScanForward(&queen, queens);
                clearBit(queens, queen);
                BitBoard queenQuiets = getQueenAttack(queen, occupancies[BOTH]);
                BitBoard queenAttacks = queenQuiets & occupancies[them];
                queenQuiets &= ~occupancies[BOTH];

                // Captures
                while (queenAttacks)
                {
                    unsigned long attack;
                    bitScanForward(&attack, queenAttacks);
                    clearBit(queenAttacks, attack);
                    Piece captured = pieceOn(attack);
                    addMoveToList(encodeMove(queen, attack, Q, captured, EMPTY, false, false, false, ((squareBitBoard(attack) & queenCheckers) > 0)), moveList);
                }

                // Quiet moves
                while (queenQuiets){
                    unsigned long quiet;
                    bitScanForward(&quiet, queenQuiets);
                    clearBit(queenQuiets, quiet);
                    addMoveToList(encodeMove(queen, quiet, Q, EMPTY, EMPTY, false, false, false, ((squareBitBoard(quiet) & queenCheckers) > 0)), moveList);
                }
                
            }
        }

        // King moves
        // Normal king moves are very easy to generate, due to its leaper nature, but we also need to generate castling moves
        // We also already have the king position stored in the ourKing variable
        {
            BitBoard kingQuiets = kingAttacks[ourKing] & ~occupancies[BOTH];
            BitBoard kingCaptures = kingAttacks[ourKing] & occupancies[them];

            // Captures
            while (kingCaptures)
            {
                unsigned long capture;
                bitScanForward(&capture, kingCaptures);
                clearBit(kingCaptures, capture);
                Piece captured = pieceOn(capture);
                // A king can't give check, so we don't need to check for check
                addMoveToList(encodeMove(ourKing, capture, K, captured, EMPTY, false, false, false, false), moveList);
            }

            
            
            // Castling moves
            // In order to castle we need to check if the king is in check, and if the Castling flags are set
            bool inCheck = isSquareAttacked(ourKing, them);
            if (!inCheck){
                if (castlingRights & WKCastle){
                    // We need to check if the passing squares are attacked or occupied, in which case we can't castle
                    if (!((squareBitBoard(f1) | squareBitBoard(g1)) & occupancies[BOTH])){
                        if (!isSquareAttacked(f1,them) && !isSquareAttacked(g1,them)){
                            // A castling move can be check if the rook that lands on f1 is on the rookCheckers bitboard
                            addMoveToList(encodeMove(ourKing, g1, K, EMPTY, EMPTY, false, true, false, ((squareBitBoard(f1) & rookCheckers) > 0)), moveList);
                        }
                    }
                }
                if (castlingRights & WQCastle){
                    // We need to check if the passing squares are attacked or occupied, in which case we can't castle
                    if (!((squareBitBoard(d1) | squareBitBoard(c1) | squareBitBoard(b1)) & occupancies[BOTH])){
                        if (!isSquareAttacked(d1,them) && !isSquareAttacked(c1,them)){
                            // A castling move can be check if the rook that lands on d1 is on the rookCheckers bitboard
                            addMoveToList(encodeMove(ourKing, c1, K, EMPTY, EMPTY, false, true, true, ((squareBitBoard(d1) & rookCheckers) > 0)), moveList);
                        }
                    }
                }
            }

            // Quiet moves
            while (kingQuiets)
            {
                unsigned long quiet;
                bitScanForward(&quiet, kingQuiets);
                clearBit(kingQuiets, quiet);
                // A king can't give check, so we don't need to check for check
                addMoveToList(encodeMove(ourKing, quiet, K, EMPTY, EMPTY, false, false, false, false), moveList);
            }
        }
    }
    else{
        // Pawn moves
        // Pawn moves are quite tricky to generate. First we try to generate the forward move. Then, if we succeed, we try to generate the double pawn push. We then generate the captures.
        // We also must account for promotions.
        {
            BitBoard pawns = bitboards[p];
            // Iterate over all pawns
            while (pawns){
                // get pawn
                unsigned long pawn;
                bitScanForward(&pawn, pawns);
                // remove pawn from bitboard
                clearBit(pawns, pawn);
                // Single pawn push
                Square to = (Square)(pawn + 8);
                bool isPromotion = to >= a1;

                // Now we can generate captures
                BitBoard captures = pawnAttacks[us][pawn] & occupancies[them];
                while (captures)
                {
                    unsigned long capture;
                    bitScanForward(&capture, captures);
                    clearBit(captures, capture);
                    Piece captured = pieceOn(capture);
                    if (isPromotion)
                    {
                        // We generate all possible promotions
                        addMoveToList(encodeMove(pawn, capture, p, captured, q, false, false, false, ((squareBitBoard(capture) & queenCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, capture, p, captured, r, false, false, false, ((squareBitBoard(capture) & rookCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, capture, p, captured, b, false, false, false, ((squareBitBoard(capture) & bishopCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, capture, p, captured, n, false, false, false, ((squareBitBoard(capture) & knightCheckers) > 0)), moveList);
                    }
                    else
                    {
                        addMoveToList(encodeMove(pawn, capture, p, captured, EMPTY, false, false, false, ((squareBitBoard(capture) & pawnCheckers) > 0)), moveList);
                    }
                }
                // Now we can generate en passant captures
                if (enPassantSquare != noSquare)
                {
                    // we check if the enPassantSquare mask & attack is not empty
                    if (squareBitBoard(enPassantSquare) & pawnAttacks[us][pawn])
                    {
                        addMoveToList(encodeMove(pawn, enPassantSquare, p, P, EMPTY, true, false, false, ((squareBitBoard(enPassantSquare) & pawnCheckers) > 0)), moveList);
                    }
                }

                // If the square ahead is clear, we can move there
                if(!testBit(occupancies[BOTH], to)){
                    if(isPromotion){
                        // We generate all possible promotions
                        addMoveToList(encodeMove(pawn, to, p, EMPTY, q, false, false, false, ((squareBitBoard(to) & queenCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, to, p, EMPTY, r, false, false, false, ((squareBitBoard(to) & rookCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, to, p, EMPTY, b, false, false, false, ((squareBitBoard(to) & bishopCheckers) > 0)), moveList);
                        addMoveToList(encodeMove(pawn, to, p, EMPTY, n, false, false, false, ((squareBitBoard(to) & knightCheckers) > 0)), moveList);
                    }
                    else{
                        addMoveToList(encodeMove(pawn, to, p, EMPTY, EMPTY, false, false, false, ((squareBitBoard(to) & pawnCheckers) > 0)), moveList);
                        // If we are on the seventh rank, we can generate a double pawn push
                        U8 rank = rankOf(pawn);
                        if (rank == 1){
                            to = (Square)(pawn + 16);
                            if(!testBit(occupancies[BOTH], to)){
                                addMoveToList(encodeMove(pawn, to, p, EMPTY, EMPTY, false, false, true, ((squareBitBoard(to) & pawnCheckers) > 0)), moveList);
                            }
                        }
                    }
                }
            }
        }
        // Knight moves
        // Knight moves are trivially easy to generate, due to its leaper nature
        {
            BitBoard knights = bitboards[n];
            while (knights){
                unsigned long knight;
                bitScanForward(&knight, knights);
                clearBit(knights, knight);
                BitBoard knightCaptures = knightAttacks[knight] & occupancies[them];
                BitBoard knightQuiets = knightAttacks[knight] & ~occupancies[BOTH];

                // Captures
                while (knightCaptures)
                {
                    unsigned long capture;
                    bitScanForward(&capture, knightCaptures);
                    clearBit(knightCaptures, capture);
                    Piece captured = pieceOn(capture);
                    addMoveToList(encodeMove(knight, capture, n, captured, EMPTY, false, false, false, ((squareBitBoard(capture) & knightCheckers) > 0)), moveList);
                }

                // Quiet moves
                while (knightQuiets){
                    unsigned long quiet;
                    bitScanForward(&quiet, knightQuiets);
                    clearBit(knightQuiets, quiet);
                    addMoveToList(encodeMove(knight, quiet, n, EMPTY, EMPTY, false, false, false, ((squareBitBoard(quiet) & knightCheckers) > 0)), moveList);
                }
            }
        }
        // Bishop Moves
        // Bishop move generation is a tad more difficult, due to it being a slider, but we have magic bitboards, so it won't be as bad
        {
            BitBoard bishops = bitboards[b];
            while (bishops){
                unsigned long bishop;
                bitScanForward(&bishop, bishops);
                clearBit(bishops, bishop);
                BitBoard bishopQuiets = getBishopAttack(bishop, occupancies[BOTH]);
                BitBoard bishopCaptures = bishopQuiets & occupancies[them];
                bishopQuiets &= ~occupancies[BOTH];

                // Captures
                while (bishopCaptures)
                {
                    unsigned long capture;
                    bitScanForward(&capture, bishopCaptures);
                    clearBit(bishopCaptures, capture);
                    Piece captured = pieceOn(capture);
                    addMoveToList(encodeMove(bishop, capture, b, captured, EMPTY, false, false, false, ((squareBitBoard(capture) & bishopCheckers) > 0)), moveList);
                }

                // Quiet moves
                while (bishopQuiets){
                    unsigned long quiet;
                    bitScanForward(&quiet, bishopQuiets);
                    clearBit(bishopQuiets, quiet);
                    addMoveToList(encodeMove(bishop, quiet, b, EMPTY, EMPTY, false, false, false, ((squareBitBoard(quiet) & bishopCheckers) > 0)), moveList);
                }
                
            }
        }
        // Rook Moves
        // Rook move generation is a tad more difficult, due to it being a slider, but we have magic bitboards, so it won't be as bad
        {
            BitBoard rooks = bitboards[r];
            while (rooks){
                unsigned long rook;
                bitScanForward(&rook, rooks);
                clearBit(rooks, rook);
                BitBoard rookQuiets = getRookAttack(rook, occupancies[BOTH]);
                BitBoard rookCaptures = rookQuiets & occupancies[them];
                rookQuiets &= ~occupancies[BOTH];

                // Captures
                while (rookCaptures)
                {
                    unsigned long capture;
                    bitScanForward(&capture, rookCaptures);
                    clearBit(rookCaptures, capture);
                    Piece captured = pieceOn(capture);
                    addMoveToList(encodeMove(rook, capture, r, captured, EMPTY, false, false, false, ((squareBitBoard(capture) & rookCheckers) > 0)), moveList);
                }

                // Quiet moves
                while (rookQuiets){
                    unsigned long quiet;
                    bitScanForward(&quiet, rookQuiets);
                    clearBit(rookQuiets, quiet);
                    addMoveToList(encodeMove(rook, quiet, r, EMPTY, EMPTY, false, false, false, ((squareBitBoard(quiet) & rookCheckers) > 0)), moveList);
                }
                
            }
        }
        // Queen Moves
        // Queen move generation is a tad more difficult, due to it being a slider, but we have magic bitboards, so it won't be as bad
        {
            BitBoard queens = bitboards[q];
            while (queens){
                unsigned long queen;
                bitScanForward(&queen, queens);
                clearBit(queens, queen);
                BitBoard queenQuiets = getQueenAttack(queen, occupancies[BOTH]);
                BitBoard queenCaptures = queenQuiets & occupancies[them];
                queenQuiets &= ~occupancies[BOTH];

                // Captures
                while (queenCaptures)
                {
                    unsigned long capture;
                    bitScanForward(&capture, queenCaptures);
                    clearBit(queenCaptures, capture);
                    Piece captured = pieceOn(capture);
                    addMoveToList(encodeMove(queen, capture, q, captured, EMPTY, false, false, false, ((squareBitBoard(capture) & queenCheckers) > 0)), moveList);
                }

                // Quiet moves
                while (queenQuiets){
                    unsigned long quiet;
                    bitScanForward(&quiet, queenQuiets);
                    clearBit(queenQuiets, quiet);
                    addMoveToList(encodeMove(queen, quiet, q, EMPTY, EMPTY, false, false, false, ((squareBitBoard(quiet) & queenCheckers) > 0)), moveList);
                }
                
            }
        }
        // King moves 
        // Normal king moves are very easy to generate, due to its leaper nature, but we also need to generate castling moves
        // We also already have the king position stored in the ourKing variable
        {
            BitBoard kingQuiets = kingAttacks[ourKing] & ~occupancies[BOTH];
            BitBoard kingCaptures = kingAttacks[ourKing] & occupancies[them];

            // Captures
            while (kingCaptures)
            {
                unsigned long capture;
                bitScanForward(&capture, kingCaptures);
                clearBit(kingCaptures, capture);
                Piece captured = pieceOn(capture);
                // A king can't give check so we don't need to check for check
                addMoveToList(encodeMove(ourKing, capture, k, captured, EMPTY, false, false, false, false), moveList);
            }
            // Castling moves
            // In order to castle we need to check if the king is in check, and if the Castling flags are set
            bool inCheck = isSquareAttacked(ourKing, them);
            if (!inCheck)
            {
                if (castlingRights & BKCastle)
                {
                    // We need to check if the passing squares are attacked or occupied, in which case we can't castle
                    if (!((squareBitBoard(f8) | squareBitBoard(g8)) & occupancies[BOTH]))
                    {
                        // We also need to check if the king is in check, and if the rook is in check
                        if (!isSquareAttacked(f8, them) && !isSquareAttacked(g8, them))
                        {
                            // A castling move can be checj if the rook that lands onf8 is on the rookCheckers bitboard
                            addMoveToList(encodeMove(ourKing, g8, k, EMPTY, EMPTY, false, true, false, ((squareBitBoard(f8) & rookCheckers) > 0)), moveList);
                        }
                    }
                }
                if (castlingRights & BQCastle)
                {
                    // We need to check if the passing squares are attacked or occupied, in which case we can't castle
                    if (!((squareBitBoard(d8) | squareBitBoard(c8) | squareBitBoard(b8)) & occupancies[BOTH]))
                    {
                        // We also need to check if the king is in check, and if the rook is in check
                        if (!isSquareAttacked(d8, them) && !isSquareAttacked(c8, them))
                        {
                            // A castling move can be checj if the rook that lands onf8 is on the rookCheckers bitboard
                            addMoveToList(encodeMove(ourKing, c8, k, EMPTY, EMPTY, false, true, false, ((squareBitBoard(d8) & rookCheckers) > 0)), moveList);
                        }
                    }
                }
            }

            // Quiet moves
            while (kingQuiets){
                unsigned long quiet;
                bitScanForward(&quiet, kingQuiets);
                clearBit(kingQuiets, quiet);
                // A king can't give check so we don't need to check for check
                addMoveToList(encodeMove(ourKing, quiet, k, EMPTY, EMPTY, false, false, false, false), moveList);
            }
        }
    }
    // We need to sort the move list
    // We will use std::sort
    std::sort(moveList->moves + 1, moveList->moves + moveList->count, std::greater<ScoredMove>());
}

Move Position::getLegalOf(char * move){
    MoveList moveList;
    generateMoves(&moveList);
    // parse source, target and promotion of the move string
    Square source = move[0] - 'a' + (8 - (move[1] - '0')) * 8;
    Square target = move[2] - 'a' + (8 - (move[3] - '0')) * 8;
    Piece promotion = EMPTY;

    switch (move[4]){
        case 'q':
            promotion = q;
            break;
        case 'r':
            promotion = r;
            break;
        case 'b':
            promotion = b;
            break;
        case 'n':
            promotion = n;
            break;
        case 'Q':
            promotion = Q;
            break;
        case 'R':
            promotion = R;
            break;
        case 'B':
            promotion = B;
            break;
        case 'N':
            promotion = N;
            break;
        default:
            break;
    }
    for (int i = 0; i < moveList.count; i++){
        Move m = moveList.moves[i];
        if ((sourceSquare(m) == source) &&
            (targetSquare(m) == target) &&
            (promotionOf(m) == promotion)){
            return m;
        }
    }
    return 0;
}

