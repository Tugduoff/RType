##
## EPITECH PROJECT, 2024
## RType components : Velocity
## File description:
## Velocity Component Makefile
##

SRC	= 	./Velocity.cpp	\

OBJ	=	$(SRC:.cpp=.o)

NAME	=	Velocity.so

INCLUDES	= 	-I../../../
CPP			= 	g++
CPPFLAGS	=	-Wall -Wextra -fpic $(INCLUDES) -std=c++20

all: $(NAME)

$(NAME):	$(OBJ)
	@echo -e "\e[1;33m[RType Components] Building $(NAME)...\e[0m"
	$(CPP) -shared -o $(NAME) $(OBJ) $(CPPFLAGS)
	@mv $(NAME) ../../bin/components/
	@echo -e "\e[1;32m[SUCCESS] $(NAME) built\e[0m"

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:   all re clean fclean
