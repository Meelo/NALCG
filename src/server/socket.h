#ifndef Socket_class
#define Socket_class

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 30;
const int MAXRECV = 500;

class Socket
{
    public:
        Socket();
        virtual ~Socket();
        bool create();
        bool bind ( const int port );
        bool listen() const;
        bool accept ( Socket& ) const;
        bool send ( const std::string ) const;
        int recv ( std::string& ) const;
        bool isValid() const { return mSock != -1; }

    private:
        int mSock;
        sockaddr_in mAddr;
};
#endif
