#include "User.h"
#include "ServerSocket.h"
#include <string>

using namespace std;

User::User ( ServerSocket *socket, string name ) { 
    this->mName = name; 
    this->mSocket = socket; 
}

User::~User(){ 
    delete mSocket; 
}
  
string User::getName ( ) { 
    return this->mName; 
}
  
void User::setName(string name) { 
    this->mName = name; 
}

ServerSocket* User::getSocket ( ) { 
    return this->mSocket; 
}

pthread_t User::getSid() { 
    return this->mSid; 
}
  
void User::setSid(pthread_t sid) { 
    this->mSid = sid; 
}
