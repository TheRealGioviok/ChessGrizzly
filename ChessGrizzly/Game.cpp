#pragma once

#include "Game.h"

U32 mainHistory[maxPly][12][64] = {{{0}}};
Move counterMoves[2][64][64] = {{{0}}};
Move killerMoves[2][maxPly] = {{0}};
U16 ply = 0;

bool Game::makeMove(Move move){
    // If the move is a null move, we return false
    if (move){
        Piece piece = pieceMoved(move);
        Square from = sourceSquare(move);
        Square to = targetSquare(move);
        Piece captured = pieceCaptured(move);
        bool captures = captured != EMPTY;
        Piece promotion = promotionOf(move);
        bool promotes = (promotion != EMPTY);
        bool enpassant = isEnPassantMove(move);
        bool castling = isCastlingMove(move);
        bool check = isCheckMove(move);
        bool doublePush = isDoublePawnPush(move);
        // TODO: implement position stack and save current position to stack

        // Move the piece
        // If the move is a capture, remove the captured piece
        if (captures) pos.removeSpecificPiece(captured, to);
        pos.fastPlacePiece(piece, to);
        pos.removeSpecificPiece(piece, from);

        

        // If the move is a promotion, replace the piece with the promotion
        if (promotes) {
            pos.removeSpecificPiece(piece, to);
            pos.fastPlacePiece(promotion, to);
        }

        // If the move is a castling move, move the rook
        if (castling) {
            // The castle move depends on the square the king is moving to
            switch (to) {
                case c1:
                    pos.fastPlacePiece(R, d1);
                    pos.removeSpecificPiece(R, a1);
                    break;
                case g1:
                    pos.fastPlacePiece(R, f1);
                    pos.removeSpecificPiece(R, h1);
                    break;
                case c8:
                    pos.fastPlacePiece(r, d8);
                    pos.removeSpecificPiece(r, a8);
                    break;
                case g8:
                    pos.fastPlacePiece(r, f8);
                    pos.removeSpecificPiece(r, h8);
                    break;
                default:{
                    std::cout << "Fatal error in move execution: can't castle to "<< squareNames[to] << std::endl;
                    exit(-1);
                }
            }
        }

        // Update castling rights
        pos.castlingRights &= castlingRightsMask[from];
        pos.castlingRights &= castlingRightsMask[to];

        // If the move is an en passant capture, remove the captured pawn
        if (enpassant) {
            pos.removeSpecificPiece(captured, pos.enPassantSquare + 8 * (1 - 2 * pos.turn));
        }

        // If the move is a double pawn push, set the en passant square
        if (doublePush) {
            pos.enPassantSquare = to + 8 * (1 - 2*pos.turn);
        }
        else{
            // reset en passant square
            pos.enPassantSquare = noSquare;
        }

        // Now we check if the move is legal
        unsigned long ourKing;
        bitScanForward(&ourKing, pos.bitboards[K + pos.turn * 6]);

        // switch turn
        pos.turn ^= 1;

        // If we are in check after the move, the move is illegal
        if (pos.isSquareAttacked(ourKing, pos.turn)){
            return false;
        }
        pos.lastMove = onlyMove(move);
        return true;
    }
    return false;
}

void Game::generateMoves(MoveList *ml){
    pos.generateMoves(ml);
}

// The perft test function
// This function is used to test the correctness of the move generation code
// It is not used in the actual game, but it is useful for testing
U64 Game::perft(int depth){
    nodes = 0ULL;
    std::cout << "Perft test for depth " << depth << std::endl;
    std::cout << "Number of moves: " << _perft(depth) << std::endl;
    std::cout << "(2) Number of moves: " << nodes << std::endl;
    return nodes;
}

U64 Game::_perft(int depth)
{
    if (depth == 0)
    {
        //print();
        //getchar();
        nodes++;
        return 1ULL;
    }
    U64 total = 0ULL;
    MoveList *moveList = new MoveList();
    generateMoves(moveList);
    Position save = pos;
    for (int i = 1; i < moveList->count; i++)
    {
        if (makeMove(moveList->moves[i]))
        {
            total += _perft(depth - 1);
        }
        pos = save;
    }
    pos = save;
    delete moveList;
    return total;
}

U64 Game::divide(int depth){
    nodes = 0ULL;
    std::cout << "Divide test for depth " << depth << std::endl;
    // iterate through all moves
    MoveList* moveList = new MoveList();
    generateMoves(moveList);
    Position save = pos;
    for (int i = 1; i < moveList->count; i++){
        if (makeMove(moveList->moves[i])){
            U64 localNodes = _perft(depth - 1);
            std::cout << "Move ";
            printMove(moveList->moves[i]);
            std::cout << ": " << localNodes << std::endl;
        }
        pos = save;
    }
    pos = save;
    delete moveList;
    std::cout << "Total nodes: " << nodes << std::endl;
    return nodes;
}



void Game::print(){
    pos.printBoard();
}

bool Game::parseFEN(const char *fen){
    return pos.parseFEN(fen);
}

Game::Game(){
    pos = Position();
    nodes = 0ULL;
}

Game::Game(const char *fen){
    pos = Position();
    nodes = 0ULL;
    parseFEN(fen);
}

Move Game::getLegalOf(char * move){
    return pos.getLegalOf(move);
}

bool Game::executeMoveList(char *ml){
    while (*ml){
        while (*ml == ' ') ml++;
        Move move = getLegalOf(ml);
        if (!move){
            return false;
        }
        if (!makeMove(move)){
            return false;
        }
        // since the move size can be either 4 or 5, we don't know if we have to skip the next character
        if (ml[4] == ' '){
            ml += 4;
        }
        else{
            ml += 5;
        }
    }
}

void Game::reset(){
    pos = Position();
    nodes = 0ULL;
    ply = 0;
    // Add stuff here when implementing heuristics
}

#define ASPIRATION_WINDOW_SIZE 30
void Game::startSearch(){
    std::cout << "Starting search:\nDepth limit " << (int)depth << "\nWhite time " << ((int)wtime)/1000 << " + " << ((int)winc)/1000 << "\nBlack time " << ((int)btime)/1000 << " + " << ((int)binc)/1000 << std::endl;
    
    nodes = 0ULL;
    // clear mainHistory, killerMoves, countermoves
    memset(mainHistory, 0, sizeof(mainHistory));
    memset(killerMoves, 0, sizeof(killerMoves));
    memset(counterMoves, 0, sizeof(counterMoves));

    pos.lastMove = 0;
    
    // Iterative deepening with aspiration windows
    Score score = negaMax(-infinity, +infinity, 1);
    std::cout << "info score depth 1 cp " << score << " nodes " << nodes << " moves ";
    printMove(bestMove);
    std::cout << std::endl;

    for (Depth d = 2; d <= depth; d++){
        nodes = 0ULL;
        score = negaMax(score - ASPIRATION_WINDOW_SIZE, score + ASPIRATION_WINDOW_SIZE, d);
        if ((score < score - ASPIRATION_WINDOW_SIZE) || (score > score + ASPIRATION_WINDOW_SIZE)){
            std::cout << "Research at full size for depth " << d << std::endl;
            score = negaMax(-infinity, infinity, d);
        }
        std::cout << "info score depth " << (int)d << " cp " << score << " nodes " << nodes << " moves ";
        printMove(bestMove);
        std::cout << std::endl;
    }

    std::cout << "bestmove ";
    printMove(bestMove);
    std::cout << std::endl;
}

Score Game::negaMax(Score alpha, Score beta, Depth depth){

    // If we have reached the depth limit, return the heuristic value
    if (depth == 0){
        return quiescence(alpha, beta);
    }

    if (depth >= maxPly){
        return pestoEval(&pos);
    }

    // Increment the number of nodes
    nodes++;

    // Are we in check?
    unsigned long kingSquare;
    bitScanForward(&kingSquare, pos.bitboards[K + pos.turn * 6]);
    bool inCheck = pos.isSquareAttacked(kingSquare, pos.turn ^ 1);

    // Generate all the moves
    MoveList *moveList = new MoveList();
    generateMoves(moveList);

    Position save = pos;
    int legalMoves = 0;

    for (int count = 0; count < moveList->count; count++){

        // The move
        Move move = onlyMove(moveList->moves[count]);

        ply++;
        // Make the move
        if (!makeMove(moveList->moves[count])){
            --ply;
            pos = save;
            continue;
        }

        legalMoves++;
        Score score = -negaMax(-beta, -alpha, depth - 1);
        --ply;
        pos = save;

        if (score > alpha){
            alpha = score;
            // history update
            if (pieceCaptured(move) == EMPTY)
                mainHistory[ply][pieceMoved(move)][targetSquare(move)] = depth * depth;
            if (score >= beta) {
                if (pieceCaptured(move) == EMPTY) {
                    // Update killer moves
                    killerMoves[0][depth] = killerMoves[1][depth];
                    killerMoves[1][depth] = move;

                    // Update countermoves
                    counterMoves[pos.turn][sourceSquare(move)][targetSquare(move)] = move;
                }
                delete moveList;
                return beta;
            }
            if (!ply)
            {
                // Update the best move
                bestMove = move;
            }
        }
    }
    
    delete moveList;
    
    if (!legalMoves){
        if (inCheck){
            return -mateValue + ply;
        }
        else{
            return 0;
        }
    }

    return alpha;
}

Score Game::quiescence(Score alpha, Score beta){

    Score eval = pestoEval(&pos);
    if (eval >= beta)
        return beta;
    if (eval > alpha)
        alpha = eval;
    if (ply >= maxPly)
        return eval;

    // Generate all the moves
    MoveList *moveList = new MoveList();
    generateMoves(moveList);

    // Loop through all the moves
    Position save = pos;

    // Are we in check?
    unsigned long kingSquare;
    bitScanForward(&kingSquare, pos.bitboards[K + pos.turn * 6]);
    bool inCheck = pos.isSquareAttacked(kingSquare, pos.turn ^ 1);

    for (int i = 1; i < moveList->count; i++){
        Move move = onlyMove(moveList->moves[i]);
        if (((pieceCaptured(move) != EMPTY)) && makeMove(move)){
            ++ply;
            Score score = -quiescence(-beta, -alpha);
            --ply;
            pos = save;
            if (score >= beta){
                delete moveList;
                return beta;
            }
            if (score > alpha){
                alpha = score;
            }
        }
    }

    delete moveList;
    return alpha;
}