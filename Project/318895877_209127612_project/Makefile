CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -lm
TARGET = symnmf
SOURCES = symnmf.c   

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
