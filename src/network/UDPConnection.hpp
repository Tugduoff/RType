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

/**
 * @class UDPConnection
 * 
 * @brief The UDPConnection class is used in the client to connect to a server.
 * @brief It wraps the asio function and make it easy to receive and send 
 * @brief informations to the server
 */
class UDPConnection {
    public:

        /**
         * @brief Constructor of UDPConnection
         * 
         * @param hostname : a string which represents the hostname of the server
         * @param port : a string which represents the port on which the server is running
         * 
         * @note This function connect the client to the server
         */
        UDPConnection(std::string hostname, std::string port);

        /**
         * @brief Send data to the server
         * 
         * @param data : The data to send in the form of an std::vector<uint8_t>
         */
        void send(const std::vector<uint8_t>& data);

        /**
         * @brief Send data to the server
         * 
         * @param data : The data to send in in the form of an std::string
         * 
         * @note This function transform the std::string in a std::vector<uint8_t> before sending it
         */
        void send(const std::string &data);

        /**
         * @brief Receive data from the server
         * 
         * @return std::vector<uint8_t> : the data received from the server
         * 
         * @note This function is a blocking call and will not continue until data from the server is sent
         */
        std::vector<uint8_t> blockingReceive();

        /**
         * @brief Receive data from the server
         * 
         * @return std::optional<std::vector<uint8_t>> : the potential data received from the server
         * 
         * @note This function is a non-blocking call and will return instantly an std::nullopt if no data
         * @note was sent from the server. Return an std::vector<uint8_t> otherwise
         */
        std::optional<std::vector<uint8_t>> nonBlockingReceive();

        /**
         * @brief Transform a string in vector of uint8
         * 
         * @param str : The string to be converted in std::vector<uint8_t>
         * 
         * @return std::vector<uint8_t> : the vector of uint8_t made from the string
         */
        static std::vector<uint8_t> strToBinary(const std::string& str);

        /**
         * @brief Transform a vector of uint8 in a string
         * 
         * @param data : The std::vector<uint8_t> to be converted in string
         * 
         * @return std::string : the string made from the vector of uint8_t
         */
        static std::string binaryToStr(const std::vector<uint8_t>& data);

        /**
         * @brief close the connection with the server 
         */
        void close();

    protected:
        boost::asio::io_context _io_context;
        udp::socket _socket;
        udp::endpoint _server_endpoint;
};

#endif /* !UDP_CONNECTION_HPP_ */
