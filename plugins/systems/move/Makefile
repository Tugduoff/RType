##
## EPITECH PROJECT, 2024
## RType Systems : Move
## File description:
## Move System Makefile
##

SRC	= 	./Move.cpp	\

OBJ	=	$(SRC:.cpp=.o)

NAME	=	Move.so

INCLUDES	= 	-I../../../
CPP			= 	g++
CPPFLAGS	=	-Wall -Wextra -fpic $(INCLUDES) -std=c++20

all: $(NAME)

$(NAME):	$(OBJ)
	@echo -e "\e[1;33m[RType Systems] Building $(NAME)...\e[0m"
	$(CPP) -shared -o $(NAME) $(OBJ) $(CPPFLAGS)
	@mv $(NAME) ../../bin/systems/
	@echo -e "\e[1;32m[SUCCESS] $(NAME) built\e[0m"

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f ../../bin/systems/$(NAME)

re: fclean all

.PHONY:   all re clean fclean
