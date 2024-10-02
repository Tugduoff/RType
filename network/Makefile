##
## EPITECH PROJECT, 2024
## RType
## File description:
## Makefile
##

BIN = udp_server udp_client clean

CXX = g++
CXXFLAGS = -std=c++11 -Wall

BOOST_INCLUDE = /usr/include
BOOST_LIB = /usr/lib

LIBS = -lboost_system

SERVER_SRC	= 	./udp_server.cpp
SERVER_OBJ	=	$(SERVER_SRC:.cpp=.o)

CLIENT_SRC	= 	./udp_client.cpp
CLIENT_OBJ	=	$(CLIENT_SRC:.cpp=.o)

all: $(BIN)

udp_server: $(SERVER_OBJ)
	@echo -e "\e[1;33m[Network - Server] Building server...\e[0m"
	$(CXX) $(CXXFLAGS) -I$(BOOST_INCLUDE) $(SERVER_OBJ) -o udp_server -L$(BOOST_LIB) $(LIBS)
	@echo -e "\e[1;32m[SUCCESS] Server built\e[0m"

udp_client: $(CLIENT_OBJ)
	@echo -e "\e[1;33m[Network - Client] Building client...\e[0m"
	$(CXX) $(CXXFLAGS) -I$(BOOST_INCLUDE) $(CLIENT_OBJ) -o udp_client -L$(BOOST_LIB) $(LIBS)
	@echo -e "\e[1;32m[SUCCESS] Client built\e[0m"

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ)

fclean: clean
	rm -f $(BIN)

re: fclean all

.PHONY:   all re clean
