#ifndef _NACLG_SERVER_H
#define _NACLG_SERVER_H

#include "user.h"
#include "serversocket.h"
#include <list>

const unsigned int MAX = 30;

class Server
{
    public:
        Server ( );
        ~Server ( );

        void createSocket(int port = 6668);
        void accept(ServerSocket& socket);
        User* addUser( );
        void quittedUsers ( );
        void clientService ( User* user );
        std::string getTime(int value);
        bool checkName(std::string &name);
        void sendMsg(std::string& msg);
        bool isCtrlMsg(std::string& msg);
        User* searchUser(std::string& name);
        void userList();

    private:
        ServerSocket *mSocket;
        std::list<User*> mClients;
        pthread_rwlock_t mLock;
        void doCtrl(std::string& msg, User* user);
        void sendMsgPlaying(std::string& msg, User* user);
};
#endif
