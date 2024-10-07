/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/
#include "../IUDP.hpp"
#include <algorithm>
#include <map>
#include <chrono>
#include <memory>  // For std::unique_ptr

using boost::asio::ip::udp;

#ifndef UDP_SERVER_HPP
    #define UDP_SERVER_HPP

class UDPServer : public IUDP {
    public:
        /**
        * @brief Constructs a new UDPServer object.
        * 
        * Initializes the UDP socket, starts receiving data, and configures server components.
        * 
        * @param io_context The Boost.Asio context for managing asynchronous operations.
        * @param port The port number on which the server will listen for incoming connections.
        */
        UDPServer(boost::asio::io_context& io_context, short port)
            : socket_(io_context, udp::endpoint(udp::v4(), port)), io_context_(io_context) {
            components_names = {"Position", "Velocity", "Health"};
            size_max = get_size_max();
            std::cout << "Server started on port " << port << ", size_max = " << size_max << std::endl;
            start_receive();
        }

    private:
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        boost::asio::io_context& io_context_;
        std::array<char, 1024> recv_buffer_;
        std::size_t size_max;
        std::vector<std::string> components_names;
        std::vector<udp::endpoint> client_endpoints;
        std::map<udp::endpoint, std::unique_ptr<boost::asio::steady_timer>> client_timers;
        std::map<udp::endpoint, std::unique_ptr<boost::asio::steady_timer>> pong_timers;
        std::map<udp::endpoint, bool> client_responses;
        std::map<udp::endpoint, bool> is_disconnected;

        /**
        * @brief Starts receiving data from clients. Asynchronously waits for data from any client.
        * If a new client connects, it adds the client to the list and starts a timer for managing pings.
        * This function is continuously called recursively to maintain receiving data.
        */
        void start_receive() override {
            socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                    if (!ec && bytes_recvd > 0) {
                        std::string message(recv_buffer_.data(), bytes_recvd);
                        std::cout << "Received message from client (" << remote_endpoint_.address().to_string() 
                                  << ":" << remote_endpoint_.port() << "): " << message << std::endl;

                        if (std::find(client_endpoints.begin(), client_endpoints.end(), remote_endpoint_) == client_endpoints.end()) {
                            client_endpoints.push_back(remote_endpoint_);
                            std::cout << "New client added: " << remote_endpoint_.address().to_string() 
                                      << ":" << remote_endpoint_.port() << std::endl;
                            client_responses[remote_endpoint_] = true;
                            is_disconnected[remote_endpoint_] = false;  // Initialize disconnection status.
                            checking_client(remote_endpoint_);
                        } else {
                            client_responses[remote_endpoint_] = true;
                        }

                        if (message == "pong") {
                            std::cout << "Received pong from client: " << remote_endpoint_.address().to_string() 
                                      << ":" << remote_endpoint_.port() << std::endl;
                            client_responses[remote_endpoint_] = true;
                        }
                        send_components_names();
                    }
                    start_receive();
                });
        }

        /**
        * @brief Sends the list of component names to the connected client.
        */
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

        /**
        * @brief Gets the maximum size of the component names.
        * 
        * @return The size of the longest component name.
        */
        std::size_t get_size_max() {
            auto max_component = std::max_element(components_names.begin(), components_names.end(),
                [](const std::string& a, const std::string& b) {
                    return a.size() < b.size();
                });
            return max_component != components_names.end() ? max_component->size() : 0;
        }

        /**
        * @brief Check the response status of a client.
        *
        * @param client The UDP endpoint of the client we want to check it's response status.
        */
        void checking_client(const udp::endpoint& client) {
            std::unique_ptr<boost::asio::steady_timer> timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(10)));
            timer->async_wait([this, client](const boost::system::error_code& ec) {
                if (!ec) {
                    if (!client_responses[client] && !is_disconnected[client]) {
                        std::cout << "Client did not respond to ping, disconnecting: " 
                                  << client.address().to_string() << ":" << client.port() << std::endl;
                        remove_client(client);
                    } else {
                        client_responses[client] = false;
                        send_ping(client);
                    }
                    checking_client(client);
                }
            });
            client_timers[client] = std::move(timer);
        }

        /**
        * @brief Sends a "ping" message to the specified client to check connectivity.
        * 
        * @param client The UDP endpoint of the client to which the ping is sent.
        */
        void send_ping(const udp::endpoint& client) {
            std::string ping_message = "ping";
            socket_.async_send_to(boost::asio::buffer(ping_message), client,
                [this, client](boost::system::error_code ec, std::size_t size_max) {
                    if (!ec) {
                        std::cout << "Sent ping to client: " << client.address().to_string() << ":" << client.port() << std::endl;
                        start_pong_timer(client);
                    }
                }
            );
        }

        /**
        * @brief Starts a timer waiting for the "pong" response from a client.
        * 
        * @param client The UDP endpoint of the client from which the pong message is expected.
        */
        void start_pong_timer(const udp::endpoint& client) {
            std::unique_ptr<boost::asio::steady_timer> pong_timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(2)));  // 2 seconds for "pong"
            pong_timer->async_wait([this, client](const boost::system::error_code& ec) {
                if (!ec) {
                    if (!client_responses[client] && !is_disconnected[client]) {
                        std::cout << "Client did not respond to ping (no pong), disconnecting: " 
                                  << client.address().to_string() << ":" << client.port() << std::endl;
                        remove_client(client);
                    }
                }
            });
            pong_timers[client] = std::move(pong_timer);
        }

        /**
        * @brief Removes a client from the server and notify them about it.
        * 
        * @param client The UDP endpoint of the client to be removed.
        */
        void remove_client(const udp::endpoint& client) {
            if (!is_disconnected[client]) {
                is_disconnected[client] = true;

                client_endpoints.erase(std::remove(client_endpoints.begin(), client_endpoints.end(), client), client_endpoints.end());
                client_timers.erase(client);
                pong_timers.erase(client);
                client_responses.erase(client);

                std::cout << "Client disconnected: " << client.address().to_string() << ":" << client.port() << std::endl;

                std::string deconnection_message = "You have been disconnected :(";
                socket_.async_send_to(boost::asio::buffer(deconnection_message), client,
                    [this, client](boost::system::error_code ec, std::size_t size_max) {
                        if (!ec)
                            std::cout << "Client " << client.address().to_string() << ":" << client.port() << " is now aware of their disconnection" << std::endl;
                    }
                );
            }
        }
};

#endif /* !UDP_SERVER_HPP_ */
