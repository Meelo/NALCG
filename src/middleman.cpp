// system includes

// class dependencies
#include "middleman.h"
#include "logic/chessboard.h"

Middleman::Middleman(const std::vector<AI*>& aiList,
    const std::vector<AIInfo>& aiInfos) :
    board(0), currentTurn(Piece::WHITE), rounds(0), aiList(aiList),
    aiInfos(aiInfos)
{
    assert(aiList.size() == aiInfos.size());
}

// public methods
void Middleman::startGame()
{
    // TODO: read board type from config file or something
    board = new ChessBoard(ChessBoard::createBoard());
    rounds = 0;

    gameStates.clear();
    gameStates.push_back(new Board(*board));

    // white starts
    currentTurn = Piece::WHITE;

    // add players
    for (std::size_t i = 0; i < views.size(); i++)
    {
        views.at(i)->init(board, this);
    }

}

Piece::Colour Middleman::endGame()
{
    // TODO: Cleaning up the mess
    delete board;
    board = 0;

    // winner
    return Piece::WHITE;
}

std::vector<std::size_t>  Middleman::getValidMovesAt(std::size_t x, std::size_t y) const
{
    return board->getValidMoves(x, y);
}

void Middleman::move(   std::size_t fromX, std::size_t fromY,
                        std::size_t toX,   std::size_t toY)
{
    if (board->move(fromX, fromY, toX, toY, currentTurn))
    {
        gameStates.push_back(new Board(*board));
        moveUpdate(fromX, fromY, toX, toY);
        ++rounds;
        playRound();
    }
    std::cerr << "gamestates.size() " << gameStates.size() << std::endl;
}

void Middleman::undo()
{
    std::cout  << "-------------" << std::endl;
    if (gameStates.size() > 2)
    {
        //~ delete gameStates.back();
        gameStates.pop_back();
        //~ delete gameStates.back();
        gameStates.pop_back();
        std::cerr << board << " " << gameStates.back() << std::endl;
        board = gameStates.back();
        rounds -= 2;
        //~ playRound();
        boardUpdate();
        board->printBoard();
    }
    std::cout  << "-------------" << std::endl;
    std::cerr << "Undo: gamestates.size() " << gameStates.size() << std::endl;
}

// private methods
void Middleman::playRound()
{
    currentTurn = currentTurn == Piece::WHITE ? Piece::BLACK : Piece::WHITE;
}

// private methods
void Middleman::moveUpdate( std::size_t fromX, std::size_t fromY,
                            std::size_t toX,   std::size_t toY)
{
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        views.at(i)->move(fromX, fromY, toX, toY);
    }
}

void Middleman::boardUpdate()
{
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        views.at(i)->setBoard(board, rounds);
    }
}
