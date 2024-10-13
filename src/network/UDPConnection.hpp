/*
** EPITECH PROJECT, 2024
** RType
** File description:
** UDPConnection.hpp
*/

#ifndef UDP_CONNECTION_HPP_
    #define UDP_CONNECTION_HPP_
    #include <iostream>
    #include <string>
    #include <optional>
    #include "boost/asio.hpp"

using boost::asio::ip::udp;


class UDPConnection {
    public:
        UDPConnection(std::string hostname, std::string port);

        void send(const std::vector<uint8_t>& data);
        void send(const std::string &data);

        std::vector<uint8_t> blockingReceive();
        std::optional<std::vector<uint8_t>> nonBlockingReceive();

        static std::vector<uint8_t> strToBinary(const std::string& str);
        static std::string binaryToStr(const std::vector<uint8_t>& data);

        void close();

    protected:
        boost::asio::io_context _io_context;
        udp::socket _socket;
        udp::endpoint _server_endpoint;
};

#endif /* !UDP_CONNECTION_HPP_ */
