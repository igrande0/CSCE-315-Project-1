# makefile

DBOBJS	= Database.o DBTest.o
POBJS	= Parser.o Database.o ParserTest.o
BOBJS = Parser.o Database.o Application.o AppMain.o
CC	= g++
DEBUG	= -g
CFLAGS	= -Wall -c --std=c++0x $(DEBUG)
LFLAGS	= -Wall --std=c++0x $(DEBUG)

all: dbtest parsertest bankapp

#-------DATABASE TEST AND DATABASE OBJECTS-------
dbtest: $(DBOBJS)
	$(CC) $(LFLAGS) $(DBOBJS) -o dbtest

Database.o: Database.cpp Database.h
	$(CC) $(CFLAGS) Database.cpp

DBTest.o: DBTest.cpp Database.h
	$(CC) $(CFLAGS) DBTest.cpp

#---------PARSER TEST AND PARSER OBJECTS---------
parsertest: $(POBJS)
	$(CC) $(LFLAGS) $(POBJS) -o parsertest

Parser.o: Parser.cpp Parser.h
	$(CC) $(CFLAGS) Parser.cpp

ParserTest.o: ParserTest.cpp Parser.h
	$(CC) $(CFLAGS) ParserTest.cpp

#--------------BANKING APPLICATION--------------
bankapp: Parser.o Database.o Application.o AppMain.o
	$(CC) $(LFLAGS) $(BOBJS) -o bankapp

Application.o: Application.cpp Application.h
	$(CC) $(CFLAGS) Application.cpp

AppMain.o: AppMain.cpp Application.h
	$(CC) $(CFLAGS) AppMain.cpp

#-----------------------------------------------
clean:
	rm *.o dbtest parsertest bankapp