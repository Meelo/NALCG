// class dependencies
#include "middleman.h"
#include "logic/chessboard.h"

// system includes

Middleman::Middleman(const std::vector<AI*>& aiList,
    const std::vector<AIInfo>& aiInfos) :
    board(0), currentTurn(WHITE), rounds(0), aiList(aiList),
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
    gameStates.push_back(board->clone());

    // white starts
    currentTurn = WHITE;

    // add players
    for (std::size_t i = 0; i < views.size(); i++)
    {
        views.at(i)->init(board, this);
    }

}

Colour Middleman::endGame()
{
    // TODO: Cleaning up the mess
    delete board;
    board = 0;

    // winner
    return WHITE;
}

std::vector<std::size_t>  Middleman::getValidMovesAt(std::size_t x, std::size_t y) const
{
    return board->getValidMoves(x, y);
}

unsigned int Middleman::move(   std::size_t fromX, std::size_t fromY,
                                std::size_t toX,   std::size_t toY,
                                unsigned int promoteTo)
{
    // make mutable versions of parameters,
    // since board's move() will update them when necessary.
    std::size_t x0 = fromX;
    std::size_t y0 = fromY;
    std::size_t x1 = toX;
    std::size_t y1 = toY;

    std::string currentMove = newLogEntry(fromX, fromY, toX, toY);
    unsigned int retValue = board->move(x0, y0, x1, y1, currentTurn, promoteTo);
    if (retValue & Board::MOVE_OK)
    {
        gameLog.push_back(currentMove);

        // nothing changed, do original move
        if (x0 == fromX && y0 == fromY && x1 == toX && y1 == toY)
        {
            if (retValue & Board::PROMOTION_OK)
            {
                promoteUpdate(fromX, fromY, toX, toY, promoteTo);
            }
            else
            {
                moveUpdate(fromX, fromY, toX, toY);
            }
        }
        else
        {
            // first make the extra move generated by move
            moveUpdate(x0, y0, x1, y1, true);
            // and then make the original move
            moveUpdate(fromX, fromY, toX, toY, false);
        }
        playRound();
    }

    return retValue;
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
            gameLog.pop_back();
        }
        board = gameStates.back()->clone();
        currentTurn = gameStates.size() % 2 ? WHITE : BLACK;
        rounds -= steps;
        boardUpdate();
    }
}

// private methods
void Middleman::playRound()
{
    gameStates.push_back(board->clone());
    currentTurn = currentTurn == WHITE ? BLACK : WHITE;
    ++rounds;
}

void Middleman::moveUpdate( std::size_t fromX, std::size_t fromY,
                            std::size_t toX,   std::size_t toY, bool continuous)
{
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        views.at(i)->move(fromX, fromY, toX, toY, continuous);
    }
    board->initRoundSpecificState();
}

void Middleman::promoteUpdate(  std::size_t fromX,  std::size_t fromY,
                                std::size_t toX,    std::size_t toY,
                                unsigned int promoteTo)
{
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        views.at(i)->promoteMove(fromX, fromY, toX, toY, promoteTo);
    }
    board->initRoundSpecificState();
}

void Middleman::boardUpdate()
{
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        views.at(i)->setBoard(board, rounds);
    }
}

const std::string Middleman::newLogEntry(std::size_t fromX, std::size_t fromY,
    std::size_t toX, std::size_t toY) const
{
    std::string currentMove;
    currentMove += board->getSymbolAt(fromX, fromY);
    currentMove += static_cast<char>(fromX + 'A');
    currentMove += static_cast<char>((board->getWidth() - fromY) + '0');

    currentMove += " ";
    currentMove += board->getSymbolAt(toX, toY);
    currentMove += static_cast<char>(toX + 'A');
    currentMove += static_cast<char>((board->getWidth() - toY) + '0');

    return currentMove;
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
