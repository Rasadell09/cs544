#Yunfan Li, Zhicheng Fu, Xiang Li
#CS544-001
#Concurrency #3
CC = gcc
DEBUG = -g
CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement
OSUFFIX = -lrt -pthread
OBJ1 = concurrency.o
PROG1 = concurrency
SRC1 = concurrency.c
#OBJ2 = mt19937ar.o
#SRC2 = mt19937ar.c

all: $(PROG1)
	rm -f makefile~ *.c~

$(PROG1): $(OBJ1) $(OBJ2)
	$(CC) -o $@ $^ $(OSUFFIX)

$(OBJ1): $(SRC1)
	$(CC) $(CFLAGS) -c $<

#$(OBJ2): $(SRC2)
#	$(CC) $(CFLAGS) -c $<

clean:
	rm -f makefile~ *.o *.c~ $(PROG1)