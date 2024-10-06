/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/
#include <algorithm>
#include <chrono>
#include <map>
#include "../IUDP.hpp"

using boost::asio::ip::udp;

#ifndef UDP_SERVER_HPP
    #define UDP_SERVER_HPP

class UDPServer : public IUDP {
    public:
        UDPServer(boost::asio::io_context& io_context, short port)
            : socket_(io_context, udp::endpoint(udp::v4(), port)), io_context_(io_context) {
            std::cout << "Server started on port " << port << std::endl;
            components_names = {"Position", "Velocity", "Health"};
            size_max = get_size_max();
            start_receive();
        }

    private:
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        boost::asio::io_context& io_context_;
        std::array<char, 1024> recv_buffer_;
        std::vector<udp::endpoint> client_endpoints;
        std::map<udp::endpoint, std::unique_ptr<boost::asio::steady_timer>> client_timers;
        std::size_t size_max;
        std::vector<std::string> components_names;

        void start_receive() override {
            socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                    if (!ec && bytes_recvd > 0) {
                        std::cout << "Client connected: " << remote_endpoint_ << std::endl;

                        // if first connexion
                        if (std::find(client_endpoints.begin(), client_endpoints.end(), remote_endpoint_) == client_endpoints.end()) {
                            client_endpoints.push_back(remote_endpoint_);
                            std::cout << "New client added: " << remote_endpoint_ << std::endl;
                            send_components_names();
                            start_timer(remote_endpoint_);
                        } else {
                            reset_timer(remote_endpoint_);
                        }

                        std::string message(recv_buffer_.data(), bytes_recvd);
                        std::cout << "Received message from client (" << remote_endpoint_ << "): " << message << std::endl;

                        if (message == "pong")
                            std::cout << "Received pong from client: " << remote_endpoint_ << std::endl;
                    }
                    start_receive();
                });
        }

        void send_components_names() {
            for (const auto& message : components_names) {
                socket_.async_send_to(
                    boost::asio::buffer(message), remote_endpoint_,
                    [this, message](boost::system::error_code ec, std::size_t size_max) {
                        if (!ec)
                            std::cout << "Message sent to client: " << message << std::endl;
                    }
                );
            }
        }

        std::size_t get_size_max() {
            auto max_component = std::max_element(components_names.begin(), components_names.end(),
                [](const std::string& a, const std::string& b) {
                    return a.size() < b.size();
                });
            return max_component != components_names.end() ? max_component->size() : 0;
        }

        void start_timer(const udp::endpoint& client) {
            std::unique_ptr<boost::asio::steady_timer> timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(10)));
            timer->async_wait([this, client](const boost::system::error_code& ec) {
                if (!ec) {
                    send_ping(client);
                }
            });
            client_timers[client] = std::move(timer);
        }

        void reset_timer(const udp::endpoint& client) {
            if (client_timers.find(client) != client_timers.end()) {
                client_timers[client]->expires_after(std::chrono::seconds(10));  // Réinitialise à 10 secondes
                client_timers[client]->async_wait([this, client](const boost::system::error_code& ec) {
                    if (!ec) {
                        send_ping(client);
                    }
                });
            }
        }

        void send_ping(const udp::endpoint& client) {
            std::string ping_message = "ping";
            socket_.async_send_to(boost::asio::buffer(ping_message), client,
                [this, client](boost::system::error_code ec, std::size_t /*bytes_sent*/) {
                    if (!ec) {
                        std::cout << "Sent ping to client: " << client << std::endl;
                    }
                }
            );
        }

        void remove_client(const udp::endpoint& client) {
            client_endpoints.erase(std::remove(client_endpoints.begin(), client_endpoints.end(), client), client_endpoints.end());
            client_timers.erase(client);
            std::cout << "Client disconnected: " << client << std::endl;
        }
};

#endif /* !UDP_SERVER_HPP_ */
