# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++11 -Iinclude

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib
EXAMPLE_DIR := examples
PREFIX = /usr/local
INSTALL_LIBDIR = $(PREFIX)/lib
INSTALL_INCDIR = $(PREFIX)/include/symcalc

# Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET := $(LIB_DIR)/libsymcalc.a

# Example files
EXAMPLE_SRCS := $(wildcard $(EXAMPLE_DIR)/*.cpp)
EXAMPLE_BINS := $(EXAMPLE_SRCS:$(EXAMPLE_DIR)/%.cpp=$(BIN_DIR)/%)

# Default target
all: $(TARGET) $(TEST_BINS) $(EXAMPLE_BINS)

# Build the library
$(TARGET): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build examples
$(BIN_DIR)/%: $(EXAMPLE_DIR)/%.cpp $(TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -L$(LIB_DIR) -lsymcalc -o $@


# Install the library and headers
install:
	@mkdir -p $(INSTALL_LIBDIR)
	@mkdir -p $(INSTALL_INCDIR)
	install -m 0755 $(TARGET) $(INSTALL_LIBDIR)
	install -m 0644 $(INC_DIR)/symcalc/*.hpp $(INSTALL_INCDIR)



# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

.PHONY: all clean
