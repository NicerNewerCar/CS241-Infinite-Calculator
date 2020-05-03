CC = gcc
SRC = main
EXEC = my-calc
TEXT = in

all: $(EXEC) test-me

$(EXEC): $(SRC).o list.c
	$(CC) -g -std=c99 -o $(EXEC) $(SRC).o list.c

$(SRC).o: $(SRC).c
	$(CC) -g -std=c99 -c $(SRC).c

test-me:
	./$(EXEC) $(TEXT).txt out.txt
	diff out.txt key.txt

clean:
	rm -f $(EXEC) $(SRC).o
