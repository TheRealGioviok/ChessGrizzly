# // The structure of a move is as follows:
# // From: The square the piece is moving from
# // To: The square the piece is moving to
# // Piece: The piece that is moving
# // Captured: The piece that is captured
# // Promotion: The piece that is promoted to
# // EnPassant: Whether or not the move is an en passant
# // Castling: Whether or not the move is a castling move
# // DoublePawnPush: Whether or not the move is a double pawn push
# // Check: Whether or not the move is a check

# /*
#     0000 0000 0000 0000 0000 0000 00xx xxxx - From
#     0000 0000 0000 0000 0000 xxxx xx00 0000 - To
#     0000 0000 0000 0000 xxxx 0000 0000 0000 - Piece
#     0000 0000 0000 xxxx 0000 0000 0000 0000 - Captured
#     0000 0000 xxxx 0000 0000 0000 0000 0000 - Promotion
#     0000 000x 0000 0000 0000 0000 0000 0000 - EnPassant
#     0000 00x0 0000 0000 0000 0000 0000 0000 - Castling
#     0000 0x00 0000 0000 0000 0000 0000 0000 - DoublePawnPush
#     0000 x000 0000 0000 0000 0000 0000 0000 - Check
# */

squareNames = [ "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
                "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
                "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
                "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
                "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
                "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
                "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
                "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1" ]

pieceNames = [ "P", "N", "B", "R", "Q", "K", "p", "n", "b", "r", "q", "k", "noPiece" ]

def unwrapMove(move):
    # unwrap from square
    fromSquare = move & 0x3F
    # unwrap to square
    toSquare = (move >> 6) & 0x3F
    # unwrap piece
    piece = (move >> 12) & 0xF
    # unwrap captured piece
    captured = (move >> 16) & 0xF
    # unwrap promotion
    promotion = (move >> 20) & 0xF
    # unwrap en passant
    enPassant = (move >> 24) & 0x1
    # unwrap castling
    castling = (move >> 25) & 0x1
    # unwrap double pawn push
    doublePawnPush = (move >> 26) & 0x1
    # unwrap check
    check = (move >> 27) & 0x1

    print("From: " + squareNames[fromSquare])
    print("To: " + squareNames[toSquare])
    print("Piece: " + pieceNames[piece] if piece <= 12 else "ERROR PIECE (out of bounds)", piece if piece > 12 else "")
    print("Captured: " + pieceNames[captured] if captured <= 12 else "ERROR CAPTURED (out of bounds)", captured if captured > 12 else "")
    print("Promotion: " + pieceNames[promotion] if promotion <= 12 else "ERROR PROMOTION (out of bounds)", promotion if promotion > 12 else "")
    print("EnPassant: " + "Yes" if enPassant == 1 else "No")
    print("Castling: " + "Yes" if castling == 1 else "No")
    print("DoublePawnPush: " + "Yes" if doublePawnPush == 1 else "No")
    print("Check: " + "Yes" if check == 1 else "No")


# print("8  e_   n_e\n7  vvv  vv\n6          \n5          \n4          \n3         \n2  ltll l l\n1  ol    o\n\n   a b c d e f g h\n\nSide to move: WHITE\nEn passant: noSquare\nCastling rights: KQ--\n")

while True:
    move = int(input("Enter a move: "))
    unwrapMove(move)
