#include "serversocket.h"
#include "socketexception.h"
#include <string>
#include <iostream>
#include <pthread.h>
#include "user.h"
#include <list>
#include <fstream>
#include <cstdlib>
       
unsigned int MAX = 30;
ServerSocket mServer ( 30000 );
pthread_rwlock_t mRwlock;
list<User*> mClients;

string getTime(int value) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    string thisTime;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    if(value == 1) strftime (buffer,80,"%T",timeinfo);
    else strftime (buffer,80,"%T %d.%m.%Y",timeinfo);
    thisTime = buffer;
    return thisTime;
}

void *scanUsers(void *arg) {
    User *temp;
    bool found;
    string name;
    while( true ) {
        pthread_rwlock_wrlock(&mRwlock);
        int k;
        found = false;

        std::list<User*>::iterator i;
        for ( i = mClients.begin(); i != mClients.end(); ++i)
	    {
	        if(!((*((**i).getSocket())).testConnection()) ) 
            {
	            found = true;
	            temp = (*i);
	            break;
	        } 
	    }

        if(found) {
            mClients.remove(temp);
            name = temp->getName();
            k = pthread_cancel(temp->getSid());
            delete temp; 
        }

        if(found) 
        {
	        for ( i = mClients.begin(); i != mClients.end(); ++i) 
            {
	            if((*((**i).getSocket())).testConnection() ) 
                {
                    *((**i).getSocket()) << getTime(1) + " User " + name + " quitted.";
                }
	        }
        }

        pthread_rwlock_unlock(&mRwlock);
        sleep(1);
    }
    pthread_exit(NULL) ;
}

void *client( void *arg ) {
    ServerSocket sock = *(((User*)arg)->getSocket());
    string name = ((User*)arg)->getName();
    string data, sendMsg;
    ((User*)arg)->setSid((unsigned int)pthread_self());
    while(true) 
    {
        try 
        {
            data = "";
            sendMsg = "";
            sock >> data;     
            sendMsg += getTime(1) + " " + name + ": " + data;
            pthread_rwlock_rdlock(&mRwlock);
            std::list<User*>::iterator i;
            for ( i = mClients.begin(); i != mClients.end(); ++i) 
            {
	            *((**i).getSocket()) << sendMsg;
            }
            pthread_rwlock_unlock(&mRwlock);  
        }
        catch ( SocketException& e) 
       { 
       
       }
    }
    pthread_exit(NULL) ;
}

bool checkName(string name) {
    pthread_rwlock_rdlock(&mRwlock);
    bool ehto = true;
    std::list<User*>::iterator i;
    for ( i = mClients.begin(); i != mClients.end(); ++i) 
    {
        if(name.compare((**i).getName()) == 0 ) ehto = false;
    }
    pthread_rwlock_unlock(&mRwlock);
    return ehto;
}

int main ( int argc, char **argv )
{
    std::cout << "Server started at " << getTime(1) << ".\nRunning....\n";
    pthread_t sid0, sid1;
    pthread_rwlock_init(&mRwlock, NULL);
    string name;
    ServerSocket *new_sock;
    User *user;

  try
    {
        pthread_create( &sid1, NULL, scanUsers, NULL ) ;
        while ( true )
	    { 
	        new_sock = new ServerSocket();
	        mServer.accept ( *new_sock );
	        if( MAX > mClients.size() ) 
            {
	            bool isValid = true;
	            do 
                {
	                if(!isValid) 
                    {
                        *new_sock << "The nickname you have chosen is already taken. Please choose another one.";
                    }
	                *new_sock << "Nickname:";
	                name = "";
	                *new_sock >> name;
	                isValid = checkName(name);
	            } while(!isValid);
	            if(name.compare("MSG_QUIT") == 0) 
                { 
                    delete new_sock;
                    continue; 
                }
	            pthread_rwlock_wrlock(&mRwlock);
	            user = new User(new_sock, name);
	            pthread_t sid;
	            pthread_create( &sid, NULL, client, (void *)user ) ;
	            std::list<User*>::iterator i;
	            for ( i = mClients.begin(); i != mClients.end(); ++i) 
                {
	                *((**i).getSocket()) << getTime(1) + " New user " + name + " logged in.";
	            }
	            mClients.push_front(user);
                *new_sock << "Welcome to NALCG!";
	            pthread_rwlock_unlock(&mRwlock);
	        } 
            else 
            {
                *new_sock << "The server is full. Please try again later."; 
                delete new_sock; 
            }
	    }
    }
    catch ( SocketException& e )
    {

    }
    return 0;
}


