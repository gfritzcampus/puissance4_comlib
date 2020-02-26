LIB_NAME=ledControl
SRC=$(shell find src -name "*.c")
OBJ=$(SRC:.c=.o)
CFLAGS=-Iinc -Werror -Wall -g -O0
CFLAGS_TEST=-Itest -DTEST_HEADER="\"stubs.h\"" -DLC_OPEN=stub_open -DLC_WRITE=stub_write -DLC_CLOSE=stub_close
CC=gcc

BIN=sendLedCommand
BIN_SRC=$(shell find bin -name "*.c")

TEST_SRC=$(shell find test -name "test_*.c")
TEST_OBJ=$(TEST_SRC:.c=.o)
TEST_EXE=$(TEST_SRC:.c=)

all: $(LIB_NAME).a sendLedCommand

sendLedCommand: $(OBJ) $(BIN_SRC)
	$(CC) $(CFLAGS) $^ -o $@

$(LIB_NAME).a: $(OBJ)
	ar rcs $@ $<

$(OBJ):%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST_EXE)

$(TEST_EXE):%:%.c $(SRC)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) $(LDFLGAS) -o $@ $^
	./$@

print-%  : ; @echo $* = $($*)

clean:
	-@rm -f $(LIB_NAME).a 2>/dev/null
	-@rm -f $(OBJ) 2>/dev/null
	-@rm -f $(TEST_EXE) 2>/dev/null
	-@rm -f $(BIN) 2>/dev/null
	-@rm -f $(BIN_SRC:.c=.o) 2>/dev/null
