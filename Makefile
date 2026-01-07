CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

SRC_DIR  := src
OBJ_DIR  := build
BIN_DIR  := bin

TARGET   := $(BIN_DIR)/memsim

SRCS     := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS     := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $@"
	@$(CXX) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	@./$(TARGET)

.PHONY: all clean run