#ifndef _NALCG_USER_H
#define _NALCG_USER_H

#include "serversocket.h"

class User
{
    public:
        User( );
	User ( ServerSocket *socket, std::string name );
	~User();
	std::string& getName();
	void setName(std::string &name);
	ServerSocket* getSocket ( );
	pthread_t getSid();
	void setSid(pthread_t sid);
	bool testConnection();
	bool isPlaying();
	void setPlaying(bool value);
	User* getOpponent();
	void setOpponent(User* opponent);

    private:
	bool mPlaying;
        std::string mName;
        ServerSocket *mSocket;
        pthread_t mSid;
	User* mOpponent;
};
#endif



