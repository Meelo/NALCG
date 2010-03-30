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

Middleman::~Middleman()
{
    views.clear();
    deleteAndClear(aiList);
    deleteAndClear(gameStates);
}

// public methods
void Middleman::startGame()
{
    // TODO: read board type from config file or something
    board = new ChessBoard(ChessBoard::createBoard());
    rounds = 0;

    deleteAndClear(gameStates);
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
        moveUpdate(fromX, fromY, toX, toY);
        playRound();
    }
}

void Middleman::undo(unsigned int steps)
{
    if (gameStates.size() <= steps)
    {
        steps = gameStates.size() - 1;
    }
    if (steps >= HALF_TURN)
    {
        for (unsigned int i = 0; i < steps; ++i)
        {
            delete gameStates.back();
            gameStates.pop_back();
        }
        board = new Board(*gameStates.back());
        currentTurn = gameStates.size() % 2 ? Piece::WHITE : Piece::BLACK;
        rounds -= steps;
        boardUpdate();
    }
}

// private methods
void Middleman::playRound()
{
    gameStates.push_back(new Board(*board));
    currentTurn = currentTurn == Piece::WHITE ? Piece::BLACK : Piece::WHITE;
    ++rounds;
}

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

template <typename T>
void Middleman::deleteAndClear(std::vector<T>& vector)
{
    while (!vector.empty())
    {
        delete vector.back();
        vector.pop_back();
    }
}
