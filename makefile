CC = gcc
CFLAGS = -ansi -Wall -pedantic -O2
LDFLAGS = -lm

myproject: assembler.o globals.o hashtable.o label.o macro.o
	$(CC) $^ $(LDFLAGS) -o $@

assembler.o: assembler.c globals.h assembler.h hashtable.h
	$(CC) $(CFLAGS) -c $< -o $@

globals.o: globals.c globals.h
	$(CC) $(CFLAGS) -c $< -o $@

hashtable.o: hashtable.c assembler.h hashtable.h
	$(CC) $(CFLAGS) -c $< -o $@

label.o: label.c label.h
	$(CC) $(CFLAGS) -c $< -o $@

macro.o: macro.c macro.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o myproject
