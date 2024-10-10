/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/

#ifndef UDP_SERVER_HPP
    #define UDP_SERVER_HPP
    #include <algorithm>
    #include <map>
    #include <chrono>
    #include <memory>
    #include "boost/asio.hpp"
    #include "../GameEngine/GameEngine.hpp"

using boost::asio::ip::udp;

class UDPServer {
    public:
        /**
        * @brief Constructs a new UDPServer object.
        * 
        * Initializes the UDP socket, starts receiving data, and configures server components.
        * 
        * @param io_context The Boost.Asio context for managing asynchronous operations.
        * @param port The port number on which the server will listen for incoming connections.
        */
        UDPServer(boost::asio::io_context& io_context, short port, std::unordered_map<std::string, std::unique_ptr<Components::IComponent>> &components);

    private:
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        boost::asio::io_context& io_context_;
        std::array<char, 1024> recv_buffer_;
        std::size_t size_max;
        std::unordered_map<std::string, std::unique_ptr<Components::IComponent>> &__components;
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
        void start_receive();

        /**
        * @brief Sends the list of component informations to the connected client.
        */
        void send_components_infos();

        /**
        * @brief Gets the maximum size of the component names.
        * 
        * @return The size of the longest component name.
        */
        std::size_t get_size_max();

        /**
        * @brief Check the response status of a client.
        *
        * @param client The UDP endpoint of the client we want to check it's response status.
        */
        void checking_client(const udp::endpoint& client);

        /**
        * @brief Sends a "ping" message to the specified client to check connectivity.
        * 
        * @param client The UDP endpoint of the client to which the ping is sent.
        */
        void send_ping(const udp::endpoint& client);

        /**
        * @brief Starts a timer waiting for the "pong" response from a client.
        * 
        * @param client The UDP endpoint of the client from which the pong message is expected.
        */
        void start_pong_timer(const udp::endpoint& client);

        /**
        * @brief Removes a client from the server and notify them about it.
        * 
        * @param client The UDP endpoint of the client to be removed.
        */
        void remove_client(const udp::endpoint& client);
};

#endif /* !UDP_SERVER_HPP_ */
