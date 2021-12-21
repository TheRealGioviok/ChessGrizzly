#pragma once

#include "Game.h"
#include "uci.h"

U32 mainHistory[maxPly][12][64] = {{{0}}};
Move counterMoves[2][64][64] = {{{0}}};
Move killerMoves[2][maxPly] = {{0}};

U8 pvLenght[maxPly] = {0};
Move pvTable[maxPly][maxPly] = {{0}};

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
        if (captures) {
            pos.removeSpecificPiece(captured, to);
            // Update the hash key by removing the capture
            pos.hashKey ^= pieceKeys[captured][to];
        }
        pos.fastPlacePiece(piece, to);
        pos.removeSpecificPiece(piece, from);
        // Update the hash key by removing the piece from the from square
        pos.hashKey ^= pieceKeys[piece][from];
        // Update the hash key by placing the piece on the to square
        pos.hashKey ^= pieceKeys[piece][to];



        // If the move is a promotion, replace the piece with the promotion
        if (promotes) {
            pos.removeSpecificPiece(piece, to);
            pos.fastPlacePiece(promotion, to);
            // Update the hash key by removing the piece from the to square
            pos.hashKey ^= pieceKeys[piece][to];
            // Update the hash key by placing the promoted piece on the to square
            pos.hashKey ^= pieceKeys[promotion][to];
        }

        // If the move is a castling move, move the rook
        if (castling) {
            // The castle move depends on the square the king is moving to
            switch (to) {
                case c1:
                    pos.fastPlacePiece(R, d1);
                    pos.removeSpecificPiece(R, a1);
                    // Update the hash key by removing the rook from the a1 square
                    pos.hashKey ^= pieceKeys[R][a1];
                    // Update the hash key by placing the rook on the d1 square
                    pos.hashKey ^= pieceKeys[R][d1];
                    break;
                case g1:
                    pos.fastPlacePiece(R, f1);
                    pos.removeSpecificPiece(R, h1);
                    // Update the hash key by removing the rook from the h1 square
                    pos.hashKey ^= pieceKeys[R][h1];
                    // Update the hash key by placing the rook on the f1 square
                    pos.hashKey ^= pieceKeys[R][f1];
                    break;
                case c8:
                    pos.fastPlacePiece(r, d8);
                    pos.removeSpecificPiece(r, a8);
                    // Update the hash key by removing the rook from the a8 square
                    pos.hashKey ^= pieceKeys[r][a8];
                    // Update the hash key by placing the rook on the d8 square
                    pos.hashKey ^= pieceKeys[r][d8];
                    break;
                case g8:
                    pos.fastPlacePiece(r, f8);
                    pos.removeSpecificPiece(r, h8);
                    // Update the hash key by removing the rook from the h8 square
                    pos.hashKey ^= pieceKeys[r][h8];
                    // Update the hash key by placing the rook on the f8 square
                    pos.hashKey ^= pieceKeys[r][f8];
                    break;
                default:{
                    std::cout << "Fatal error in move execution: can't castle to "<< squareNames[to] << std::endl;
                    exit(-1);
                }
            }
        }

        // Update the hash key by removing castling rights
        pos.hashKey ^= castlingKeys[pos.castlingRights];

        // Update castling rights
        pos.castlingRights &= castlingRightsMask[from];
        pos.castlingRights &= castlingRightsMask[to];

        // Update the hash key by adding back castling rights
        pos.hashKey ^= castlingKeys[pos.castlingRights];

        // If the move is an en passant capture, remove the captured pawn
        if (enpassant) {
            pos.removeSpecificPiece(captured, pos.enPassantSquare + 8 * (1 - 2 * pos.turn));
            // Update the hash key by removing the captured pawn
            pos.hashKey ^= pieceKeys[captured][pos.enPassantSquare + 8 * (1 - 2 * pos.turn)];
            // Update the hash key by re-adding the ghost pawn (that we removed when handling normal captures)
            pos.hashKey ^= pieceKeys[captured][to];
        }

        // Update the hash key by removing the en passant square
        pos.hashKey ^= enPassantKeys[pos.enPassantSquare];

        // If the move is a double pawn push, set the en passant square
        if (doublePush) {
            pos.enPassantSquare = to + 8 * (1 - 2*pos.turn);
        }
        else{
            // reset en passant square
            pos.enPassantSquare = noSquare;
        }

        // Update the hash key by adding the en passant square
        pos.hashKey ^= enPassantKeys[pos.enPassantSquare];

        // Now we check if the move is legal
        unsigned long ourKing;
        bitScanForward(&ourKing, pos.bitboards[K + pos.turn * 6]);

        // switch turn
        pos.turn ^= 1;

        // Update the hash key by adding the turn
        pos.hashKey ^= sideKey;

        // If we are in check after the move, the move is illegal
        if (pos.isSquareAttacked(ourKing, pos.turn)){
            return false;
        }

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
            // For debugging purposes, we check the incrementally updated hash key against a newly generated one
            HashKey HashKey = pos.hashKey;
            pos.generateHashKey();
            if (HashKey != pos.hashKey)
            {
                std::cout << "Hash key error" << std::endl;
                std::cout << "Move: " << moveList->moves[i] << std::endl;
                print();
                exit(-1);
            }
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

void Game::makeNullMove(){
    pos.turn ^= 1;
    pos.enPassantSquare = noSquare;
    pos.lastMove = 0;

}

#define ASPIRATION_WINDOW_SIZE 50
void Game::startSearch(){
    // std::cout << "Starting search:\nDepth limit " << (int)depth << "\nWhite time " << ((int)wtime)/1000 << " + " << ((int)winc)/1000 << "\nBlack time " << ((int)btime)/1000 << " + " << ((int)binc)/1000 << std::endl;
    
    nodes = 0ULL;
    stopped = false;
    // clear mainHistory, killerMoves, countermoves
    memset(mainHistory, 0, sizeof(mainHistory));
    memset(killerMoves, 0, sizeof(killerMoves));
    memset(counterMoves, 0, sizeof(counterMoves));

    // clear pvLen and pvTable
    memset(pvLenght, 0, sizeof(pvLenght));
    memset(pvTable, 0, sizeof(pvTable));

    std::cin.clear();
    std::cout << std::endl;

    pos.lastMove = 0;
    
    U64 startTime = getTime64();

    if (moveTime == 0xFFFFFFFFFFFFFFFD) {
        // if moveTime is set to -2, we are playing with time control
        // we need to calculate the time for the next move

        moveTime = getTime64();
        if (pos.turn == WHITE) {
            moveTime += (U64)(wtime / 25) + winc;
        }
        else {
            moveTime += (U64)(btime / 25) + binc;
        }
        depth = 64;
    }

    stopped = false;

    // Iterative deepening with aspiration windows
    Score score = negaMax(-infinity, +infinity, 1);
    std::cout << "info score depth 1 cp " << score << " nodes " << nodes << " moves ";
    printMove(pvTable[0][0]);
    std::cout << std::endl;

    stopped = false;

    for (Depth d = 2; d <= depth; d++){
        if (stopped) break;
        nodes = 0ULL;
        U64 timer = getTime64();
        if (!stopped)
            score = negaMax(score - ASPIRATION_WINDOW_SIZE, score + ASPIRATION_WINDOW_SIZE, d);
        if (!stopped && (score < score - ASPIRATION_WINDOW_SIZE) || (score > score + ASPIRATION_WINDOW_SIZE)){
            std::cout << "Research at full size for depth " << d << std::endl;
            if (!stopped)
                score = negaMax(-infinity, infinity, d);
        }

        if (!stopped){
            bestMove = pvTable[0][0];
            U64 timer2 = getTime64();
            if (score < -mateScore && score > -mateValue){
                std::cout << "info score mate " << -(score + mateValue) / 2 - 1 << " depth " << (int)d << " nodes " << nodes << " pv ";
            }
            else if (score > mateScore && score < mateValue){
                std::cout << "info score mate " << (score - mateValue) / 2 + 1 << " depth " << (int)d << " nodes " << nodes << " pv ";
            }
            else{
                std::cout << "info score cp " << score << " depth " << (int)d << " nodes " << nodes << " pv ";
            }

            for (int i = 0; i<pvLenght[0]; i++){
                printMove(pvTable[0][i]);
                std::cout << " ";
            }

            std::cout << " speed " << (nodes / (timer2 - timer + 1)) << "kN/S" << std::endl;
        }

        
    }

    std::cout << "bestmove ";
    printMove(bestMove);
    std::cout << std::endl;
}

inline Score Game::negaMax(Score alpha, Score beta, Depth depth){

    // init pvLenght
    pvLenght[ply] = ply;

    // If we have reached the depth limit, return the heuristic value
    if (depth == 0){
        return quiescence(alpha, beta);
    }

    if (depth >= maxPly){
        return pestoEval(&pos);
    }

    // communicate every 2048 nodes
    if ((nodes & 2047) == 0){
        communicate(this);
        // if stopped, return 0 (value won't be used)
        if (stopped) return 0;
    }
    // Increment the number of nodes
    nodes++;

    // Are we in check?
    unsigned long kingSquare;
    bitScanForward(&kingSquare, pos.bitboards[K + pos.turn * 6]);
    bool inCheck = pos.isSquareAttacked(kingSquare, pos.turn ^ 1);

    Position save = pos;
    bool pvNode = (beta - alpha) > 1;

    // Null move pruning
    if (ply && !pvNode && depth > 3 && !inCheck && pos.lastMove != 0 && !pos.mayBeZugzwang()) {
        ++ply;
        makeNullMove();
        Score score = -negaMax(-beta, -alpha, depth - 3);
        --ply;
        pos = save;
        if (score >= beta){
            return beta;
        }
    }

    // Generate all the moves
    MoveList *moveList = new MoveList();
    generateMoves(moveList);

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
        Score score;
        // PVS search: we search the first move at full window, then the others at null window. If the search fails, then we research at full window
        if (!legalMoves || !ply){
            score = -negaMax(-beta, -alpha, depth - 1);
        }
        else{
            score = -negaMax(-alpha - 1, -alpha, depth - 1 - (bool)okToReduce(moveList->moves[count]) * reductionTable[depth][legalMoves]);
            if (score > alpha && score < beta){
                score = -negaMax(-beta, -alpha, depth - 1);
            }
        }
        --ply;
        pos = save;

        if (score > alpha){
            alpha = score;

            if (pvNode){
                // write PV move to pvTable
                pvTable[ply][ply] = move;

                // copy moves from deeper ply to current ply's line

                for (int i = ply + 1; i < pvLenght[ply + 1]; i++){
                    pvTable[ply][i] = pvTable[ply + 1][i];
                }

                // adjust pvLen 
                pvLenght[ply] = pvLenght[ply + 1];
            }

            
            if (score >= beta) {
                if (pieceCaptured(move) == EMPTY) {
                    // Update killer moves
                    killerMoves[0][depth] = killerMoves[1][depth];
                    killerMoves[1][depth] = move;

                    // Update countermoves
                    counterMoves[pos.turn][sourceSquare(pos.lastMove)][targetSquare(pos.lastMove)] = move;
                }
                delete moveList;
                return beta;
            }

            // history update
            if (pieceCaptured(move) == EMPTY)
                mainHistory[ply][pieceMoved(move)][targetSquare(move)] += depth * depth;
        }
    }
    
    pos = save;
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

    // communicate every 2048 nodes
    if ((nodes & 2047) == 0){
        communicate(this);
        // if stopped, return 0 (value won't be used)
        if (stopped) return 0;
    }

    // Are we in check?
    unsigned long kingSquare;
    bitScanForward(&kingSquare, pos.bitboards[K + pos.turn * 6]);
    bool inCheck = pos.isSquareAttacked(kingSquare, pos.turn ^ 1);

    Score eval = pestoEval(&pos);
    if (eval >= beta)
        return beta;
    if (eval > alpha)
        alpha = eval;
    if (ply >= maxPly)
        return eval;

#define BIGDELTA 775

    if (!inCheck && (eval < alpha - BIGDELTA * (1 + ( promotionOf(pos.lastMove) != EMPTY))))
        return alpha;

    // Generate all the moves
    MoveList* moveList = new MoveList();
    inCheck ? generateMoves(moveList) : pos.generateCaptures(moveList);

    // Loop through all the moves
    Position save = pos;

    for (int i = 1; i < moveList->count; i++){
        Move move = onlyMove(moveList->moves[i]);
        if (makeMove(move)){
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
        else {
            pos = save;
        }
    }

    delete moveList;
    return alpha;
}