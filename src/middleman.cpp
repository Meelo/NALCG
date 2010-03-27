// system includes

// class dependencies
#include "middleman.h"
#include "logic/chessboard.h"

MiddleMan::MiddleMan(const std::vector<AI*>& aiList,
    const std::vector<AIInfo>& aiInfos) :
    board(0), currentTurn(Piece::WHITE), rounds(0), aiList(aiList),
    aiInfos(aiInfos)
{
    assert(aiList.size() == aiInfos.size());
}

// public methods
void MiddleMan::startGame()
{
    // TODO: read board type from config file or something
    board = new ChessBoard(ChessBoard::createBoard());
    rounds = 0;

    // add players

    // white starts
    currentTurn = Piece::WHITE;
}

Piece::Colour MiddleMan::endGame()
{
    // TODO: Cleaning up the mess
    delete board;
    board = 0;

    // winner
    return Piece::WHITE;
}

std::vector<std::size_t>  MiddleMan::getValidMovesAt(std::size_t x, std::size_t y) const
{
    return board->getValidMoves(x, y);
}

void MiddleMan::move(   std::size_t fromX, std::size_t fromY,
                        std::size_t toX,   std::size_t toY)
{
    if (board->move(fromX, fromY, toX, toY))
    {
        moveUpdate(fromX, fromY, toX, toY);
    }
}

void  MiddleMan::undo()
{

}

// private methods
void MiddleMan::playRound()
{

}

// private methods
void MiddleMan::moveUpdate( std::size_t fromX, std::size_t fromY,
                            std::size_t toX,   std::size_t toY)
{
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        views.at(i)->move(fromX, fromY, toX, toY);
    }
}

void MiddleMan::boardUpdate()
{
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        views.at(i)->setBoard(board, rounds);
    }
}
