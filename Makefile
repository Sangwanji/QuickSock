CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
LDLIBS ?= -lws2_32
BIN_DIR := bin
SRC_DIR := src
SERVER_BIN := $(BIN_DIR)\server.exe
CLIENT_BIN := $(BIN_DIR)\client.exe

all: $(SERVER_BIN) $(CLIENT_BIN)

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(SERVER_BIN): $(SRC_DIR)/server.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

$(CLIENT_BIN): $(SRC_DIR)/client.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

clean:
	cmd /C "if exist $(SERVER_BIN) del /f /q $(SERVER_BIN) & if exist $(CLIENT_BIN) del /f /q $(CLIENT_BIN)"

.PHONY: all clean
