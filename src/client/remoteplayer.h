#ifndef _NALCG_REMOTE_PLAYER_H_
#define _NALCG_REMOTE_PLAYER_H_

// system includes

// class includes
#include "../enduser.h"
#include "network.h"

class RemotePlayer : public EndUser
{
public:
    RemotePlayer()
    {
    }
    virtual ~RemotePlayer()
    {
    }

    virtual void init(const Board* board, Middleman* middleman)
    {
        network.connect("dzarg.mine.nu", "6668");
        network.startBuffering();

        // Send nickname.
        std::ostringstream nick;
        nick << rand();
        network.sendln(nick.str());
    }

    virtual void move(int fromX, int fromY, int toX, int toY)
    {
        std::ostringstream message;
        message << "TPE_3 ";
        message << fromX << " " << fromY << " " << toX << " " << toY;
        network.sendln(message.str());
    }


    virtual void setBoard(const Board* board, unsigned int round)
    {
        // Not implemented.
    }

    virtual void setControl(bool white, bool black)
    {
        // This holds no meaning in network play.
    }

    virtual void sendChallenge(const std::string& name)
    {
        network.sendln("MSG_B" + name);
    }

    virtual void respondToChallenge(bool accept)
    {
        if (accept)
        {
            network.sendln("MSG_C");
        }
        else
        {
            network.sendln("MSG_D");
        }
    }


    // Somehow handle incoming messages and send them to middleman.

    

protected:
    Network network;
};

#endif // _NALCG_REMOTE_PLAYER_H_
