CC=gcc
CFLAGS=-c  -Wall -ggdb -I/usr/local/include 
LDFLAGS=-L/usr/local/lib -lwiringPi
SOURCES= src/serialDriver.c  src/main.c src/ATParser.c src/ETRX357.c src/LUT.c src/PhilipsHUE.c
DISTDIR= bin/
ODIR=obj
SDIR=src
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(DISTDIR)$@
	mv $(SDIR)/*.o obj
.c.o:   
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(ODIR)/*o bin/$(EXECUTABLE)
