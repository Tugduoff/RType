// /*
// ** EPITECH PROJECT, 2024
// ** RType
// ** File description:
// ** udp_client
// */

// #include "boost/asio.hpp"
// #include <iostream>

// using boost::asio::ip::udp;

// int main() {
//     try {
//         boost::asio::io_context io_context;
//         udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));

//         udp::resolver resolver(io_context);
//         udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "127.0.0.1", "8080");

//         std::string message = "Hello from client!";
//         std::array<char, 1024> recv_buffer;

//         socket.send_to(boost::asio::buffer(message), *endpoints.begin());
//         std::cout << "Message sent to server: " << message << std::endl;

//         while (true) {
//             // if (message == "exit")
//             //     break;
//             // socket.send_to(boost::asio::buffer(message), *endpoints.begin());
//             // std::cout << "Message sent to server: " << message << std::endl;

//             // Wait for server's response (if any)
//             udp::endpoint server_endpoint;
//             std::size_t len = socket.receive_from(boost::asio::buffer(recv_buffer), server_endpoint);

//             std::cout << "Received from server: " << std::string(recv_buffer.data(), len) << std::endl;
//             // if (std::string(recv_buffer.data(), len) == "ping") {
//             //     socket.send_to(boost::asio::buffer("pong"), *endpoints.begin());
//             //     std::cout << "Message sent to server: " << "pong" << std::endl;
//             // }
//         }

//     } catch (std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 84;
//     }
//     return 0;
// }
/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp_client
*/

#include "boost/asio.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using boost::asio::ip::udp;

// Fonction pour convertir 2 octets big-endian en uint16_t
uint16_t ntohs_uint16(const std::array<char, 1024>& buffer, std::size_t offset) {
    return (static_cast<uint16_t>(static_cast<uint8_t>(buffer[offset])) << 8) |
           static_cast<uint16_t>(static_cast<uint8_t>(buffer[offset + 1]));
}

int main() {
    try {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));

        udp::resolver resolver(io_context);
        udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "127.0.0.1", "8080");

        std::string message = "Hello from client!";
        std::array<char, 1024> recv_buffer;

        // Envoi du message initial au serveur
        socket.send_to(boost::asio::buffer(message), *endpoints.begin());
        std::cout << "Message sent to server: " << message << std::endl;

        // Step 1: Recevoir le nombre total de composants (2 octets, big-endian)
        udp::endpoint server_endpoint;
        std::size_t len = socket.receive_from(boost::asio::buffer(recv_buffer), server_endpoint);
        uint16_t total_components = ntohs_uint16(recv_buffer, 0); // Conversion big-endian -> host-endian
        std::cout << "Total components received: " << total_components << std::endl;

        // Step 2: Recevoir la taille maximale des noms de composants (1 octet)
        len = socket.receive_from(boost::asio::buffer(recv_buffer), server_endpoint);
        uint8_t max_name_length = static_cast<uint8_t>(recv_buffer[0]);
        std::cout << "Max component name length: " << static_cast<int>(max_name_length) << std::endl;

        // Step 3: Recevoir chaque nom de composant
        std::vector<std::string> component_names;
        for (uint16_t i = 0; i < total_components; ++i) {
            len = socket.receive_from(boost::asio::buffer(recv_buffer), server_endpoint);
            std::string component_name(recv_buffer.data(), len);
            component_names.push_back(component_name);
            std::cout << "Component name received: " << component_name << std::endl;
        }

        // Step 4: Recevoir l'indicateur de fin (0xffff)
        len = socket.receive_from(boost::asio::buffer(recv_buffer), server_endpoint);
        uint16_t end_indicator = ntohs_uint16(recv_buffer, 0);

        if (end_indicator == 0xffff) {
            std::cout << "End indicator received: 0xffff" << std::endl;
        } else {
            std::cerr << "Error: Invalid end indicator received." << std::endl;
        }

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
