CC = g++
CFLAGS = -std=c++17
SRC = src/main.cpp $(wildcard include/*.cpp)
BUILD_DIR = build
TARGET = $(BUILD_DIR)/Fix8

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf $(BUILD_DIR)

run:
	$(TARGET) 