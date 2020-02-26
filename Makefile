LIB_NAME=ledControl
SRC=$(shell find src -name "*.c")
OBJ=$(SRC:.c=.o)
CFLAGS=-Iinc
CC=gcc

$(LIB_NAME).a: $(OBJ)
	ar rcs $@ $<

.o: .c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-@rm -f $(LIB_NAME).a 2>/dev/null
	-@rm -f $(OBJ) 2>/dev/null
