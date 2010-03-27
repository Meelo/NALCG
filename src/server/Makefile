# Makefile for Server
CFLAGS=-lpthread
OPTS=-Wall
CC=g++
APP=NALCG-server
OBJS= ServerSocket.o Socket.o User.o ServerMain.o

all: $(OBJS)
	$(CC) $(OPTS) $(CFLAGS) $(OBJS) -o $(APP)

clean:
	rm --force $(OBJS) $(APP)

%.o: %.cpp
	$(CC) $(OPTS) $(CFLAGS) -c $< -o $@
