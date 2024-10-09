/*
** EPITECH PROJECT, 2024
** RType
** File description:
** UDPConnection
*/

#include "UDPConnection.hpp"

UDPConnection::UDPConnection(std::string hostname, std::string port)
    : _socket(_io_context, udp::endpoint(udp::v4(), 0))
{
    udp::resolver resolver(_io_context);
    udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), hostname, port);
    _server_endpoint = *endpoints.begin();
}

void UDPConnection::send(const std::vector<uint8_t>& data)
{
    _socket.send_to(boost::asio::buffer(data), _server_endpoint);
}

void UDPConnection::send(const std::string &data)
{
    _socket.send_to(boost::asio::buffer(strToBinary(data)), _server_endpoint);
}

std::vector<uint8_t> UDPConnection::blockingReceive()
{
    std::vector<uint8_t> recv_buffer(1024);
    udp::endpoint sender_endpoint;
    size_t len = 0;

    while (sender_endpoint != _server_endpoint) {
        len = _socket.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);
    }
    recv_buffer.resize(len);

    return recv_buffer;
}


std::optional<std::vector<uint8_t>> UDPConnection::nonBlockingReceive()
{
    std::vector<uint8_t> recv_buffer(1024);
    udp::endpoint sender_endpoint;

    if (_socket.available() > 0) {
        size_t len = _socket.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);

        if (sender_endpoint == _server_endpoint) {
            recv_buffer.resize(len);
            return recv_buffer;
        } else {
            std::cerr << "Data received from another host, ignoring..." << std::endl;
            return std::nullopt;
        }
    } else {
        return std::nullopt;
    }
}

std::vector<uint8_t> UDPConnection::strToBinary(const std::string& str)
{
    return std::vector<uint8_t>(str.begin(), str.end());
}

std::string UDPConnection::binaryToStr(const std::vector<uint8_t>& data)
{
    return std::string(data.begin(), data.end());
}

void UDPConnection::close()
{
    boost::system::error_code ec;
    _socket.close(ec);
    if (ec) {
        std::cerr << "Error closing socket: " << ec.message() << std::endl;
    } else {
        std::cout << "Socket closed successfully." << std::endl;
    }
}
