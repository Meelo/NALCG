#include "socket.h"
#include <iostream>
#include <string>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

Socket::Socket() : mSock ( -1 )
{
    memset ( &mAddr, 0, sizeof ( mAddr ) );
}

Socket::~Socket()
{
    if ( isValid() )::close ( mSock );
}

bool Socket::create()
{
    mSock = socket ( AF_INET, SOCK_STREAM, 0 );

    if ( ! isValid() )
    {
        return false;
    }

    int on = 1;
    if ( setsockopt ( mSock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    {
        return false;
    }

    return true;
}

bool Socket::bind ( const int port )
{
    if ( ! isValid() )
    {
        return false;
    }

    mAddr.sin_family = AF_INET;
    mAddr.sin_addr.s_addr = INADDR_ANY;
    mAddr.sin_port = htons ( port );

    int bind_return = ::bind ( mSock, ( struct sockaddr * ) &mAddr, sizeof ( mAddr ) );

    if ( bind_return == -1 )
    {
        return false;
    }

    return true;
}


bool Socket::listen() const
{
    if ( ! isValid() )
    {
        return false;
    }

    int listen_return = ::listen ( mSock, MAXCONNECTIONS );

    if ( listen_return == -1 )
    {
        return false;
    }

  return true;
}


bool Socket::accept ( Socket& new_socket ) const
{ 
    int addr_length = sizeof ( mAddr );
    new_socket.mSock = ::accept ( mSock, ( sockaddr * ) &mAddr, ( socklen_t * ) &addr_length );

    if ( new_socket.mSock <= 0 ) 
    {
        return false;
    } 
    else
    {
        return true;
    }
}


bool Socket::send ( const std::string s ) const
{
    int status = ::send ( mSock, s.c_str(), s.size(), MSG_NOSIGNAL );

    if ( status == -1 )
    { 
        return false;
    }
    else
    {
        return true;
    }
}


int Socket::recv ( std::string& s ) const
{
    char buf [ MAXRECV + 1 ];

    s = "";

    memset ( buf, 0, MAXRECV + 1 );

    int status = ::recv ( mSock, buf, MAXRECV, 0 );

    if ( status == -1 )
    {
        return 0;
    }
    else if ( status == 0 )
    {
        return 0;
    }
    else
    {
        s = buf;
        return status;
    }
}

