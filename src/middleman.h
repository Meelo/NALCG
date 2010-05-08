#ifndef _NALCG_MIDDLEMAN_H_
#define _NALCG_MIDDLEMAN_H_

// class includes
#include "ais/ai.h"
#include "ais/aiinfo.h"
#include "enduser.h"
#include "logic/colour.h"
#include "logic/board.h"
#include "logic/chesspieces-meta.h"
#include "client/remoteplayer.h"

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

    std::vector<std::size_t> getValidMovesAt(std::size_t x, std::size_t y) const;

    unsigned int move(  std::size_t fromX, std::size_t fromY,
                        std::size_t toX,   std::size_t toY,
                        unsigned int promoteTo = 0);

    void undo(unsigned int steps = FULL_TURN);

    std::size_t getAICount() const { return aiList.size(); }

    const AIInfo& getAIInfoAt(std::size_t index) const { return aiInfos.at(index); }

    const std::vector<std::string>& getGameLog() const { return gameLog; }

    const Board* getGameStateAt(std::size_t index) const { return gameStates.at(index); }

    // Should be called *after* playRound(), since that updates currentPlayer.
    unsigned int getGameConditionMask() const;

    void addView(EndUser* view) { views.push_back(view); }

    // 0 = Human, 1..n = AI player index + 1.
    void setControl(unsigned int whiteController, unsigned int blackController);

    // Methods (possibly) over IP
    void sendChallenge(const std::string& remotePlayer);

    void promptChallenge(const std::string& challenger);

    void respondToChallenge(bool accept);

    void updateUsers(const std::vector<std::string>& users);

private:
    // Members
    Board* board;
    RemotePlayer client;
    Colour currentTurn;
    unsigned int rounds;
    std::vector<EndUser*> views;
    std::vector<AI*> aiList;
    std::vector<AIInfo> aiInfos;
    std::vector<Board*> gameStates;
    std::vector<std::string> gameLog;
    bool running;

    // Methods
    void playRound();

    void moveUpdate(std::size_t fromX, std::size_t fromY,
                    std::size_t toX,   std::size_t toY, bool continuous = false);

    void promoteUpdate( std::size_t fromX, std::size_t fromY,
                        std::size_t toX,   std::size_t toY,
                        unsigned int promoteTo);

    void boardUpdate();

    void updateBoardForAI();

    const std::string newLogEntry(  std::size_t fromX, std::size_t fromY,
                                    std::size_t toX,   std::size_t toY) const;

    template <typename T>
    void deleteAndClear(std::vector<T>& vector);
};

#endif // _NALCG_MIDDLEMAN_H_
