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

UDPServer::UDPServer(boost::asio::io_context& io_context, short port,
                     std::unordered_map<std::string, std::type_index> &idStringToType)
    : socket_(io_context, udp::endpoint(udp::v4(), port)), io_context_(io_context) {
    __idStringToType = idStringToType;

    size_max = get_size_max();
    std::cerr << "--- Server started on port " << port << ", package size_max = " << size_max << std::endl;
    start_receive();
}

// --- PRIVATE --- //

// --- Loop --- //

void UDPServer::start_receive() {
    boost::system::error_code ec;
    std::size_t bytes_recvd;

    if (!socket_.available()) {
        return;
    }
    bytes_recvd = socket_.receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_, 0, ec);
    if (!ec && bytes_recvd > 0) {
        std::string message(recv_buffer_.data(), bytes_recvd);
        std::cerr << "Received message from client (" << remote_endpoint_.address().to_string() 
                    << ":" << remote_endpoint_.port() << "): " << message << std::endl;

        if (std::find(client_endpoints.begin(), client_endpoints.end(), remote_endpoint_) == client_endpoints.end()) {
            if (message == "start") {
                client_endpoints.push_back(remote_endpoint_);
                std::cerr << "New client added: " << remote_endpoint_.address().to_string() 
                            << ":" << remote_endpoint_.port() << std::endl;
                client_responses[remote_endpoint_] = true;
                is_disconnected[remote_endpoint_] = false;
                checking_client(remote_endpoint_);
            }
        } else {
            client_responses[remote_endpoint_] = true;
        }

        if (message == "pong") {
            std::cerr << "Received pong from client: " << remote_endpoint_.address().to_string() 
                        << ":" << remote_endpoint_.port() << std::endl;
            client_responses[remote_endpoint_] = true;
        }
        send_components_infos();
    }
}

// --- Helpers --- //

std::size_t UDPServer::get_size_max() {
    auto max_name_length = std::max_element(__idStringToType.begin(), __idStringToType.end(),
        [](const auto& a, const auto& b) {
            return a.first.size() < b.first.size();
        });

    return max_name_length != __idStringToType.end() ? max_name_length->first.size() : 0;
}

void UDPServer::send_message(const std::vector<uint8_t>& message) {
    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_sent) {
            if (!ec) {
                std::cerr << "Sent message of size " << bytes_sent << " bytes." << std::endl;
            } else {
                std::cerr << "Error sending message: " << ec.message() << std::endl;
            }
        }
    );
}

// --- Client Init --- //

void UDPServer::send_components_infos() {
    send_total_components();
    send_size_max();
    send_components();
}

void UDPServer::send_total_components() {
    uint16_t total_components = static_cast<uint16_t>(__idStringToType.size());
    total_components = htons(total_components);

    std::vector<uint8_t> message(2);
    std::memcpy(message.data(), &total_components, sizeof(total_components));
    send_message(message);
}

void UDPServer::send_size_max() {
    std::size_t max_name_length = 0;
    for (const auto& component_name : __idStringToType)
        max_name_length = std::max(max_name_length, component_name.first.size());
    uint8_t max_name_length_byte = static_cast<uint8_t>(max_name_length);

    std::vector<uint8_t> message2(1);
    message2[0] = max_name_length_byte;
    send_message(message2);
}

void UDPServer::send_components() {
    int i = 0;
    for (const auto& component_name : __idStringToType) {
        uint8_t index = static_cast<uint8_t>(i);
        std::vector<uint8_t> buffer(1 + component_name.first.size());
        buffer[0] = index;
        std::memcpy(buffer.data() + 1, component_name.first.data(), component_name.first.size());

        send_message(buffer);
        i++;
    }
}

// --- Client Activity --- //

void UDPServer::checking_client(const udp::endpoint& client) {
    std::unique_ptr<boost::asio::steady_timer> timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(60)));
    timer->async_wait([this, client](const boost::system::error_code& ec) {
        if (!ec) {
            if (!client_responses[client] && !is_disconnected[client]) {
                std::cerr << "Client did not respond to ping, disconnecting: " 
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
        [this, client](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                std::cerr << "Sent ping to client: " << client.address().to_string() << ":" << client.port() << std::endl;
                start_pong_timer(client);
            }
        }
    );
}

void UDPServer::start_pong_timer(const udp::endpoint& client) {
    std::unique_ptr<boost::asio::steady_timer> pong_timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(10)));
    pong_timer->async_wait([this, client](const boost::system::error_code& ec) {
        if (!ec) {
            if (!client_responses[client] && !is_disconnected[client]) {
                std::cerr << "Client did not respond to ping (no pong), disconnecting: " 
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

        std::cerr << "Client disconnected: " << client.address().to_string() << ":" << client.port() << std::endl;

        std::string deconnection_message = "You have been disconnected :(";
        socket_.async_send_to(boost::asio::buffer(deconnection_message), client,
            [this, client](boost::system::error_code ec, std::size_t) {
                if (!ec)
                    std::cerr << "Client " << client.address().to_string() << ":" << client.port() << " is now aware of their disconnection" << std::endl;
            }
        );
    }
}

// --- Entity --- //

void UDPServer::create_entity(const ECS::Entity &entity) {
    uint8_t opcode = 0x0;
    uint32_t networkId = _nextNetworkId;
    _nextNetworkId++;
    _entitiesNetworkId.insert({entity, networkId});
    // uint16_t entity_id = static_cast<uint16_t>(entity);
    
    networkId = htonl(networkId);

    std::array<uint8_t, 5> message;
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, networkId](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint32_t id = ntohl(networkId);
                std::cerr << "Entity " << static_cast<int>(id) << " created." << std::endl;
            }
        }
    );
}

void UDPServer::delete_entity(const ECS::Entity &entity) {
    uint8_t opcode = 0x1;
    uint32_t networkId = _entitiesNetworkId.at(entity);

    std::cerr << "Deleted entity n°" << entity << " in local" << std::endl;
    std::cerr << "Entity network id: " << networkId << std::endl;

    networkId = htonl(networkId);

    _entitiesNetworkId.erase(entity);
    std::cerr << "Entity " << entity << " deleted. Now sending infos to the client" << std::endl;

    std::array<uint8_t, 5> message;
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, networkId](boost::system::error_code ec, std::size_t) {
            std::cerr << "Sending delete entity message to client for entity: " << networkId << std::endl;
            if (!ec) {
                uint32_t id = ntohl(networkId);
                std::cerr << "Entity " << static_cast<int>(id) << " delete." << std::endl;
            }
        }
    );
}

// --- Component --- //

void UDPServer::attach_component(size_t entity, std::type_index component) {
    uint8_t opcode = 0x2;
    uint32_t networkId = _entitiesNetworkId.at(entity);
    networkId = htonl(networkId);

    auto it = std::find_if(__idStringToType.begin(), __idStringToType.end(),
    [&component](const auto& pair) {
        return pair.second == component;
    });
    if (it == __idStringToType.end()) {
        std::cerr << "Error 0x2: Component '" << component.name() << "' not found." << std::endl;
        return;
    }

    uint8_t index = static_cast<uint8_t>(std::distance(__idStringToType.begin(), it));
    uint16_t component_id = index;
    component_id = htons(component_id);

    std::array<uint8_t, 7> message;
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));
    std::memcpy(&message[5], &component_id, sizeof(component_id));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, networkId, component_id](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t e_id = ntohl(networkId);
                uint16_t c_id = ntohs(component_id);
                std::cerr << "Attach component [" << static_cast<int>(c_id) << "] to entity [" << static_cast<int>(e_id) << "]." << std::endl;
            }
        }
    );
}

void UDPServer::update_component(size_t entity, std::string name, std::vector<uint8_t> data) {
    uint8_t opcode = 0x3;
    uint32_t networkId = _entitiesNetworkId.at(entity);
    networkId = htonl(networkId);

    auto it = std::find_if(__idStringToType.begin(), __idStringToType.end(),
    [&name](const auto& pair) {
        return pair.first == name;
    });
    if (it == __idStringToType.end()) {
        std::cerr << "Error 0x3: Component '" << name << "' not found." << std::endl;
        return;
    }

    uint8_t index = static_cast<uint8_t>(std::distance(__idStringToType.begin(), it));
    uint16_t component_id = index;
    component_id = htons(component_id);

    size_t component_size = data.size();

    std::vector<uint8_t> message(7 + component_size);
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));
    std::memcpy(&message[5], &component_id, sizeof(component_id));
    std::memcpy(&message[7], data.data(), component_size);

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, networkId, component_id](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t e_id = ntohl(networkId);
                uint16_t c_id = ntohs(component_id);
                std::cerr << "Updated component [" << static_cast<int>(c_id) << "] of entity [" << static_cast<int>(e_id) << "]." << std::endl;
            }
        }
    );
}

void UDPServer::detach_component(size_t entity, std::type_index component) {
    uint8_t opcode = 0x4;
    uint32_t networkId = _entitiesNetworkId.at(entity);
    networkId = htonl(networkId);

    auto it = std::find_if(__idStringToType.begin(), __idStringToType.end(),
    [&component](const auto& pair) {
        return pair.second == component;
    });
    if (it == __idStringToType.end()) {
        std::cerr << "Error 0x4: Component '" << component.name() << "' not found." << std::endl;
        return;
    }

    uint8_t index = static_cast<uint8_t>(std::distance(__idStringToType.begin(), it));
    uint16_t component_id = index;
    component_id = htons(component_id);

    std::array<uint8_t, 7> message;
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));
    std::memcpy(&message[5], &component_id, sizeof(component_id));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, networkId, component_id](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t e_id = ntohl(networkId);
                uint16_t c_id = ntohs(component_id);
                std::cerr << "Detach component [" << static_cast<int>(c_id) << "] from entity [" << static_cast<int>(e_id) << "]." << std::endl;
            }
        }
    );
}
