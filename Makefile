# makefile

OBJS 	= Database.o DBTest.o
CC	= g++
DEBUG	= -g
CFLAGS	= -Wall -c --std=c++0x $(DEBUG)
LFLAGS	= -Wall --std=c++0x $(DEBUG)

all: dbtest

dbtest: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o dbtest

Database.o: Database.cpp Database.h
	$(CC) $(CFLAGS) Database.cpp

DBTest.o: DBTest.cpp Database.h
	$(CC) $(CFLAGS) DBTest.cpp

clean:
	rm *.o dbtest