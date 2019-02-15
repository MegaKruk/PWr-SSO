CFLAGS=

all: clienttcp clientudp server

clienttcp: clienttcp.c
	$(CC) $(CFLAGS) -o $@ $^	

clientudp: clientudp.c
	$(CC) $(CFLAGS) -o $@ $^	

server: server.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGETS)