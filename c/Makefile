CC=gcc
CFLAGS = -Wall -g -pthread
OBJ = server.o client.o

all: client server

client: client.c
	$(CC) $(CFLAGS) -o client client.c

server: server.c
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm server client
