CC=gcc
CFLAGS=-c  -Wall -ggdb -I/usr/local/include -I ./Middleware/inc -I ./Hardware/inc 
LDFLAGS=-L/usr/local/lib -lwiringPi
SOURCES= Hardware/src/serialDriver.c  src/main.c Middleware/src/ATParser.c Middleware/src/ETRX357.c Middleware/src/LUT.c Middleware/src/PhilipsHUE.c
DISTDIR= bin/
ODIR=obj
SDIR=src
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(ODIR)
	mkdir -p $(DISTDIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(DISTDIR)$@
	mv $(SDIR)/*.o $(ODIR)
	
.c.o:   
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(ODIR)/*o $(DISTDIR)/$(EXECUTABLE)
