CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = main.exe
SRC = main.c functions.c

all: $(TARGET)

$(TARGET): $(SRC) functions.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
