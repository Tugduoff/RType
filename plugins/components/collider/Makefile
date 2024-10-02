##
## EPITECH PROJECT, 2024
## RType components : Collider
## File description:
## Collider Component Makefile
##

# Compiler
CXX = g++
CXXFLAGS += --std=c++20 -Wall -Wextra $(INCLUDEFLAGS) -g -Og

# PKG_CONFIG
PKG_CONFIG_LIBS := $(shell pkg-config --libs sfml-all libconfig++)
PKG_CONFIG_CFLAGS := $(shell pkg-config --cflags sfml-all libconfig++)

# Criterion library for testing
CRITERION_LIBS = -lcriterion

SRC	= 	./Collider.cpp	\

TEST_SRCS = 	tests/Main.cpp	\
				../../../ECS/entity/EntityManager.cpp	\
				../../../ECS/system/SystemManager.cpp	\
				../../../DLLoader/DLLoader.cpp	\

OBJ	=	$(SRC:.cpp=.o)

# Test object files
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

NAME	=	Collider.so

TEST_EXEC = RType_tests

INCLUDES	= 	-I../../../
CPP			= 	g++
CPPFLAGS	=	-Wall -Wextra -Werror -fpic $(INCLUDES) -std=c++20

all: $(NAME)

$(NAME):	$(OBJ)
	@echo -e "\e[1;33m[RType Components] Building $(NAME)...\e[0m"
	$(CPP) -shared -o $(NAME) $(OBJ) $(CPPFLAGS)
	@mv $(NAME) ../../bin/components/
	@echo -e "\e[1;32m[SUCCESS] $(NAME) built\e[0m"

# Linking and compiling tests
tests: $(TEST_OBJS) $(OBJS)
	$(CXX) $(TEST_OBJS) $(OBJS) $(PKG_CONFIG_LIBS) $(CRITERION_LIBS) -o $(TEST_EXEC)

# Run tests
tests_run: tests
	./$(TEST_EXEC)

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(EXEC) $(TEST_EXEC)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:   all re clean fclean
