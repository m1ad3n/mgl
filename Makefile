CC = gcc
CFLAGS = -O3 -Wall -Wextra -ggdb -Wno-uninitialized -Wno-format-extra-args
LIBS = -lGL -lGLEW -lOpenGL -lglfw

SRC_DIR = ./src
OUT_DIR = .

SOURCES = $(shell find $(SRC_DIR) -name "*.c")
HEADERS = $(shell find $(SRC_DIR) -name "*.h")

TARGET = $(OUT_DIR)/$(notdir $(shell pwd))

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LIBS)

.PHONY: clean
clean:
	@rm -rvf $(TARGET)
