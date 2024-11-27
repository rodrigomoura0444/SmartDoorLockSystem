# Makefile for Smart Door Lock System
CC = gcc
CFLAGS = -Wall -Werror

TARGET = smart_door_lock
SRC = src/smart_door_lock.c
BUILD_DIR = build

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR) data/password.txt data/access_log.txt
