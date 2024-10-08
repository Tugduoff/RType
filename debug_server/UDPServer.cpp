/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/
#include "UDPServer.hpp"
#include <iostream>

using boost::asio::ip::udp;

// --- PUBLIC --- //

UDPServer::UDPServer(boost::asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)), io_context_(io_context) {
    std::cout << "Server started on port " << port << ", package size_max = " << size_max << std::endl;
    start_receive();
}

// --- PRIVATE --- //

void UDPServer::start_receive() {
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
                    is_disconnected[remote_endpoint_] = false;
                    // checking_client(remote_endpoint_);
                } else {
                    client_responses[remote_endpoint_] = true;
                }

                if (message == "pong") {
                    std::cout << "Received pong from client: " << remote_endpoint_.address().to_string() 
                                << ":" << remote_endpoint_.port() << std::endl;
                    client_responses[remote_endpoint_] = true;
                }
                if (message == "start") {
                    std::cout << "Starting init of client: " << remote_endpoint_.address().to_string() 
                                << ":" << remote_endpoint_.port() << std::endl;
                    send_init_sequence();
                }
                // send_components_infos();
            }
            start_receive();
        });
}

// most significant byte first
void UDPServer::send_init_sequence() {
    // std::vector<uint8_t> initData;
    uint16_t compNb = 3;
    std::vector<uint8_t> componentNb;
    componentNb.push_back(static_cast<uint8_t>(compNb >> 8));
    componentNb.push_back(static_cast<uint8_t>(compNb & 0xFF));
    std::cout << "Sending init sequence" << std::endl;

    socket_.send_to(boost::asio::buffer(componentNb), remote_endpoint_);

    std::vector<uint8_t> maxSize = {8};

    socket_.send_to(boost::asio::buffer(maxSize), remote_endpoint_);

    std::vector<std::string> components = {"0 Position", "1 Velocity", "2 Health"};

    for (std::string name : components) {
        socket_.send_to(boost::asio::buffer(name), remote_endpoint_);
    }
}

// void UDPServer::send_components_infos() {
//     // Total components
//     socket_.async_send_to(
//         boost::asio::buffer("Total " + std::to_string(__components.size())), remote_endpoint_,
//         [this](boost::system::error_code ec, std::size_t) {
//             if (!ec)
//                 std::cout << "Total components sent to client." << std::endl;
//         }
//     );

//     // Size max
//     socket_.async_send_to(
//         boost::asio::buffer("Size " + std::to_string(size_max)), remote_endpoint_,
//         [this](boost::system::error_code ec, std::size_t size_max) {
//             if (!ec)
//                 std::cout << "Size max sent to client: " << size_max << std::endl;
//         }
//     );

//     // components
//     int index = 0;
//     for (const auto& component : __components) {
//         std::string indexed_message = std::to_string(index) + " " + component.first;
//         socket_.async_send_to(
//             boost::asio::buffer(indexed_message), remote_endpoint_,
//             [this, indexed_message](boost::system::error_code ec, std::size_t size_max) {
//                 if (!ec)
//                     std::cout << "Message sent to client: " << indexed_message << std::endl;
//             }
//         );
//         index++;
//     }
// }

// std::size_t UDPServer::get_size_max() {
//     auto max_component = std::max_element(__components.begin(), __components.end(),
//         [](const auto& a, const auto& b) {
//             return a.first.size() < b.first.size();
//         });

//     return max_component != __components.end() ? max_component->first.size() : 0;
// }

void UDPServer::checking_client(const udp::endpoint& client) {
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

void UDPServer::send_ping(const udp::endpoint& client) {
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

void UDPServer::start_pong_timer(const udp::endpoint& client) {
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

void UDPServer::remove_client(const udp::endpoint& client) {
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
