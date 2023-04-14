PORT=59664
CFLAGS= -DPORT=$(PORT) -g -std=gnu99 -Wall -Werror

all: friends_server friends_client

friends.o: friends.c friends.h
	gcc $(CFLAGS) -c friends.c

friends_server.o: friends_server.c friends.h
	gcc $(CFLAGS) -c friends_server.c

friends_client.o: friends_client.c friends.h
	gcc $(CFLAGS) -c friends_client.c

friends_server: friends.o friends_server.o
	gcc $(CFLAGS) -o friends_server friends.o friends_server.o

friends_client: friends.o friends_client.o
	gcc $(CFLAGS) -o friends_client friends.o friends_client.o

clean:
	rm -f friends_server friends_client *.o
