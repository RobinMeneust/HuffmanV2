HEAD = $(wildcard ./include/*.h)
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
CC = gcc 
PROG=./bin/huffman

all: $(PROG) 

$(PROG) : $(OBJ)
	$(CC) $^ -o $@

obj/%.o: src/%.c $(HEAD)
	$(CC) -c $< -o $@

.PHONY : cleanlinux cleanwin doc run

cleanlinux:
	rm obj/*.o

cleanwin:
	del obj\*.o

cleandoclinux:
	del doxygen\html

cleandocwin:
	del doxygen\html

doc:
	doxygen doxygen/Doxyfile

run:
	./bin/huffman