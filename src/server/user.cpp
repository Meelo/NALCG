#include "user.h"
#include "serversocket.h"
#include <string>

User::User ( ) { }

User::User ( ServerSocket *socket, std::string name ) : mName(name), mSocket(socket) 
{ 
    mPlaying = false;
}

User::~User()
{ 
    delete mSocket; 
}
  
std::string& User::getName() 
{ 
    return mName; 
}
  
void User::setName(std::string& name)
{ 
    mName = name; 
}

ServerSocket* User::getSocket()
{ 
    return mSocket; 
}

pthread_t User::getSid() 
{ 
    return mSid; 
}
  
void User::setSid(pthread_t sid) { 
    mSid = sid; 
}

bool User::testConnection()
{
    return mSocket->testConnection();
}

bool User::isPlaying() 
{
    return mPlaying;
}

void User::setPlaying(bool value)
{
    mPlaying = value;
}

void User::setOpponent(User* opponent)
{
    mOpponent = opponent;
}

User* User::getOpponent()
{
    return mOpponent;
}
