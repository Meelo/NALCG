#include "remoteplayer.h"
#include "network.h"
#include "../middleman.h"
#include "../logic/chessboard.h"

RemotePlayer::RemotePlayer()
    : mNetwork(Network::createNewNetwork())
    , mConnected(false)
    , mThread(0)
    , mCollectUsers(false)
{
}

RemotePlayer::~RemotePlayer()
{
    mConnected = false;
    if (mThread)
    {
        mThread->join();
        delete mThread;
    }
    delete mNetwork;
}

void RemotePlayer::init(const Board* board, Middleman* middleman)
{
    mMiddleman = middleman;

    mNetwork->connect("dzarg.mine.nu", "6668");
    mNetwork->startBuffering();

    mConnected = true;
    mThread = new boost::thread(boost::bind(&RemotePlayer::handleIncomingMessages, this));

    // Send nickname.
    std::ostringstream nick;
    nick << rand();
    mNetwork->sendln(nick.str());
}

void RemotePlayer::move(int fromX, int fromY, int toX, int toY, unsigned int promoteTo)
{
    std::ostringstream message;
    message << "TPE_3M ";
    message << fromX << " " << fromY << " " << toX << " " << toY << " " << promoteTo;
    mNetwork->sendln(message.str());
}


void RemotePlayer::sendUndo(unsigned int steps)
{
    std::ostringstream message;
    message << "TPE_3U ";
    message << steps;
    mNetwork->sendln(message.str());
}

void RemotePlayer::setControl(bool white, bool black)
{
    // This holds no meaning in network play.
    // Or at least so it seems.
    // In reality, we are going to use this method to define who controls
    // what and where.
}

void RemotePlayer::sendChallenge(const std::string& name)
{
    mNetwork->sendln("MSG_B" + name);
}

void RemotePlayer::respondToChallenge(bool accept)
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

void RemotePlayer::handleIncomingMessages()
{
    while (mConnected)
    {
        while (mNetwork->hasLines())
        {
            std::string line = mNetwork->popLine();

            if (line.substr(0, 6) == "TPE_3M")
            {
                std::stringstream move;
                move << line.substr(7);
                std::size_t fromX, fromY, toX, toY, promoteTo;
                move >> fromX;
                move >> fromY;
                move >> toX;
                move >> toY;
                move >> promoteTo;

                std::ostringstream response;
                response << "TPE_3R ";
                response << mMiddleman->move(fromX, fromY, toX, toY, promoteTo);
                mNetwork->sendln(response.str());
            }
            else if (line.substr(0, 6) == "TPE_3U")
            {
                std::stringstream undo;
                undo << line.substr(7);
                unsigned int steps;
                undo >> steps;
                mMiddleman->undo(steps);
            }
            else if (line.substr(0, 5) == "MSG_E")
            {
                mMiddleman->promptChallenge(line.substr(5));
            }
            else if (line == "MSG_U")
            {
                mCollectUsers = true;
                mUsers.clear();
            }
            else if (line == "MSG_UEND")
            {
                mCollectUsers = false;
                mMiddleman->updateUsers(mUsers);
            }
            else if (mCollectUsers)
            {
                mUsers.push_back(line);
            }
        }
        boost::this_thread::sleep(boost::posix_time::millisec(10));
    }
}


