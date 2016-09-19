CC=gcc

LDFLAGS= -lpthread -lm

client: client.o
	$(CC) client.o -o client $(LDFLAGS)
server: server.o
	$(CC) server.o -o server $(LDFLAGS)

client.o: client.c
	$(CC) -c client.c -o client.o $(LDFLAGS)
server.o: server.c
	$(CC) -c server.c -o server.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f client
	rm -f server
	
