export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:libx86-64
CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -fPIC 
LDLIBS = -lAdresseInternet
LDFLAGS = -Llibx86-64
PRGS = libAdresseInternet.so 

all: $(PRGS)

libAdresseInternet.so: AdresseInternet.o
	$(CC) -shared $(CFLAGS) $^ -o $@

AdresseInternet.o: AdresseInternet.c AdresseInternet.h AdresseInternetType.h

install: libAdresseInternet.so
	mv libAdresseInternet.so  libx86-64


clean:
	$(RM) -r *~ *.o $(PRGS) *.aux *.log *.synctex.gz auto 

dist-clean: clean
	$(RM) -rv libx86-64/* $(PRGS)

