// class dependencies
#include "network.h"

// system includes
#include <iostream>
#include <vector>

// boost includes
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

class NetworkImpl : public Network
{
public:
    NetworkImpl();
    virtual ~NetworkImpl();
    virtual bool connect(const char* ip, const char* port);
    virtual void send(const std::string& message);
    virtual void sendln(const std::string& message);
    virtual void startBuffering(char delimiter = '\n');
    virtual bool hasLines();
    virtual std::string popLine();

protected:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;
    boost::mutex mutex;
    std::vector<std::string> linesBuffer;
    char delimiter;
    bool connected;

    virtual void readToBuffer();
};

using boost::asio::ip::tcp;

NetworkImpl::NetworkImpl() : socket(io_service), delimiter(0), connected(false)
{

}

NetworkImpl::~NetworkImpl()
{
}

bool NetworkImpl::connect(const char* ip, const char* port)
{
    try
    {
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), ip, port);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        socket.connect(*iterator);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        connected = false;
        return false;
    }
    connected = true;
    return true;
}

void NetworkImpl::startBuffering(char delimiter)
{
    this->delimiter = delimiter;
    boost::thread thread(boost::bind(&NetworkImpl::readToBuffer, this));
}

void NetworkImpl::readToBuffer()
{
    try {
        boost::asio::streambuf input;
        std::string hanging; // Stores partial lines
        while (boost::asio::read(socket, input, boost::asio::transfer_at_least(1))) {
            std::istream inputStream(&input);
            while (inputStream.good()) {
                std::stringbuf sb;
                // Get data until '\n' and take it too.
                // Or until the end of the stream.
                while (inputStream.good()) {
                    int c = inputStream.get();
                    if (c == delimiter) { break; } // End of line.
                    if (c < 0) { break; } // End of stream.
                    sb.sputc(c);
                }
                // If the stream hasn't ended yet, then a complete line has been received.
                if (inputStream.good()) {
                    // If there was a partially received line then connect it to its end that was just received.
                    std::string line = hanging + sb.str();

                    {
                        boost::mutex::scoped_lock l(mutex);
                        linesBuffer.push_back(line); // Storing the line.
                    }
                    hanging.clear();
                }
                else {
                    // Line was only partially received, add it to hanging string.
                    hanging += sb.str();
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

bool NetworkImpl::hasLines()
{
    {
        boost::mutex::scoped_lock l(mutex);
        return linesBuffer.size() > 0;
    }
}

std::string NetworkImpl::popLine()
{
    {
        boost::mutex::scoped_lock l(mutex);
        std::string line = linesBuffer.front(); // FIFO
        // Can't do front & back swap since we need to preserve the order.
        linesBuffer.erase(linesBuffer.begin()); // O(n) erase :/
        return line;
    }
}

void NetworkImpl::send(const std::string& message)
{
    if (connected)
    {
        boost::asio::write(socket, boost::asio::buffer(message, message.size()));
    }
}

void NetworkImpl::sendln(const std::string& message)
{
    send(message + "\n");
}

Network* Network::createNewNetwork()
{
    return new NetworkImpl(); 
}