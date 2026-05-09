CC = gcc
CFLAGS = -Wall -Wextra -g

all: server client

server: server.c
	$(CC) $(CFLAGS) server.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

run-server:
	./server

run-echo:
	./client 1

run-file:
	./client 2

clean:
	rm -f server client received_file.txt