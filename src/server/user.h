#ifndef _NACLG_USER_H
#define _NALCG_USER_H

#include "user.h"
#include "serversocket.h"
#include "socketexception.h"
#include <string>

class User
{
    private:
        string mName;
        ServerSocket *mSocket;
        pthread_t mSid;

    public:
        User( ) { }
        User ( ServerSocket *socket, string name );
        ~User();
        string getName ( );
        void setName(string name);
        ServerSocket* getSocket ( );
        pthread_t getSid();
        void setSid(pthread_t sid);
};


#endif



