CC = g++
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
BIN_DIR = bin
SRC = $(wildcard $(SRC_DIR)/*.cpp)

TARGET = $(BIN_DIR)/dlns
TEST = test/test.cnf

all: build run

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run:
	$(TARGET) $(TEST)

clean:
	rm -f $(TARGET)

.PHONY: all clean