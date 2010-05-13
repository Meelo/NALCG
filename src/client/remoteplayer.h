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

struct NetworkInfo
{
    std::string nick;
    std::string address;
    std::string port;
};

class RemotePlayer
{
public:
    RemotePlayer();
    virtual ~RemotePlayer();

    virtual void init(Middleman* middleman);
    virtual bool connect();
    virtual void disconnect();
    virtual void move(int fromX, int fromY, int toX, int toY, unsigned int promoteTo);
    virtual void sendUndo(unsigned int steps);
    virtual void setControl(bool white, bool black);
    virtual void sendChallenge(const std::string& name);
    virtual void respondToChallenge(bool accept);

protected:
    virtual void handleIncomingMessages();
    virtual void log(const std::string& message);
    bool parseConfigFile();
    bool updateNetworkInfo( const std::string& key,
                            const std::string& value);
    void removeInvalidChars(std::string& line);

    Network* mNetwork;
    Middleman* mMiddleman;
    NetworkInfo mNInfo;
    bool mDisabled;
    bool mConnected;
    boost::thread* mThread;
    std::vector<std::string> mUsers;
    bool mCollectUsers;
};

#endif // _NALCG_REMOTE_PLAYER_H_
