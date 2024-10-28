TARGET = cli-lib-example

SRC_DIR = src
INC_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c)

CC = gcc
CFLAGS = -std=c99 -I$(INC_DIR)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
