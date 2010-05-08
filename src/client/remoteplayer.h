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

    virtual void init(Middleman* middleman);
    virtual bool connect(const char* ip, const char* port);
    virtual void disconnect();
    virtual void move(int fromX, int fromY, int toX, int toY, unsigned int promoteTo);
    virtual void sendUndo(unsigned int steps);
    virtual void setControl(bool white, bool black);
    virtual void sendChallenge(const std::string& name);
    virtual void respondToChallenge(bool accept);

protected:
    virtual void handleIncomingMessages();
    virtual void log(const std::string& message);

    Network* mNetwork;
    Middleman* mMiddleman;
    bool mConnected;
    boost::thread* mThread;
    std::vector<std::string> mUsers;
    bool mCollectUsers;
};

#endif // _NALCG_REMOTE_PLAYER_H_
