CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

SRC_DIR  := src
OBJ_DIR  := build
BIN_DIR  := bin

TARGET   := $(BIN_DIR)/memsim

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS     := $(call rwildcard,$(SRC_DIR),*.cpp)

OBJS     := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
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