CC=gcc
CFLAGS = -Wall -g -pthread
OBJ = router.o client.o

all: client router

client: client.c
	$(CC) $(CFLAGS) -o client client.c

router: router.c
	$(CC) $(CFLAGS) -o router router.c

clean:
	rm router client
