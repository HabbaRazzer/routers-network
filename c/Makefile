CC=gcc
CFLAGS = -std=c99 -Wall -g -pthread
OBJ = router.o client.o

all: client router

client: client.c router_funcs.c
	$(CC) $(CFLAGS) -o client client.c router_funcs.c

router: router.c router_funcs.c
	$(CC) $(CFLAGS) -o router router.c router_funcs.c

clean:
	rm router client
