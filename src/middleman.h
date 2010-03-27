#ifndef _NALCG_MIDDLE_MAN_H_
#define _NALCG_MIDDLE_MAN_H_

// system includes
#include <cassert>
#include <vector>

// class includes
#include "ai.h"
#include "aiinfo.h"
#include "enduser.h"
#include "logic/board.h"
#include "logic/chesspieces-meta.h"

class Middleman
{
    // Constants

    // Members
    Board* board;
    Piece::Colour currentTurn;
    unsigned int rounds;
    std::vector<EndUser*> views;
    std::vector<AI*> aiList;
    std::vector<AIInfo> aiInfos;

    // Methods
    void playRound();
    void moveUpdate(std::size_t fromX, std::size_t fromY,
                    std::size_t toX,   std::size_t toY);
    void boardUpdate();

public:
    Middleman(const std::vector<AI*>& aiList, const std::vector<AIInfo>& aiInfos);
    virtual ~Middleman() { }

    // Methods
    void startGame();
    Piece::Colour endGame();
    std::vector<std::size_t> getValidMovesAt(std::size_t x, std::size_t y) const;
    void move(  std::size_t fromX, std::size_t fromY,
                std::size_t toX,   std::size_t toY);
    void undo();

    // Getters
    std::size_t getAICount() const { return aiList.size(); }
    const AIInfo& getAIInfoAt(std::size_t index) const { return aiInfos.at(index); }

    // Setters
    void addView(EndUser* view) { views.push_back(view); }
};

#endif // _NALCG_MIDDLE_MAN_H_
