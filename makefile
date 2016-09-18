CC=gcc

SOURCE=thread.c
OUTPUT=thread
OBJECT=thread.o
LDFLAGS= -lpthread


$(OUTPUT): $(OBJECT)
	$(CC) $(OBJECT) -o $(OUTPUT) $(LDFLAGS)

$(OBJECT): $(SOURCE)
	$(CC) -c $(SOURCE) -o thread.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f thread
