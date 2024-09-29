##
## EPITECH PROJECT, 2024
## RType TEST
## File description:
## TEST Makefile
##

# Compiler
CXX = g++
CXXFLAGS += --std=c++20 -Wall -Wextra $(INCLUDEFLAGS) -g -Og

# PKG_CONFIG
PKG_CONFIG_LIBS := $(shell pkg-config --libs sfml-all libconfig++)
PKG_CONFIG_CFLAGS := $(shell pkg-config --cflags sfml-all libconfig++)

# Criterion library for testing
CRITERION_LIBS = -lcriterion

# Source files
SRCS = 	ECS/entity/EntityManager.cpp	\
		ECS/system/SystemManager.cpp	\
		DLLoader/DLLoader.cpp	\
		GameEngine.cpp

# Main Source file
MAIN_SRC = Main.cpp	\

# Test source files
TEST_SRCS = 	tests/Main.cpp

# Include flags
INCLUDEFLAGS = -I.

# Main object file
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

# Object files
OBJS = $(SRCS:.cpp=.o)

# Test object files
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Executable names
EXEC = RType
TEST_EXEC = RType_tests

# Default target
all: $(EXEC)

# Linking main executable
$(EXEC): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(OBJS) $(MAIN_OBJ) $(PKG_CONFIG_LIBS) $(PKG_CONFIG_CFLAGS) -o $(EXEC)

# Compiling main object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking and compiling tests
tests: $(TEST_OBJS) $(OBJS)
	$(CXX) $(TEST_OBJS) $(OBJS) $(PKG_CONFIG_LIBS) $(CRITERION_LIBS) -o $(TEST_EXEC)

# Run tests
tests_run: tests
	./$(TEST_EXEC)

# Cleaning object files and executables
clean:
	rm -f $(OBJS) $(TEST_OBJS) $(EXEC) $(TEST_EXEC)

# Rebuild everything
re: clean all

# Phony targets
.PHONY: all clean re tests tests_run
