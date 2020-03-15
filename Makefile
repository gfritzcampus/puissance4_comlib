SRC=$(shell find src -name "*.c")
OBJ=$(SRC:.c=.o)
CFLAGS=-Iinc -Werror -Wall -g -O0
CC=gcc

TEST_SRC=$(shell find test -name "test_*.c")
TEST_OBJ=$(TEST_SRC:.c=.o)
TEST_EXE=$(TEST_SRC:.c=)

all: $(TEST_EXE)

$(OBJ):%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TEST_EXE):%:%.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	./$@

print-%  : ; @echo $* = $($*)

clean:
	-@rm -f $(OBJ) 2>/dev/null
	-@rm -f $(TEST_EXE) 2>/dev/null
