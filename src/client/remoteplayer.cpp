#include "remoteplayer.h"
#include "network.h"
#include "../middleman.h"
#include "../logic/chessboard.h"

RemotePlayer::RemotePlayer()
    : mNetwork(Network::createNewNetwork())
    , mMiddleman(0)
    , mConnected(false)
    , mThread(0)
    , mCollectUsers(false)
{
}

RemotePlayer::~RemotePlayer()
{
    disconnect();
    delete mNetwork;
}

void RemotePlayer::init(Middleman* middleman)
{
    mMiddleman = middleman;
}


bool RemotePlayer::connect(const char* ip, const char* port)
{
    log("Connect begin");
    if (mNetwork->connect(ip, port))
    {
        log("Connect successful");
        mNetwork->startBuffering();

        mConnected = true;
        mThread = new boost::thread(boost::bind(&RemotePlayer::handleIncomingMessages, this));

        // Send nickname.
        log("Nickname send begin");
        std::ostringstream nick;
        srand(static_cast<unsigned int>(time(0)));
        nick << rand();
        mNetwork->send(nick.str());
        log("Nickname send end");
        
        return true;
    }
    log("Connect failed");
    return false;
}

void RemotePlayer::disconnect()
{
    log("Disconnect begin");
    mConnected = false;
    if (mThread)
    {
        mThread->join();
        delete mThread;
    }
    log("Disconnect end");
}

void RemotePlayer::move(int fromX, int fromY, int toX, int toY, unsigned int promoteTo)
{
    log("Move begin");
    std::ostringstream message;
    message << "TPE_3M ";
    message << fromX << " " << fromY << " " << toX << " " << toY << " " << promoteTo;
    mNetwork->sendln(message.str());
    log("Move end");
}


void RemotePlayer::sendUndo(unsigned int steps)
{
    log("Undo begin");
    std::ostringstream message;
    message << "TPE_3U ";
    message << steps;
    mNetwork->sendln(message.str());
    log("Undo end[" + message.str() + "]");
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
    log("Challenge send begin[" + name + "]");
    mNetwork->send("MSG_B" + name);
    log("Challenge send end");
}

void RemotePlayer::respondToChallenge(bool accept)
{
    log("Response begin");
    if (accept)
    {
        mNetwork->sendln("MSG_C");
        log("Response true");
    }
    else
    {
        mNetwork->sendln("MSG_D");
        log("Response false");
    }
}

void RemotePlayer::handleIncomingMessages()
{
    while (mConnected)
    {
        while (mNetwork->hasLines())
        {
            std::string line = mNetwork->popLine();

            if (line != "MSG_A")
            {
                log("Received[" + line + "]");
            }

            if (line.substr(0, 6) == "TPE_3M")
            {
                log("Receive move begin");
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
                log("Receive move end");
            }
            else if (line.substr(0, 6) == "TPE_3U")
            {
                log("Receive undo begin");
                std::stringstream undo;
                undo << line.substr(7);
                unsigned int steps;
                undo >> steps;
                mMiddleman->undo(steps);
                log("Receive undo end");
            }
            else if (line.substr(0, 5) == "MSG_E")
            {
                log("Receive challenge begin[" + line.substr(5) + "]");
                mMiddleman->promptChallenge(line.substr(5));
                log("Receive challenge end");
            }
            else if (line == "MSG_U")
            {
                log("Receive userlist begin");
                mCollectUsers = true;
                mUsers.clear();
            }
            else if (line == "MSG_UEND")
            {
                mCollectUsers = false;
                mMiddleman->updateUsers(mUsers);
                log("Receive userlist end");
            }
            else if (mCollectUsers)
            {
                log("Add user[" + line + "]");
                mUsers.push_back(line);
            }
        }
        boost::this_thread::sleep(boost::posix_time::millisec(10));
    }
}

void RemotePlayer::log(const std::string& message)
{
    std::cout << "@Remote: " << message << std::endl;
}
