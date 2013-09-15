# makefile

DBOBJS	= Database.o DBTest.o
POBJS	= Parser.o ParserTest.o
CC	= g++
DEBUG	= -g
CFLAGS	= -Wall -c --std=c++0x $(DEBUG)
LFLAGS	= -Wall --std=c++0x $(DEBUG)

all: dbtest parsertest

dbtest: $(DBOBJS)
	$(CC) $(LFLAGS) $(DBOBJS) -o dbtest

Database.o: Database.cpp Database.h
	$(CC) $(CFLAGS) Database.cpp

DBTest.o: DBTest.cpp Database.h
	$(CC) $(CFLAGS) DBTest.cpp

parsertest: $(POBJS)
	$(CC) $(LFLAGS) $(DBOBJS) -o parsertest

Parser.o: Parser.cpp Parser.h
	$(CC) $(CFLAGS) Parser.cpp

ParserTest.o: ParserTest.cpp Parser.h
	$(CC) $(CFLAGS) ParserTest.cpp

clean:
	rm *.o dbtest parsertest