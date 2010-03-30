#ifndef _NACLG_SOCKET_EXCEPTION_H
#define _NACLG_SOCKET_EXCEPTION_H

#include <string>

class SocketException
{
    public:
        SocketException ( std::string s ) : mMsg ( s ) {};
        ~SocketException (){};
        std::string description() { return mMsg; }

    private:
        std::string mMsg;
};

#endif
