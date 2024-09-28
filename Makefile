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

# Source files
SRCS = 	Main.cpp	\
		ECS/entity/EntityManager.cpp	\
		ECS/system/SystemManager.cpp	\
		DLLoader/DLLoader.cpp	\

# Include flags
INCLUDEFLAGS = -I.

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = RType

# Default target
all: $(EXEC)

# Linking
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(PKG_CONFIG_LIBS) $(PKG_CONFIG_CFLAGS) -o $(EXEC)

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleaning
clean:
	rm -f $(OBJS) $(EXEC)

# Re
re: clean all

# Phony targets
.PHONY: all clean
