# Makefile for Smart Door Lock System with OpenCV
CC = gcc
CFLAGS = -Wall -Werror `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

TARGET = smart_door_lock
SRC = src/smart_door_lock.c
BUILD_DIR = build

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/$(TARGET) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) data/password.txt data/access_log.txt
