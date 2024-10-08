/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp_client
*/

#include "UDPConnection.hpp"


int main() {
    try {
        boost::asio::io_context io_context;
        UDPConnection conn(io_context, "127.0.0.1", "8080");

        std::string message = "Hello from client!";

        conn.send(message);
        std::cout << "Sent message: " << UDPConnection::binaryToStr(UDPConnection::strToBinary(message)) << std::endl;

        while (true) {
            udp::endpoint server_endpoint;
            std::string recv_mesage = conn.blockingReceive();
            std::cout << "Received message from server: " << recv_mesage << std::endl;

            if (recv_mesage == "ping") {
                conn.send("pong");
                std::cout << "Sent message: " << "pong" << std::endl;
            }
        }

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
