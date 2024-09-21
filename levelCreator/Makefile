##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile for bsRtype project
##

# Compiler
CXX = g++
CXXFLAGS += --std=c++20 -Wall -Wextra $(INCLUDEFLAGS)

# PKG_CONFIG
PKG_CONFIG_LIBS := $(shell pkg-config --libs sfml-all)
PKG_CONFIG_CFLAGS := $(shell pkg-config --cflags sfml-all)

# Source files
SRCS = 	Main.cpp				\
		Window.cpp				\
		UIButton.cpp			\
		Game.cpp				\
		MainMenuUI.cpp			\
		AUI.cpp					\
		UISprite.cpp			\
		LevelEditorUI.cpp		\
		LevelEditorScene.cpp	\
		UISlider.cpp			\
		UIInputField.cpp		\

# Include flags
INCLUDEFLAGS = -I./

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = levelCreator

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
