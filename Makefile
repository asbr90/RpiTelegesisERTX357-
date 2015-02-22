CC=gcc
CFLAGS=-c  -ggdb -I/usr/local/include -I ./Middleware/inc -I ./Hardware/inc 
LDFLAGS=-L/usr/local/lib -lwiringPi
SOURCES= Hardware/src/serialDriver.c  src/main.c Middleware/src/ATParser.c Middleware/src/ETRX357.c Middleware/src/LUT.c Middleware/src/HUE.c Middleware/src/PowerSocket.c Middleware/src/DriverAPI.c
DISTDIR= bin/
ODIR=obj
ODIRMID=Middleware/
ODIRHWD=Hardware/
SDIR=src
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(ODIR)
	mkdir -p $(DISTDIR)
	mkdir -p $(ODIRMID)$(ODIR)
	mkdir -p $(ODIRHWD)$(ODIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(DISTDIR)$@
	mv $(SDIR)/*.o $(ODIR)
	mv $(ODIRMID)$(SDIR)/*.o $(ODIRMID)$(ODIR)
	mv $(ODIRHWD)$(SDIR)/*.o $(ODIRHWD)$(ODIR)
.c.o:   
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(ODIR)/*o $(DISTDIR)$(EXECUTABLE) $(ODIRHWD)$(ODIR)/*o $(ODIRMID)$(ODIR)/*o
