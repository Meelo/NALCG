#ifndef _NACLG_SERVER_SOCKET_H
#define _NACLG_SERVER_SOCKET_H

#include "socket.h"

class ServerSocket : private Socket
{
    public:
        ServerSocket ( int port );
        ServerSocket (){};
        virtual ~ServerSocket();
        const ServerSocket& operator << ( const std::string& ) const;
        const ServerSocket& operator >> ( std::string& ) const;
        void accept ( ServerSocket& );
        void listen ( );
        bool testConnection ( );
};

#endif
