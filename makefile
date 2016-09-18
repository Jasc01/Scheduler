CC=gcc

SOURCE= scheduler.c
OUTPUT= scheduler
OBJECT= scheduler.o
LDFLAGS= -lpthread

$(OUTPUT): $(OBJECT)
	$(CC) $(OBJECT) -o $(OUTPUT) $(LDFLAGS)

$(OBJECT): $(SOURCE)
	$(CC) -c $(SOURCE) -o scheduler.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f scheduler
