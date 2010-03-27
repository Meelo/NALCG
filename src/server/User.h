#ifndef User_class
#define User_class

using namespace std;

#include "User.h"
#include "ServerSocket.h"
#include "SocketException.h"
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
