CC = gcc
CFLAGS = -Wall -Iinclude

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	TARGET := minesweeper
	CFLAGS += -D_LINUX
else
	CC := x86_64-w64-mingw32-gcc
	TARGET = minesweeper.exe
	CFLAGS += -D_WINDOWS
endif

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean