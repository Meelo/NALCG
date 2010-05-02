#ifndef _NALCG_MIDDLEMAN_H_
#define _NALCG_MIDDLEMAN_H_

// class includes
#include "ais/ai.h"
#include "ais/aiinfo.h"
#include "enduser.h"
#include "logic/colour.h"
#include "logic/board.h"
#include "logic/chesspieces-meta.h"

// system includes
#include <cassert>
#include <vector>

class Middleman
{
public:
    // Constants
    static const unsigned int HALF_TURN = 1;
    static const unsigned int FULL_TURN = 2;

    Middleman(const std::vector<AI*>& aiList, const std::vector<AIInfo>& aiInfos);
    virtual ~Middleman();

    // Methods
    void startGame();

    Colour endGame();

    std::vector<std::size_t> getValidMovesAt(std::size_t x, std::size_t y) const;

    unsigned int move(  std::size_t fromX, std::size_t fromY,
                        std::size_t toX,   std::size_t toY,
                        unsigned int promoteTo = 0);

    void undo(unsigned int steps = FULL_TURN);

    void updateBoardForAI();

    // Getters
    std::size_t getAICount() const { return aiList.size(); }

    const AIInfo& getAIInfoAt(std::size_t index) const { return aiInfos.at(index); }

    const std::vector<std::string>& getGameLog() const { return gameLog; }

    const Board* getGameStateAt(std::size_t index) const { return gameStates.at(index); }

    // Should be called *after* playRound(), since that updates currentPlayer.
    unsigned int getGameConditionMask() const;

    // Setters
    void addView(EndUser* view) { views.push_back(view); }

private:
    // Members
    Board* board;
    Colour currentTurn;
    unsigned int rounds;
    std::vector<EndUser*> views;
    std::vector<AI*> aiList;
    std::vector<AIInfo> aiInfos;
    std::vector<Board*> gameStates;
    std::vector<std::string> gameLog;

    // Methods
    void playRound();

    void moveUpdate(std::size_t fromX, std::size_t fromY,
                    std::size_t toX,   std::size_t toY, bool continuous = false);

    void promoteUpdate( std::size_t fromX, std::size_t fromY,
                        std::size_t toX,   std::size_t toY,
                        unsigned int promoteTo);

    void boardUpdate();

    const std::string newLogEntry(  std::size_t fromX, std::size_t fromY,
                                    std::size_t toX,   std::size_t toY) const;

    template <typename T>
    void deleteAndClear(std::vector<T>& vector);
};

#endif // _NALCG_MIDDLEMAN_H_
