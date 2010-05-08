#include "server.h"
#include "user.h"
#include "serversocket.h"
#include "socketexception.h"
#include <string>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <cstdlib>

Server mServer;

void *scanUsers(void *arg) {
    mServer.quittedUsers();
    pthread_exit(NULL) ;
}

void *client( void *arg ) {
    ((User*)arg)->setSid((unsigned int)pthread_self());
    mServer.clientService((User*)arg);
    pthread_exit(NULL) ;
}

int main ( int argc, char **argv )
{
    pthread_t sid1;
    User *user;

    // usage:   ./NALCG-server <port_number>
    // example: ./NALCG-server 6611
    // example: ./NALCG-server
    // default port is 6668
    if (argc >= 2)
    {
        int port = atoi(argv[1]);
        if (port > 0)
        {
            mServer.createSocket(port);
        }
    }
    else
    {
        mServer.createSocket(6668);
    }

    try
    {
        pthread_create( &sid1, NULL, scanUsers, NULL ) ;

        while ( true )
        {
            user = mServer.addUser();
            if(user != NULL)
            {
                pthread_t sid;
                pthread_create( &sid, NULL, client, (void *)user ) ;
            }
        }
    }
    catch ( SocketException& e )
    {
    }
    return 0;
}


