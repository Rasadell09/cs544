#Yunfan Li, Zhicheng Fu, Xiang Li
#CS544-001
#Concurrency #5

CC = gcc
DEBUG = -g
CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement
OSUFFIX = -lrt -pthread
OBJ1 = c51.o
PROG1 = c51
SRC1 = c51.c
OBJ2 = c52.o
SRC2 = c52.c
PROG2 = c52

all: $(PROG1) $(PROG2)
	rm -f makefile~ *.c~

$(PROG1): $(OBJ1) #$(OBJ2)
	$(CC) -o $@ $^ $(OSUFFIX)

$(PROG2): $(OBJ2)
	$(CC) -o $@ $^ $(OSUFFIX)

$(OBJ1): $(SRC1)
	$(CC) $(CFLAGS) -c $<

$(OBJ2): $(SRC2)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f makefile~ *.o *.c~ $(PROG1) $(PROG2)