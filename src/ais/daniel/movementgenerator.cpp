#include "position.h"
#include <cassert>

#define PACK_MOVE(i1,j1,i2,j2) (((i1) << 12) + ((j1) << 8) + ((i2) << 4) + j2)

MovementGenerator::MovementGenerator() {
}

void MovementGenerator::addLegalRookMoves(int i, int j) {
    // Check to the left
    int x = j - 1;
    while (x >= 0) {
        if ((board[i][x] == ' ' || isPieceHostile(i,x)) && isKingSafe(i,j,i,x)) {
            int move = (i << 12) + (j << 8) + (i << 4) + x;
            legalMoves->push_back(PACK_MOVE(i,j,i,x));
        }
        if (board[i][x] != ' ') {
            break;
        }
        --x;
    }

    // Check to the right
    x = j + 1;
    while (x < 8) {
        if ((board[i][x] == ' ' || isPieceHostile(i,x)) && isKingSafe(i,j,i,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,i,x));
        }
        if (board[i][x] != ' ') {
            break;
        }
        ++x;
    }

    // Check upwards
    int y = i - 1;
    while (y >= 0) {
        if ((board[y][j] == ' ' || isPieceHostile(y,j)) && isKingSafe(i,j,y,j)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,j));
        }
        if (board[y][j] != ' ') {
            break;
        }
        --y;
    }

    // Check downwards
    y = i + 1;
    while (y < 8) {
        if ((board[y][j] == ' ' || isPieceHostile(y,j)) && isKingSafe(i,j,y,j)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,j));
        }
        if (board[y][j] != ' ') {
            break;
        }
        ++y;
    }
}

void MovementGenerator::addLegalBishopMoves(int i, int j) {
    // Check upwards to the left
    int x = j - 1;
    int y = i - 1;
    while (x >= 0 && y >= 0) {
        if ((board[y][x] == ' ' || isPieceHostile(y,x)) && isKingSafe(i,j,y,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,x));
        }
        if (board[y][x] != ' ') {
            break;
        }
        --x;
        --y;
    }

    // Check upwards to the right
    x = j + 1;
    y = i - 1;
    while (x < 8 && y >= 0) {
        if ((board[y][x] == ' ' || isPieceHostile(y,x)) && isKingSafe(i,j,y,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,x));
        }
        if (board[y][x] != ' ') {
            break;
        }
        ++x;
        --y;
    }

    // Check downwards to the left
    x = j - 1;
    y = i + 1;
    while (x >= 0 && y < 8) {
        if ((board[y][x] == ' ' || isPieceHostile(y,x)) && isKingSafe(i,j,y,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,x));
        }
        if (board[y][x] != ' ') {
            break;
        }
        --x;
        ++y;
    }

    // Check downwards to the right
    x = j + 1;
    y = i + 1;
    while (x < 8 && y < 8) {
        if ((board[y][x] == ' ' || isPieceHostile(y,x)) && isKingSafe(i,j,y,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,x));
        }
        if (board[y][x] != ' ') {
            break;
        }
        ++x;
        ++y;
    }
}

void MovementGenerator::addLegalKnightMoves(int i, int j) {
    int x = j - 2;
    int y = i - 1;
    if (x >= 0 && y >= 0 && (board[y][x] == ' ' || isPieceHostile(y, x)) && isKingSafe(i,j,y,x)) {
        legalMoves->push_back(PACK_MOVE(i,j,y,x));
    }
    y = i + 1;
    if (x >= 0 && y < 8 && (board[y][x] == ' ' || isPieceHostile(y, x)) && isKingSafe(i,j,y,x)) {
        legalMoves->push_back(PACK_MOVE(i,j,y,x));
    }
    x = j + 2;
    if (x < 8 && y < 8 && (board[y][x] == ' ' || isPieceHostile(y, x)) && isKingSafe(i,j,y,x)) {
        legalMoves->push_back(PACK_MOVE(i,j,y,x));
    }
    y = i - 1;
    if (x < 8 && y >= 0 && (board[y][x] == ' ' || isPieceHostile(y, x)) && isKingSafe(i,j,y,x)) {
        legalMoves->push_back(PACK_MOVE(i,j,y,x));
    }
    x = j - 1;
    y = i - 2;
    if (x >= 0 && y >= 0 && (board[y][x] == ' ' || isPieceHostile(y, x)) && isKingSafe(i,j,y,x)) {
        legalMoves->push_back(PACK_MOVE(i,j,y,x));
    }
    x = j + 1;
    if (x < 8 && y >= 0 && (board[y][x] == ' ' || isPieceHostile(y, x)) && isKingSafe(i,j,y,x)) {
        legalMoves->push_back(PACK_MOVE(i,j,y,x));
    }
    y = i + 2;
    if (x < 8 && y < 8 && (board[y][x] == ' ' || isPieceHostile(y, x)) && isKingSafe(i,j,y,x)) {
        legalMoves->push_back(PACK_MOVE(i,j,y,x));
    }
    x = j - 1;
    if (x >= 0 && y >= 0 && (board[y][x] == ' ' || isPieceHostile(y, x)) && isKingSafe(i,j,y,x)) {
        legalMoves->push_back(PACK_MOVE(i,j,y,x));
    }
}

void MovementGenerator::addLegalKingMoves(int i, int j) {
    for (int y = i - 1; y <= i + 1; ++y) {
        for (int x = j - 1; x <= j + 1; ++x) {
            if (!(y == i && x == j)) {
                if (x >= 0 && x < 8 && y >= 0 && y < 8 && (board[y][x] == ' ' || isPieceHostile(y, x)) &&
                    isKingSafe(i,j,y,x)) {
                        legalMoves->push_back(PACK_MOVE(i,j,y,x));
                }
            }
        }
    }
}

void MovementGenerator::addLegalPawnMoves(int i, int j) {
    int x,y;
    // Check whether it's a white or a black pawn
    if (board[i][j] == 'p') {
        y = i - 1;
        // If the square ahead is empty, it's a legal move
        if (board[y][j] == ' ') {
            if (isKingSafe(i,j,y,j)) {
                legalMoves->push_back(PACK_MOVE(i,j,y,j));
            }
            // If the pawn is still in the starting position it's allowed to move 2 squares,
            // provided the square is vacant.
            if (i == 6 && board[i-2][j] == ' ' && isKingSafe(i,j,i-2,j)) {
                legalMoves->push_back(PACK_MOVE(i,j,i-2,j));
            }
        }
    }
    else {
        y = i + 1;
        // If the square ahead is empty, it's a legal move
        if (board[y][j] == ' ') {
            if (isKingSafe(i,j,y,j)) {
                legalMoves->push_back(PACK_MOVE(i,j,y,j));
            }
            // If the pawn is still in the starting position it's allowed to move 2 squares,
            // provided the square is vacant.
            if (i == 1 && board[i+2][j] == ' ' && isKingSafe(i,j,i+2,j)) {
                legalMoves->push_back(PACK_MOVE(i,j,i+2,j));
            }
        }
    }

    // It's also legal to strike diagonally
    x = j - 1;
    if (x >= 0) {
        if (isPieceHostile(y,x) && isKingSafe(i,j,y,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,x));
        }
        // Also test for en passant
        if ((board[i][x] == 'e' || board[i][x] == 'E') && isKingSafe(i,j,y,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,x));
        }
    }
    x = j + 1;
    if (x < 8) {
        if (isPieceHostile(y,x) && isKingSafe(i,j,y,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,x));
        }
        // Also test for en passant
        if ((board[i][x] == 'e' || board[i][x] == 'E') && isKingSafe(i,j,y,x)) {
            legalMoves->push_back(PACK_MOVE(i,j,y,x));
        }
    }
}

void MovementGenerator::addLegalCastlingMoves() {
    if (whiteToMove) {
        if (board[7][4] == 'x' && board[7][7] == 't') {
            if (board[7][5] == ' ' && board[7][6] == ' ') {
                if (isKingSafe(7,4,7,4) && isKingSafe(7,4,7,5) && isKingSafe(7,4,7,6)) {
                    legalMoves->push_back(PACK_MOVE(7,4,7,6));
                }
            }
        }
        if (board[7][4] == 'x' && board[7][0] == 't') {
            if (board[7][3] == ' ' && board[7][2] == ' ' && board[7][1] == ' ') {
                if (isKingSafe(7,4,7,4) && isKingSafe(7,4,7,3) && isKingSafe(7,4,7,2)) {
                    legalMoves->push_back(PACK_MOVE(7,4,7,2));
                }
            }
        }
    }
    else {
        if (board[0][4] == 'X' && board[0][7] == 'T') {
            if (board[0][5] == ' ' && board[0][6] == ' ') {
                if (isKingSafe(0,4,0,4) && isKingSafe(0,4,0,5) && isKingSafe(0,4,0,6)) {
                    legalMoves->push_back(PACK_MOVE(0,4,0,6));
                }
            }
        }
        if (board[0][4] == 'X' && board[0][0] == 'T') {
            if (board[0][3] == ' ' && board[0][2] == ' ' && board[0][1] == ' ') {
                if (isKingSafe(0,4,0,4) && isKingSafe(0,4,0,3) && isKingSafe(0,4,0,2)) {
                    legalMoves->push_back(PACK_MOVE(0,4,0,2));
                }
            }
        }
    }
}

inline bool MovementGenerator::isKingSafe(int i1, int j1, int i2, int j2) {
    bool enPassant = false;
    char memory = board[i2][j2];
    board[i2][j2] = board[i1][j1];
    if (i1 != i2 || j1 != j2) {
        board[i1][j1] = ' ';
    }
    if (board[i2][j2] == 'p' && board[i2 + 1][j2] == 'E') {
        enPassant = true;
        board[i2 + 1][j2] = ' ';
    }
    if (board[i2][j2] == 'P' && board[i2 - 1][j2] == 'e') {
        enPassant = true;
        board[i2 - 1][j2] = ' ';
    }
    // x & y = The coordinates for the king.
    int x,y;
    if (whiteToMove) {
        x = wkx;
        y = wky;
    }
    else {
        x = bkx;
        y = bky;
    }
    // However, we have to check whether the piece being moved is the king...
    if (board[i2][j2] == 'k' || board[i2][j2] == 'K' ||
        board[i2][j2] == 'x' || board[i2][j2] == 'X') {
            x = j2;
            y = i2;
    }

    // Test horizontally left
    int testx = x - 1;
    while (testx >= 0 && board[y][testx] == ' ') {
        --testx;
    }
    if (testx >= 0 && isPieceHostile(y, testx)) {
        // Did we move into the enemy king's attack range?
        if (x - testx == 1 && (board[y][testx] == 'k' || board[y][testx] == 'K' ||
            board[y][testx] == 'x' || board[y][testx] == 'X')) {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
        // Was the king exposed to a rook or a queen?
        if (board[y][testx] == 'q' || board[y][testx] == 'Q' || board[y][testx] == 'r' ||
            board[y][testx] == 'R' || board[y][testx] == 't' || board[y][testx] == 'T') {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
    }

    // Test horizontally right
    testx = x + 1;
    while (testx < 8 && board[y][testx] == ' ') {
        ++testx;
    }
    if (testx < 8 && isPieceHostile(y, testx)) {
        // Did we move into the enemy king's attack range?
        if (testx - x == 1 && (board[y][testx] == 'k' || board[y][testx] == 'K' ||
            board[y][testx] == 'x' || board[y][testx] == 'X')) {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
        // Was the king exposed to a rook or a queen?
        if (board[y][testx] == 'q' || board[y][testx] == 'Q' || board[y][testx] == 'r' ||
            board[y][testx] == 'R' || board[y][testx] == 't' || board[y][testx] == 'T') {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
    }

    // Test vertically up
    int testy = y - 1;
    while (testy >= 0 && board[testy][x] == ' ') {
        --testy;
    }
    if (testy >= 0 && isPieceHostile(testy, x)) {
        // Did we move into the enemy king's attack range?
        if (y - testy == 1 && (board[testy][x] == 'k' || board[testy][x] == 'K' ||
            board[testy][x] == 'x' || board[testy][x] == 'X')) {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
        // Was the king exposed to a rook or a queen?
        if (board[testy][x] == 'q' || board[testy][x] == 'Q' || board[testy][x] == 'r' ||
            board[testy][x] == 'R' || board[testy][x] == 't' || board[testy][x] == 'T') {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
    }

    // Test vertically down
    testy = y + 1;
    while (testy < 8 && board[testy][x] == ' ') {
        ++testy;
    }
    if (testy < 8 && isPieceHostile(testy, x)) {
        // Did we move into the enemy king's attack range?
        if (testy - y == 1 && (board[testy][x] == 'k' || board[testy][x] == 'K' ||
            board[testy][x] == 'x' || board[testy][x] == 'X')) {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
        // Was the king exposed to a rook or a queen?
        if (board[testy][x] == 'q' || board[testy][x] == 'Q' || board[testy][x] == 'r' ||
            board[testy][x] == 'R' || board[testy][x] == 't' || board[testy][x] == 'T') {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
    }

    // Test up and left
    testx = x - 1;
    testy = y - 1;
    while (testx >= 0 && testy >= 0 && board[testy][testx] == ' ') {
        --testx;
        --testy;
    }
    if (testx >= 0 && testy >= 0 && isPieceHostile(testy, testx)) {
        // Test for enemy king or pawn. (Pawn is only an issue for the white player.)
        if (x - testx == 1) {
            if (board[testy][testx] == 'k' || board[testy][testx] == 'K' ||
                board[testy][testx] == 'x' || board[testy][testx] == 'X' ||
                board[testy][testx] == 'P' || board[testy][testx] == 'E') {
                    restore(i1,j1,i2,j2,memory);
                    return false;
            }
        }
        // Test for enemy queen or bishop
        if (board[testy][testx] == 'q' || board[testy][testx] == 'Q' ||
            board[testy][testx] == 'b' || board[testy][testx] == 'B') {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
    }

    // Test up and right
    testx = x + 1;
    testy = y - 1;
    while (testx < 8 && testy >= 0 && board[testy][testx] == ' ') {
        ++testx;
        --testy;
    }
    if (testx < 8 && testy >= 0 && isPieceHostile(testy, testx)) {
        // Test for enemy king or pawn. (Pawn is only an issue for the white player.)
        if (testx - x == 1) {
            if (board[testy][testx] == 'k' || board[testy][testx] == 'K' ||
                board[testy][testx] == 'x' || board[testy][testx] == 'X' ||
                board[testy][testx] == 'P' || board[testy][testx] == 'E') {
                    restore(i1,j1,i2,j2,memory);
                    return false;
            }
        }
        // Test for enemy queen or bishop
        if (board[testy][testx] == 'q' || board[testy][testx] == 'Q' ||
            board[testy][testx] == 'b' || board[testy][testx] == 'B') {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
    }

    // Test down and left
    testx = x - 1;
    testy = y + 1;
    while (testx >= 0 && testy < 8 && board[testy][testx] == ' ') {
        --testx;
        ++testy;
    }
    if (testx >= 0 && testy < 8 && isPieceHostile(testy, testx)) {
        // Test for enemy king or pawn. (Pawn is only an issue for the black player.)
        if (x - testx == 1) {
            if (board[testy][testx] == 'k' || board[testy][testx] == 'K' ||
                board[testy][testx] == 'x' || board[testy][testx] == 'X' ||
                board[testy][testx] == 'p' || board[testy][testx] == 'e') {
                    restore(i1,j1,i2,j2,memory);
                    return false;
            }
        }
        // Test for enemy queen or bishop
        if (board[testy][testx] == 'q' || board[testy][testx] == 'Q' ||
            board[testy][testx] == 'b' || board[testy][testx] == 'B') {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
    }

    // Test down and right
    testx = x + 1;
    testy = y + 1;
    while (testx < 8 && testy < 8 && board[testy][testx] == ' ') {
        ++testx;
        ++testy;
    }
    if (testx < 8 && testy < 8 && isPieceHostile(testy, testx)) {
        // Test for enemy king or pawn. (Pawn is only an issue for the black player.)
        if (testx - x == 1) {
            if (board[testy][testx] == 'k' || board[testy][testx] == 'K' ||
                board[testy][testx] == 'x' || board[testy][testx] == 'X' ||
                board[testy][testx] == 'p' || board[testy][testx] == 'e') {
                    restore(i1,j1,i2,j2,memory);
                    return false;
            }
        }
        // Test for enemy queen or bishop
        if (board[testy][testx] == 'q' || board[testy][testx] == 'Q' ||
            board[testy][testx] == 'b' || board[testy][testx] == 'B') {
                restore(i1,j1,i2,j2,memory);
                return false;
        }
    }

    // Test for enemy knights
    testx = x - 2;
    testy = y - 1;
    if (testx >= 0 && testy >= 0 &&
        (board[testy][testx] == 'n' || board[testy][testx] == 'N') &&
        isPieceHostile(testy, testx)) {
            return false;
    }
    testy = y + 1;
    if (testx >= 0 && testy < 8 &&
        (board[testy][testx] == 'n' || board[testy][testx] == 'N') &&
        isPieceHostile(testy, testx)) {
            restore(i1,j1,i2,j2,memory);
            return false;
    }
    testx = x + 2;
    if (testx < 8 && testy < 8 &&
        (board[testy][testx] == 'n' || board[testy][testx] == 'N') &&
        isPieceHostile(testy, testx)) {
            restore(i1,j1,i2,j2,memory);
            return false;
    }
    testy = y - 1;
    if (testx < 8 && testy >= 0 &&
        (board[testy][testx] == 'n' || board[testy][testx] == 'N') &&
        isPieceHostile(testy, testx)) {
            restore(i1,j1,i2,j2,memory);
            return false;
    }
    testx = x - 1;
    testy = y - 2;
    if (testx >= 0 && testy >= 0 &&
        (board[testy][testx] == 'n' || board[testy][testx] == 'N') &&
        isPieceHostile(testy, testx)) {
            restore(i1,j1,i2,j2,memory);
            return false;
    }
    testx = x + 1;
    if (testx < 8 && testy >= 0 &&
        (board[testy][testx] == 'n' || board[testy][testx] == 'N') &&
        isPieceHostile(testy, testx)) {
            restore(i1,j1,i2,j2,memory);
            return false;
    }
    testy = y + 2;
    if (testx < 8 && testy < 8 &&
        (board[testy][testx] == 'n' || board[testy][testx] == 'N') &&
        isPieceHostile(testy, testx)) {
            restore(i1,j1,i2,j2,memory);
            return false;
    }
    testx = x - 1;
    if (testx >= 0 && testy < 8 &&
        (board[testy][testx] == 'n' || board[testy][testx] == 'N') &&
        isPieceHostile(testy, testx)) {
            restore(i1,j1,i2,j2,memory);
            return false;
    }
    if (enPassant) {
        if (board[i2][j2] == 'p') {
            board[i2 + 1][j2] = 'E';
        }
        else {
            board[i2 - 1][j2] = 'e';
        }
    }
    restore(i1,j1,i2,j2,memory);
    return true;
}

bool MovementGenerator::isPieceHostile(int i, int j) {
    if (whiteToMove && board[i][j] > 'A' && board[i][j] < 'Z') {
        return true;
    }
    if (!whiteToMove && board[i][j] > 'a') {
        return true;
    }
    return false;
}

// Restore board to pre-move state
void MovementGenerator::restore(int i1, int j1, int i2, int j2, char memory) {
    board[i1][j1] = board[i2][j2];
    board[i2][j2] = memory;
}

std::string MovementGenerator::convertToStringCoordinates(int i1, int j1, int i2, int j2) {
    assert(false && "shouldn't be called");
    std::string str;
    str.push_back(j1 + 'A');
    str.push_back(8 - i1 + '0');
    str.push_back('-');
    str.push_back(j2 + 'A');
    str.push_back(8 - i2 + '0');
    return str;
}

void MovementGenerator::setPosition(Position *p) {
    p->getBoard(&board[0], &wkx, &wky, &bkx, &bky);
    whiteToMove = p->isWhiteToMove();
}

void MovementGenerator::getAllLegalMoves(std::vector<int> *lm) {
    legalMoves = lm;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] > 'A' && !isPieceHostile(i,j)) {
                switch (board[i][j]) {
                case 'r':
                case 'R':
                case 't':
                case 'T':
                    addLegalRookMoves(i, j);
                    break;
                case 'b':
                case 'B':
                    addLegalBishopMoves(i, j);
                    break;
                case 'q':
                case 'Q':
                    addLegalRookMoves(i, j);
                    addLegalBishopMoves(i, j);
                    break;
                case 'n':
                case 'N':
                    addLegalKnightMoves(i, j);
                    break;
                case 'k':
                case 'K':
                case 'x':
                case 'X':
                    addLegalKingMoves(i, j);
                    break;
                case 'p':
                case 'P':
                    addLegalPawnMoves(i, j);
                    break;
                }
            }
        }
    }
    addLegalCastlingMoves();
}
