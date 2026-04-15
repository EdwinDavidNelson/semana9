CC      = gcc
CFLAGS  = -Wall -Wextra -g -Isrc
TARGET  = backfile

SRCS = y.tab.c lex.yy.c src/ast.c src/symbols.c src/interpreter.c

all: $(TARGET)

y.tab.c y.tab.h: parser.y
	yacc -d parser.y

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) -lfl

run: $(TARGET)
	./$(TARGET) demo.bafle

test: $(TARGET)
	./$(TARGET) test_functions.bafle

clean:
	rm -f y.tab.c y.tab.h lex.yy.c $(TARGET)

.PHONY: all run test clean
