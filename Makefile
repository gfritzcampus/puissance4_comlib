SRC=$(shell find src -name "*.c")
OBJ=$(SRC:.c=.o)
CFLAGS=-Iinc -Werror -Wall -g -O0
CC=gcc

LIBNAME=libpuissance4.a

TEST_SRC=$(shell find test -name "test_*.c")
TEST_OBJ=$(TEST_SRC:.c=.o)
TEST_EXE=$(TEST_SRC:.c=)
TEST_RUN=$(addprefix run-,$(notdir $(TEST_EXE)))

all: $(LIBNAME) test

$(LIBNAME): $(OBJ)
	ar rcs $@ $+

$(OBJ):src/%.o:inc/%.h
$(OBJ): inc/common.h
$(filter %_receiver.o,$(OBJ)): inc/receiver.h
$(TEST_OBJ): inc/common.h test/test.h

$(TEST_EXE):test/test_%:test/test_%.o src/%.o
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_RUN):run-%:test/%
	@./$^
	-@if ! grep -qe "$^" .gitignore; then echo "$^" >> .gitignore; fi

test: $(TEST_EXE)
	make $(addprefix run-,$(notdir $?))
	touch test

doc:
	doxygen Doxyfile

.PHONY: clean doc

clean:
	-@rm -f $(OBJ) 2>/dev/null
	-@rm -f $(TEST_EXE) 2>/dev/null
	-@rm -f test/*.o 2>/dev/null
	-@rm -f src/*.o 2>/dev/null
