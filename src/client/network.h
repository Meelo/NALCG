#ifndef _NALCG_NETWORK_H_
#define _NALCG_NETWORK_H_

// system includes
#include <string>
#include <vector>

// boost includes
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

// class dependencies

class Network
{
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;
    boost::mutex mutex;
    std::vector<std::string> linesBuffer;
    char delimiter;
    virtual void readToBuffer();
public:
    Network();
    ~Network();
    virtual bool connect(const char* ip, const char* port);
    virtual void send(const std::string& message);
    virtual void sendln(const std::string& message);
    virtual void startBuffering(char delimiter = '\n');
    virtual bool hasLines();
    virtual std::string popLine();
};

#endif // _NALCG_NETWORK_H_
