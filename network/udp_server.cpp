/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp_server
*/

#include "boost/asio.hpp"
#include <iostream>
#include "include/server/UDPServer.hpp"

int main() {
    try {
        boost::asio::io_context io_context;
        UDPServer server(io_context, 8080);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
