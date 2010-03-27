#ifndef SocketException_class
#define SocketException_class

#include <string>

class SocketException
{
    public:
      SocketException ( std::string s ) : mMsg ( s ) {};
      ~SocketException (){};

      std::string description() { return mMsg; }

    private:
        std::string mMsg;
};

#endif
