# Makefile for Server
CFLAGS=-lpthread
OPTS=-Wall
CC=g++ -g
APP=NALCG-server
OBJS= serversocket.o socket.o server.o user.o servermain.o

all: $(OBJS)
	$(CC) $(OPTS) $(CFLAGS) $(OBJS) -o $(APP)

clean:
	rm --force $(OBJS) $(APP)

%.o: %.cpp
	$(CC) $(OPTS) $(CFLAGS) -c $< -o $@
