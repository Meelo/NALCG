#ifndef _NALCG_REMOTE_PLAYER_H_
#define _NALCG_REMOTE_PLAYER_H_

#include <boost/thread/thread.hpp>

// system includes
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>

class Middleman;
class Board;
class Network;

class RemotePlayer
{
public:
    RemotePlayer();
    virtual ~RemotePlayer();

    virtual void init(const Board* board, Middleman* middleman);
    virtual void move(int fromX, int fromY, int toX, int toY, unsigned int promoteTo);
    virtual void sendUndo(unsigned int steps);
    virtual void setControl(bool white, bool black);
    virtual void sendChallenge(const std::string& name);
    virtual void respondToChallenge(bool accept);
    virtual void handleIncomingMessages();

protected:
    Network* mNetwork;
    Middleman* mMiddleman;
    bool mConnected;
    boost::thread* mThread;
    std::vector<std::string> mUsers;
    bool mCollectUsers;
};

#endif // _NALCG_REMOTE_PLAYER_H_
