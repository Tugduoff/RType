/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp_client
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
        UDPConnection(boost::asio::io_context& io_context, std::string hostname, std::string port)
            : _socket(io_context, udp::endpoint(udp::v4(), 0))
        {
            udp::resolver resolver(io_context);
            udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), hostname, port);
            _server_endpoint = *endpoints.begin();
        }

        void send(const std::vector<uint8_t>& data) {
            _socket.send_to(boost::asio::buffer(data), _server_endpoint);
        }

        void send(const std::string &data) {
            _socket.send_to(boost::asio::buffer(strToBinary(data)), _server_endpoint);
        }

        std::string blockingReceive() {
            char recv_buffer[1024];
            udp::endpoint sender_endpoint;
            size_t len = 0;

            while (sender_endpoint != _server_endpoint) {
                len = _socket.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);
            }
            std::string message(recv_buffer, len);
            return message;
        }

        std::optional<std::string> nonBlockingReceive() {
            char recv_buffer[1024];
            udp::endpoint sender_endpoint;

            if (_socket.available() > 0) {
                size_t len = _socket.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);

                if (sender_endpoint == _server_endpoint) {
                    std::string message(recv_buffer, len);
                    return message;
                } else {
                    std::cerr << "Data received from another host, ignoring..." << std::endl;
                    return std::nullopt;
                }
            } else {
                return std::nullopt;
            }
        }

        static std::vector<uint8_t> strToBinary(const std::string& str) {
            return std::vector<uint8_t>(str.begin(), str.end());
        }

        static std::string binaryToStr(const std::vector<uint8_t>& data) {
            return std::string(data.begin(), data.end());
        }

        void close() {
            boost::system::error_code ec;
            _socket.close(ec);
            if (ec) {
                std::cerr << "Error closing socket: " << ec.message() << std::endl;
            } else {
                std::cout << "Socket closed successfully." << std::endl;
            }
        }

    protected:
        udp::socket _socket;
        udp::endpoint _server_endpoint;
};

#endif /* !UDP_CONNECTION_HPP_ */
