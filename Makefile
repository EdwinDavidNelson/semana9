CC      = gcc
CFLAGS  = -Wall -Wextra -g -Isrc
TARGET  = backfile

SRCS = y.tab.c lex.yy.c src/ast.c src/symbols.c src/interpreter.c src/semantic.c

all: $(TARGET)

y.tab.c y.tab.h: parser.y
	yacc -d parser.y

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) -lfl

run: $(TARGET)
	./$(TARGET) tests/demo.bafle

test: $(TARGET)
	./$(TARGET) tests/test_functions.bafle

test-all: $(TARGET)
	@echo "=== Ejecutando todos los tests en tests/ ==="
	@for f in tests/*.bafle; do \
		echo "\n--- $$f ---"; \
		./$(TARGET) $$f; \
	done

clean:
	rm -f y.tab.c y.tab.h lex.yy.c $(TARGET)

.PHONY: all run test test-all clean
