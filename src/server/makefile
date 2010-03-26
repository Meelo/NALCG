# Makefile for Server

server_objects = ServerSocket.o Socket.o User.o ServerMain.o

all : server

server: $(server_objects)
	g++ -o server $(server_objects) -lpthread

Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ServerMain: ServerMain.cpp
User: User.cpp

clean:
	-rm -f *.o

