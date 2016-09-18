CC=gcc

SOURCE=cliente.c
OUTPUT=cliente
OBJECT=cliente.o
LDFLAGS= -lpthread


$(OUTPUT): $(OBJECT)
	$(CC) $(OBJECT) -o $(OUTPUT) $(LDFLAGS)

$(OBJECT): $(SOURCE)
	$(CC) -c $(SOURCE) -o cliente.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f cliente
