#include "Position.h"


Position::Position(MovementGenerator *mg) : generator(mg), gameOver(false), isCheck(false) {
    reset();
    mg->setPosition(this);
    mg->getAllLegalMoves(&legalMoves);
}

Position::Position(Position *p, int mv) {
    int x;
    p->getBoard(&board[0], &x, &x, &x, &x);
    whiteToMove = p->isWhiteToMove();
    generator = p->getMovementGenerator();
    move(p->getLegalMoves()->at(mv),0);
    gameOver = testLeafNode();
}

Position::Position(MovementGenerator *mg, char b[8][8], bool white) : generator(mg), whiteToMove(white) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = b[i][j];
        }
    }
    mg->setPosition(this);
    mg->getAllLegalMoves(&legalMoves);
    isCheck = !generator->isKingSafe(0,0,0,0);
    gameOver = testLeafNode();
}

// Is the game over
bool Position::testLeafNode() {
    // Checkmate or stalemate
    if (legalMoves.empty()) {
        return true;
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            switch(board[i][j]) {
            case ' ':
            case 'K':
            case 'X':
            case 'k':
            case 'x':
                break;
                // Game is still alive
            default:
                return false;
            }
        }
    }
    // Nothing but kings on the board
    return true;
}

char Position::translate(char piece) {
    switch(piece) {
    case 'T':
        return 'R';
    case 't':
        return 'r';
    case 'X':
        return 'K';
    case 'x':
        return 'k';
    case 'E':
        return 'P';
    case 'e':
        return 'p';
    }
    return piece;
}

int Position::pawnIsolationPenalty(int j, bool white) {
    char p = 'P';
    char e = 'E';
    if (white) {
        p += 32;
        e += 32;
    }
    int k = j - 1;
    if (j > 0) {
        for (int i = 0; i < 8; ++i) {
            if (board[i][k] == p || board[i][k] == e) {
                return 0;
            }
        }
    }
    if (j < 7) {
        k = j + 1;
        for (int i = 0; i < 8; ++i) {
            if (board[i][k] == p || board[i][k] == e) {
                return 0;
            }
        }
    }
    switch(j) {
    case 0:
    case 7:
        return 12;
    case 1:
    case 6:
        return 14;
    case 2:
    case 5:
        return 16;
    }
    return 20;
}

int Position::pawnBackwardPenalty(int i, int j, bool white) {
    int y = i + 1;
    int x1 = j - 1;
    int x2 = j + 1;
    char p = 'P';
    char e = 'E';
    if (white) {
        y = i - 1;
        p += 32;
        e += 32;
    }
    if ((x1 >= 0 && (board[i][x1] == p || board[i][x1] == e || board[y][x1] == p || board[y][x1] == e)) || (x2 < 8 && (board[i][x2] == p || board[i][x2] == e || board[y][x2] == p || board[y][x2] == e))) {
        return 0;
    }
    else {
        // Check that the pawn isn't on a half-open lane (which will cause
        // a higher penalty.)
        if (white) {
            for (int k = i - 1; k >= 0; --k) {
                if (board[k][j] == 'P' || board[k][j] == 'E') {
                    return 6;
                }
            }
        }
        else {
            for (int k = i + 1; k < 8; ++k) {
                if (board[k][j] == 'p' || board[k][j] == 'e') {
                    return 6;
                }
            } 
        }
    }
    return 10;
}

int Position::pawnAdvancementBonus(int i, int j, bool white) {
    int advancement;
    if (white) {
        advancement = 6 - i;
    }
    else {
        advancement = i - 1;
    }
    // Penalty for unadvanced king & queen pawns
    if (advancement == 0 && (j == 3 || j == 4)) {
        return -10;
    }
    // Big fat bonus for pawn about to queen
    if (advancement == 5) {
        return 200;
    }
    if (advancement == 4) {
        return 50;
    }
    // Bigger bonus for centre pawns
    switch (j) {
    case 0:
    case 7:
        return advancement * 2;
    case 3:
    case 4:
        return advancement * 4;
    }
    return advancement * 3;
}

int Position::pawnProximityToKingBonus(int i, int j, bool white) {
    int bonus = 0;
    char p = 'P';
    char e = 'E';
    if (white) {
        p = 'p';
        e = 'e';
    }
    for (int y = i - 1; y <= i + 1; ++y) {
        for (int x = j - 1; x <= j + 1; ++x) {
            if (x >= 0 && x < 8 && y >= 0 && y < 8 && (board[y][x] == p || board[y][x] == e)) {
                bonus += 10;
            }
        }
    }
    // Having no pawns at all in the vincinity is bad. Penalty!
    if (bonus == 0) {
        return -8;
    }
    return bonus;
}

int Position::centreBonus(int x) {
    switch (x) {
    case 0:
    case 7:
        return 0;
    case 1:
    case 6:
        return 5;
    case 2:
    case 5:
        return 10;
    default:
        return 15;
    }
}

int Position::bishopControlBonus(int i, int j, bool white) {
    int bonus = 0;
    // Check upwards to the left
    int x = j - 1;
    int y = i - 1;
    while (x >= 0 && y >= 0) {
        switch(board[y][x]) {
        case 'p':
        case 'P':
        case 'e':
        case 'E':
            goto upright;
        default:
            bonus += xrayControlBonus(y,x,white);
        }
        --x;
        --y;
    }

upright:
    // Check upwards to the right
    x = j + 1;
    y = i - 1;
    while (x < 8 && y >= 0) {
        switch(board[y][x]) {
        case 'p':
        case 'P':
        case 'e':
        case 'E':
            goto downleft;
        default:
            bonus += xrayControlBonus(y,x,white);
        }
        ++x;
        --y;
    }
downleft:
    // Check downwards to the left
    x = j - 1;
    y = i + 1;
    while (x >= 0 && y < 8) {
        switch(board[y][x]) {
        case 'p':
        case 'P':
        case 'e':
        case 'E':
            goto downright;
        default:
            bonus += xrayControlBonus(y,x,white);
        }
        --x;
        ++y;
    }
downright:
    // Check downwards to the right
    x = j + 1;
    y = i + 1;
    while (x < 8 && y < 8) {
        switch(board[y][x]) {
        case 'p':
        case 'P':
        case 'e':
        case 'E':
            return bonus;
        default:
            bonus += xrayControlBonus(y,x,white);
        }
        ++x;
        ++y;
    }
    return bonus;
}

int Position::rookControlBonus(int i, int j, bool white) {
    int bonus = 0;
    // Check to the left
    int x = j - 1;
    while (x >= 0) {
        switch(board[i][x]) {
        case 'p':
        case 'P':
        case 'e':
        case 'E':
            goto right;
        default:
            bonus += xrayControlBonus(i,x,white);
        }
        --x;
    }
right:
    // Check to the right
    x = j + 1;
    while (x < 8) {
        switch(board[i][x]) {
        case 'p':
        case 'P':
        case 'e':
        case 'E':
            goto up;
        default:
            bonus += xrayControlBonus(i,x,white);
        }
        ++x;
    }
up:
    // Check upwards
    int y = i - 1;
    while (y >= 0) {
        switch(board[y][j]) {
        case 'p':
        case 'P':
        case 'e':
        case 'E':
            goto down;
        default:
            bonus += xrayControlBonus(y,j,white);
        }
        --y;
    }
down:
    // Check downwards
    y = i + 1;
    while (y < 8) {
        switch(board[y][j]) {
        case 'p':
        case 'P':
        case 'e':
        case 'E':
            return bonus;
        default:
            bonus += xrayControlBonus(y,j,white);
        }
        ++y;
    }
    return bonus;
}

int Position::xrayControlBonus(int i, int j, bool white) {
    int bonus = 0;
    bool whiteSide = true;
    if (i < 4) {
        whiteSide = false;
    }
    switch(board[i][j]) {
    case 'b':
    case 'B':
    case 'n':
    case 'N':
    case ' ':
        break;
    default:
        // Threatening a valuable (r, q or k) piece even as an "x-ray" attack
        // is worth a bonus.
        if ((white && board[i][j] < 'Z') || (!white && board[i][j] > 'a')) {
            bonus += 8;
        }
    }
    // If white and whiteSide are the same, increment bonus by 1, otherwise by 2.
    // (Control of the enemy's squares is more valuable.)
    if (white == whiteSide) {
        ++bonus;
    }
    else {
        bonus += 2;
    }
    return bonus;
}

void Position::reset() {
    whiteToMove = true;
    isCheck = false;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = ' ';
        }
    }
    board[0][0] = 'T';
    board[0][1] = 'N';
    board[0][2] = 'B';
    board[0][3] = 'Q';
    board[0][4] = 'X';
    board[0][5] = 'B';
    board[0][6] = 'N';
    board[0][7] = 'T';
    board[7][0] = 't';
    board[7][1] = 'n';
    board[7][2] = 'b';
    board[7][3] = 'q';
    board[7][4] = 'x';
    board[7][5] = 'b';
    board[7][6] = 'n';
    board[7][7] = 't';
    for (int i = 0; i < 8; ++i) {
        board[1][i] = 'P';
        board[6][i] = 'p';
    }
}

void Position::move(int mv, int promotion) {
    // Move the piece
    board[UNPACK_I2(mv)][UNPACK_J2(mv)] = board[UNPACK_I1(mv)][UNPACK_J1(mv)];
    board[UNPACK_I1(mv)][UNPACK_J1(mv)] = ' ';
    // Check for castling
    if (board[UNPACK_I2(mv)][UNPACK_J2(mv)] == 'x') {
        if (UNPACK_J1(mv) - UNPACK_J2(mv) == 2) {
            board[7][3] = 'r';
            board[7][0] = ' ';
        }
        else if (UNPACK_J1(mv) - UNPACK_J2(mv) == -2) {
            board[7][5] = 'r';
            board[7][7] = ' ';
        }
    }
    if (board[UNPACK_I2(mv)][UNPACK_J2(mv)] == 'X') {
        if (UNPACK_J1(mv) - UNPACK_J2(mv) == 2) {
            board[0][3] = 'R';
            board[0][0] = ' ';
        }
        else if (UNPACK_J1(mv) - UNPACK_J2(mv) == -2) {
            board[0][5] = 'R';
            board[0][7] = ' ';
        }
    }

    if (whiteToMove) {
        // Check for en passant
        if (board[UNPACK_I2(mv)][UNPACK_J2(mv)] == 'p' && board[UNPACK_I2(mv) + 1][UNPACK_J2(mv)] == 'E') {
            board[UNPACK_I2(mv) + 1][UNPACK_J2(mv)] = ' ';
        }
        // The right to en passant is one-turn-only. Set all E's to P's.
        for (int j = 0; j < 8; ++j) {
            if (board[3][j] == 'E') {
                board[3][j] = 'P';
            }
        }
        // Check for pawn promotion
        if (UNPACK_I2(mv) == 0 && board[UNPACK_I2(mv)][UNPACK_J2(mv)] == 'p') {
            switch (promotion) {
            case 1:
                board[UNPACK_I2(mv)][UNPACK_J2(mv)] = 'r';
                break;
            case 2:
                board[UNPACK_I2(mv)][UNPACK_J2(mv)] = 'b';
                break;
            case 3:
                board[UNPACK_I2(mv)][UNPACK_J2(mv)] = 'n';
                break;
            default:
                board[UNPACK_I2(mv)][UNPACK_J2(mv)] = 'q';
            }
        }
    }
    else {
        // Same checks for black
        if (board[UNPACK_I2(mv)][UNPACK_J2(mv)] == 'P' && board[UNPACK_I2(mv) - 1][UNPACK_J2(mv)] == 'e') {
            board[UNPACK_I2(mv) - 1][UNPACK_J2(mv)] = ' ';
        }    

        for (int j = 0; j < 8; ++j) {
            if (board[4][j] == 'e') {
                board[4][j] = 'p';
            }
        }
        // Check for pawn promotion
        if (UNPACK_I2(mv) == 7 && board[UNPACK_I2(mv)][UNPACK_J2(mv)] == 'P') {
            switch (promotion) {
            case 1:
                board[UNPACK_I2(mv)][UNPACK_J2(mv)] = 'R';
                break;
            case 2:
                board[UNPACK_I2(mv)][UNPACK_J2(mv)] = 'B';
                break;
            case 3:
                board[UNPACK_I2(mv)][UNPACK_J2(mv)] = 'N';
                break;
            default:
                board[UNPACK_I2(mv)][UNPACK_J2(mv)] = 'Q';
            }
        }
    }
    // Check whether a piece was moved that requires a change of status
    // due to the movement
    switch(board[UNPACK_I2(mv)][UNPACK_J2(mv)]) {
    case 'x':
    case 'X':
        board[UNPACK_I2(mv)][UNPACK_J2(mv)] -= 13; // X -> K, x -> k
        break;
    case 't':
    case 'T':
        board[UNPACK_I2(mv)][UNPACK_J2(mv)] -= 2; // T -> R, t -> r
        break;
    case 'p':
    case 'P':
        if (std::abs(UNPACK_I1(mv) - UNPACK_I2(mv)) > 1) {
            board[UNPACK_I2(mv)][UNPACK_J2(mv)] -= 11; // P -> E, p -> e
        }
        break;
    }
    whiteToMove = !whiteToMove;
    legalMoves.clear();
    generator->setPosition(this);
    generator->getAllLegalMoves(&legalMoves);
    isCheck = !generator->isKingSafe(0,0,0,0);
}

void Position::print() {
    for (int i = 0; i < 8; ++i) {
        std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
        std::cout << 8 - i;
        for (int j = 0; j < 8; ++j) {
            std::cout << " | " << translate(board[i][j]);
        }
        std::cout << " |" << std::endl;
    }
    std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    std::cout << "    A   B   C   D   E   F   G   H" << std::endl << std::endl;
}

void Position::getBoard(char b[8][8], int *wkx, int *wky, int *bkx, int *bky) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == 'K' || board[i][j] == 'X') {
                *bkx = j;
                *bky = i;
            }
            if (board[i][j] == 'k' || board[i][j] == 'x') {
                *wkx = j;
                *wky = i;
            }
            b[i][j] = board[i][j];
        }
    }
}

inline bool Position::isWhiteToMove() {
    return whiteToMove;
}

bool Position::isGameOver() {
    return gameOver;
}

int Position::evaluate() {
    if (gameOver) {
        if (isCheck) {
            if (whiteToMove) {
                // White is checkmated
                return std::numeric_limits<int>::min();
            }
            else {
                // Black is checkmated
                return std::numeric_limits<int>::max();
            }
        }
        else {
            // Stalemate or two kings butting heads
            return 0;
        }
    }
    int white = 0;
    int black = 0;
    int wkx,wky,bkx,bky;
    int lesserPiece = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            switch(board[i][j]) {
            case 'R':
            case 'T':
                black += 520;
                black += rookControlBonus(i,j,false);
                break;
            case 'B':
                black += 330;
                black += bishopControlBonus(i,j,false);
                break;
            case 'N':
                black += 330;
                black += centreBonus(i);
                black += centreBonus(j);
                break;
            case 'Q':
                black += 950;
                break;
            case 'P':
            case 'E':
                black += 100;
                black -= pawnIsolationPenalty(j,false);
                //	black -= pawnBackwardPenalty(i,j,false);
                black += pawnAdvancementBonus(i,j,false);
                break;
            case 'K':
            case 'X':
                bkx = j;
                bky = i;
                break;
            case 'r':
            case 't':
                white += 520;
                white += rookControlBonus(i,j,true);
                break;
            case 'b':
                white += 330;
                white += bishopControlBonus(i,j,true);
                break;
            case 'n':
                white += 330;
                white += centreBonus(i);
                white += centreBonus(j);
                break;
            case 'q':
                white += 950;
                break;
            case 'p':
            case 'e':
                white += 100;
                white -= pawnIsolationPenalty(j,true);
                //	white -= pawnBackwardPenalty(i,j,true);
                white += pawnAdvancementBonus(i,j,true);
                break;
            case 'k':
            case 'x':
                wkx = j;
                wky = i;
                break;
            }
        }
    }
    // Before the endgame we don't want the king in the centre of the board
    if (white + black > 1700) {
        black -= centreBonus(bkx);
        black -= centreBonus(bky);
        white -= centreBonus(wkx);
        white -= centreBonus(wky);
        black += pawnProximityToKingBonus(bky,bkx,false);
        white += pawnProximityToKingBonus(wky,wkx,true);
    }
    // But during the endgame he's welcome.
    else {
        black += centreBonus(bkx);
        black += centreBonus(bky);
        white += centreBonus(wkx);
        white += centreBonus(wky);
    }
    return white - black;
}

std::vector<int>* Position::getLegalMoves() {
    return &legalMoves;
}

MovementGenerator* Position::getMovementGenerator() {
    return generator;
}

long Position::getHash() {
    long hash = 0;
    // Hash piece positions
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            switch (board[i][j]) {
            case 'P':
                hash = hash ^ (1 << (i * 8 + j)) ^ 1;
                break;
            case 'E':
                hash = hash ^ (1 << (i * 8 + j)) ^ 2;
                break;
            case 'T':
            case 'R':
                hash = hash ^ (1 << (i * 8 + j)) ^ 4;
                break;
            case 'N':
                hash = hash ^ (1 << (i * 8 + j)) ^ 8;
                break;
            case 'B':
                hash = hash ^ (1 << (i * 8 + j)) ^ 16;
                break;
            case 'Q':
                hash = hash ^ (1 << (i * 8 + j)) ^ 32;
                break;
            case 'K':
            case 'X':
                hash = hash ^ (1 << (i * 8 + j)) ^ 64;
                break;
            case 'p':
                hash = hash ^ (1 << (i * 8 + j)) ^ 128;
                break;
            case 'e':
                hash = hash ^ (1 << (i * 8 + j)) ^ 256;
                break;
            case 't':
            case 'r':
                hash = hash ^ (1 << (i * 8 + j)) ^ 512;
                break;
            case 'n':
                hash = hash ^ (1 << (i * 8 + j)) ^ 1024;
                break;
            case 'b':
                hash = hash ^ (1 << (i * 8 + j)) ^ 2048;
                break;
            case 'q':
                hash = hash ^ (1 << (i * 8 + j)) ^ 4096;
                break;
            case 'k':
            case 'x':
                hash = hash ^ (1 << (i * 8 + j)) ^ 8192;
                break;
            }
        }
    }
    // Hash castling rights
    if (board[0][4] == 'X') {
        if (board[0][0] == 'T') {
            hash = hash ^ 16384;
        }
        if (board[0][7] == 'T') {
            hash = hash ^ 32768;
        }
    }
    if (board[7][4] == 'x') {
        if (board[7][0] == 't') {
            hash = hash ^ 65536;
        }
        if (board[7][7] == 't') {
            hash = hash ^ 131072;
        }
    }
    // Hash turn
    if (whiteToMove) {
        hash = hash ^ 262144;
    }
    return hash;
}