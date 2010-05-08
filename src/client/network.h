#ifndef _NALCG_NETWORK_H_
#define _NALCG_NETWORK_H_

// system includes
#include <string>

class Network
{
public:
    virtual ~Network() { }

    virtual bool connect(const char* ip, const char* port) = 0;
    virtual void send(const std::string& message) = 0;
    virtual void sendln(const std::string& message) = 0;
    virtual void startBuffering(char delimiter = '\n') = 0;
    virtual bool hasLines() = 0;
    virtual std::string popLine() = 0;
    virtual void disconnect() = 0;

    static Network* createNewNetwork();
};

#endif // _NALCG_NETWORK_H_
