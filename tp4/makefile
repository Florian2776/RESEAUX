CFLAGS = -Wall -Wextra -Werror -std=gnu11 -pedantic -pthread
LDFLAGS = -pthread
OBJECTS = test.o 
EXE = test

all : $(EXE)

test.o : test.c 
	gcc $(CFLAGS) -c test.c 

test: test.o AddresseInternet.o
	gcc $(CFLAGS) -o test test.o AddresseInternet.o -lrt

AddresseInternet.o: AddresseInternet.c AddresseInternet.h 
	gcc $(CFLAGS) -c AddresseInternet.c

clean:
	$(RM) -f $(EXE) *.o
