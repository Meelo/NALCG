#ifndef _NALCG_REMOTE_PLAYER_H_
#define _NALCG_REMOTE_PLAYER_H_

// class includes
#include "network.h"

// system includes
#include <sstream>

class Middleman;
class Board;

class RemotePlayer
{
public:
    RemotePlayer() : mNetwork(Network::createNewNetwork())
    {
    }

    virtual ~RemotePlayer()
    {
        delete mNetwork;
    }

    virtual void init(const Board* board, Middleman* middleman)
    {
        mMiddleman = middleman;
        mNetwork->connect("dzarg.mine.nu", "6668");
        mNetwork->startBuffering();

        // Send nickname.
        std::ostringstream nick;
        nick << rand();
        mNetwork->sendln(nick.str());
    }

    virtual void move(int fromX, int fromY, int toX, int toY)
    {
        std::ostringstream message;
        message << "TPE_3 ";
        message << fromX << " " << fromY << " " << toX << " " << toY;
        mNetwork->sendln(message.str());
    }


    virtual void setBoard(const Board* board, unsigned int round)
    {
        // Not implemented.
    }

    virtual void setControl(bool white, bool black)
    {
        // This holds no meaning in network play.
        // Or at least so it seems.
        // In reality, we are going to use this method to define who controls
        // what and where.
    }

    virtual void sendChallenge(const std::string& name)
    {
        mNetwork->sendln("MSG_B" + name);
    }

    virtual void respondToChallenge(bool accept)
    {
        if (accept)
        {
            mNetwork->sendln("MSG_C");
        }
        else
        {
            mNetwork->sendln("MSG_D");
        }
    }


    // Somehow handle incoming messages and send them to middleman.



protected:
    Network* mNetwork;
    Middleman* mMiddleman;
};

#endif // _NALCG_REMOTE_PLAYER_H_
