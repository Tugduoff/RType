/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp_client
*/
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::udp;

int main() {
    try {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));

        udp::resolver resolver(io_context);
        udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "127.0.0.1", "8080");

        std::string message = "Hello from client!";
        socket.send_to(boost::asio::buffer(message), *endpoints.begin());

        std::cout << "Message from server: " << message << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
