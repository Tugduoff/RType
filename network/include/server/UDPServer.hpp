/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/
#include "../IUDP.hpp"

using boost::asio::ip::udp;

#ifndef UDP_SERVER_HPP
    #define UDP_SERVER_HPP

class UDPServer : public IUDP {
    public:
        UDPServer(boost::asio::io_context& io_context, short port)
            : socket_(io_context, udp::endpoint(udp::v4(), port)) {
            std::cout << "Server started on port " << port << std::endl;
            start_receive();
        }

    private:
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;

        void start_receive() override {
            socket_.async_receive_from( boost::asio::buffer(recv_buffer_), remote_endpoint_,
                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                    if (!ec && bytes_recvd > 0)
                        std::cout << "Received: " << std::string(recv_buffer_.data(), bytes_recvd) << std::endl;
                    
                    start_receive();
                });
        }
};

#endif /* !UDP_SERVER_HPP_ */
