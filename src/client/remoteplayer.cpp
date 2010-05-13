#include "remoteplayer.h"
#include "network.h"
#include "../middleman.h"
#include "../logic/chessboard.h"

#include <fstream>
#include <sstream>

RemotePlayer::RemotePlayer()
    : mNetwork(Network::createNewNetwork())
    , mMiddleman(0)
    , mDisabled(true)
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
    //~ mDisabled = !parseConfigFile();
    mDisabled = true;
}


bool RemotePlayer::connect()
{
    if (mDisabled)
    {
        log("Re-parsing configs begin");
        mDisabled = !parseConfigFile();
        log("Re-parsing configs end");
    }

    log("Connect begin");
    if (!mDisabled
        && mNetwork->connect(mNInfo.address.c_str(), mNInfo.port.c_str()))
    {
        log("Connect successful");
        mNetwork->startBuffering();

        mConnected = true;
        mThread = new boost::thread(boost::bind(
            &RemotePlayer::handleIncomingMessages, this));

        // Send nickname.
        log("Nickname send begin");
        mNetwork->send(mNInfo.nick);
        log("Nickname send end");

        return true;
    }
    log("Connect failed");
    mDisabled = true;
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
    if (!mDisabled)
    {
        log("Move begin");
        std::ostringstream message;
        message << "TPE_3M ";
        message << fromX << " " << fromY << " " << toX << " " << toY << " " << promoteTo;
        mNetwork->sendln(message.str());
        log("Move end");
    }
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


bool RemotePlayer::parseConfigFile()
{
    log("parsing begin");
    std::ifstream inputFile("../data/remote.cfg");
    if (!inputFile.is_open())
    {
        log("parsing end, Fail(couldn't open config file)");
        return false;
    }

    std::string line;
    while (std::getline(inputFile, line))
    {
        removeInvalidChars(line);
        std::size_t separator;

        if (!line.empty()
            && (separator = line.find('=')) != std::string::npos)
        {
            std::string key = line.substr(0, separator);
            std::string value = line.substr(separator + 1);

            if (!updateNetworkInfo(key, value))
            {
                log("parsing end, Fail(wrong key)");
                return false;
            }
        }
    }

    log("parsing end, success");
    return true;
}


bool RemotePlayer::updateNetworkInfo(const std::string& key,
                                     const std::string& value)
{
    if (key == "name")
    {
        mNInfo.nick = value;
        return true;
    }
    if (key == "address")
    {
        mNInfo.address = value;
        return true;
    }
    if (key == "port")
    {
        std::stringstream ss(value);
        unsigned int num;
        if ((ss >> num).fail())
        {
            return false;
        }

        mNInfo.port = value;
        return true;
    }

    return false;
}

void RemotePlayer::removeInvalidChars(std::string& line)
{
    std::size_t found;
    while ((found = line.find_first_not_of("abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]=-_#.")) != std::string::npos) {
        line.erase(found, 1);
    }
}


