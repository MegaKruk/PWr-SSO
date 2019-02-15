CFLAGS=-Wall -g -std=c99 

all: client server

client: client.c
	$(CC) $(CFLAGS) -o $@ $^	

server: server.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGETS)