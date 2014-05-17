CC = clang
CFLAGS = -std=c99 -Wall -Wextra -pedantic
SRC = maze.c stack.c rand.c generate.c visual.c
OBJ = $(SRC:.c=.o)
Test = "hi"

maze: $(OBJ)

test:
	$(CC) $(CFLAGS) -DTESTING=\"$(Test)\" -o maze $(SRC)

clean:
	rm -rf maze *.o

.PHONY: clean test
