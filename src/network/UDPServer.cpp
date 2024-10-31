/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/
#include "UDPServer.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <typeindex>
#include <utility>

using boost::asio::ip::udp;

// --- PUBLIC --- //

UDPServer::UDPServer(
    boost::asio::io_context& io_context,
    short port,
    std::unordered_map<std::string, std::type_index> &idStringToType,
    std::function<const std::vector<ECS::Entity> &()> entityLister
)
:   socket_(io_context, udp::endpoint(udp::v4(),port)),
    io_context_(io_context),
    _listEntities(std::move(entityLister))
{
    this->updateIdStringToType(idStringToType);

    std::cerr << "--- Server started on port " << port << ", package size_max = " << size_max << std::endl;
}

// --- PRIVATE --- //

// --- Loop --- //

void UDPServer::start_receive(Engine::GameEngine &engine) {
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
                __add_new_client();
            }
        } else {
            client_responses[remote_endpoint_] = true;
        }
        std::vector<uint8_t> mes(recv_buffer_.begin(), recv_buffer_.begin() + bytes_recvd);
        if (mes[0] == 0x3) {
            std::cout << "Received input from client : " << message << std::endl;
            std::cout << "Message array size : " << message.size() << std::endl;
            std::cout << "Uint8 vector size : " << mes.size() << std::endl;
            std::cout << "Uint8 vector : ";
            for (const auto &byte : mes) {
                std::cout << "0x" << std::hex << int(byte) << " ";
            }
            std::cout << std::endl;
            std::cout << "message array : ";
            for (const auto &car : message) {
                std::cout << "0x" << std::hex << int(car) << " ";
            }
            std::cout << std::dec << std::endl;

            receiveUpdateComponent(engine, mes);
        }

        if (message == "pong") {
            std::cerr << "Received pong from client: " << remote_endpoint_.address().to_string() 
                        << ":" << remote_endpoint_.port() << std::endl;
            client_responses[remote_endpoint_] = true;
        }
    }
}

// --- Helpers --- //

std::size_t UDPServer::__get_size_max() {
    auto max_name_length = std::max_element(_comps_info.begin(), _comps_info.end(),
        [](const auto& a, const auto& b) {
            return a.second.name.size() < b.second.name.size();
        }
    );

    return max_name_length != _comps_info.end() ? max_name_length->second.name.size() : 0;
}

void UDPServer::__send_message(const std::span<const uint8_t>& message) {
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

void UDPServer::__add_new_client()
{
    client_endpoints.push_back(remote_endpoint_);
    std::cerr << "New client added: " << remote_endpoint_.address().to_string() 
                << ":" << remote_endpoint_.port() << std::endl;
    client_responses[remote_endpoint_] = true;
    is_disconnected[remote_endpoint_] = false;
    // __checking_client(remote_endpoint_);
    __send_components_infos();
    for (const auto &e : _listEntities()) {
        __send_entity_created_message(
            _entitiesNetworkId.at(e),
            remote_endpoint_
        );
    }
}

// --- Client Init --- //

void UDPServer::__send_components_infos() {
    __send_total_components();
    __send_size_max();
    __send_components();
}

void UDPServer::__send_total_components() {
    uint16_t total_components = static_cast<uint16_t>(_comps_info.size());
    total_components = htons(total_components);

    std::vector<uint8_t> message(2);
    std::memcpy(message.data(), &total_components, sizeof(total_components));
    __send_message(message);
}

void UDPServer::__send_size_max() {
    std::size_t max_name_length = 0;
    for (const auto& component_name : __idStringToType)
        max_name_length = std::max(max_name_length, component_name.first.size());
    uint8_t max_name_length_byte = static_cast<uint8_t>(max_name_length);

    std::vector<uint8_t> message2(2);
    *reinterpret_cast<uint16_t *>(&message2[0]) = max_name_length_byte;
    __send_message(message2);
}

void UDPServer::__send_components() {
    for (const auto &[_, info] : _comps_info) {
        const auto &[name, index] = info;
        std::vector<uint8_t> buffer(2 + name.size());
        *reinterpret_cast<uint16_t *>(&buffer[0]) = index;
        std::memcpy(buffer.data() + 2, name.data(), name.size());

        __send_message(buffer);
    }
}

// --- Client Activity --- //

void UDPServer::__checking_client(const udp::endpoint& client) {
    std::unique_ptr<boost::asio::steady_timer> timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(60)));
    timer->async_wait([this, client](const boost::system::error_code& ec) {
        if (!ec) {
            if (!client_responses[client] && !is_disconnected[client]) {
                std::cerr << "Client did not respond to ping, disconnecting: " 
                            << client.address().to_string() << ":" << client.port() << std::endl;
                __remove_client(client);
            } else {
                client_responses[client] = false;
                __send_ping(client);
            }
            __checking_client(client);
        }
    });
    client_timers[client] = std::move(timer);
}

void UDPServer::__send_ping(const udp::endpoint& client) {
    std::string ping_message = "ping";
    socket_.async_send_to(boost::asio::buffer(ping_message), client,
        [this, client](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                std::cerr << "Sent ping to client: " << client.address().to_string() << ":" << client.port() << std::endl;
                __start_pong_timer(client);
            }
        }
    );
}

void UDPServer::__start_pong_timer(const udp::endpoint& client) {
    std::unique_ptr<boost::asio::steady_timer> pong_timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(10)));
    pong_timer->async_wait([this, client](const boost::system::error_code& ec) {
        if (!ec) {
            if (!client_responses[client] && !is_disconnected[client]) {
                std::cerr << "Client did not respond to ping (no pong), disconnecting: " 
                            << client.address().to_string() << ":" << client.port() << std::endl;
                __remove_client(client);
            }
        }
    });
    pong_timers[client] = std::move(pong_timer);
}

void UDPServer::__remove_client(const udp::endpoint& client) {
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
    uint32_t networkId = _nextNetworkId;
    _nextNetworkId++;
    _entitiesNetworkId[entity] = networkId;

    __send_entity_created_message(networkId, remote_endpoint_);
}

void UDPServer::__send_entity_created_message(
    uint32_t networkId,
    const udp::endpoint &client
)
{
    uint8_t opcode = 0x0;
    networkId = htonl(networkId);

    std::array<uint8_t, 5> message;
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));

    socket_.async_send_to(
        boost::asio::buffer(message), client,
        [ networkId](boost::system::error_code ec, std::size_t) {
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

    _entitiesNetworkId.erase(entity);
    networkId = htonl(networkId);

    std::cerr << "Entity " << entity << " deleted. Now sending infos to the client" << std::endl;

    std::array<uint8_t, 5> message;
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));

    for (auto byte : message) {
        std::cerr << static_cast<int>(byte) << " ";
    }
    std::cerr << std::endl;

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, networkId](boost::system::error_code ec, std::size_t) {
            std::cerr << "Sending delete entity message to client for entity: " << ntohl(networkId) << std::endl;
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

    if (!_comps_info.contains(component)) {
        std::cerr << "Error 0x2: Component '" << component.name() << "' not found." << std::endl;
        return;
    }

    uint16_t component_id = _comps_info.at(component).networkId;
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

    if (!__idStringToType.contains(name)) {
        std::cerr << "Error 0x3: Component '" << name << "' not found." << std::endl;
        return;
    }

    const std::type_index &type = __idStringToType.at(name);

    uint16_t component_id = _comps_info.at(type).networkId;
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
                // uint16_t e_id = ntohl(networkId);
                // uint16_t c_id = ntohs(component_id);
                // std::cerr << "Updated component [" << static_cast<int>(c_id) << "] of entity [" << static_cast<int>(e_id) << "]." << std::endl;
            }
        }
    );
}

void UDPServer::detach_component(size_t entity, std::type_index component) {
    uint8_t opcode = 0x4;
    uint32_t networkId = _entitiesNetworkId.at(entity);
    networkId = htonl(networkId);

    if (!_comps_info.contains(component)) {
        std::cerr << "Error 0x4: Component '" << component.name() << "' not found." << std::endl;
        return;
    }

    uint16_t component_id = _comps_info.at(component).networkId;
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

uint16_t uint16From2Uint8(uint8_t first, uint8_t second)
{
    return static_cast<uint16_t>((first << 8) | static_cast<uint16_t>(second));
}

uint32_t uint32From4Uint8(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4)
{
    return static_cast<uint32_t>((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4);
}

void UDPServer::receiveUpdateComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));
        uint16_t componentId = uint16From2Uint8(operation[5], operation[6]);

        auto comp_info_it = std::find_if(
            _comps_info.begin(),
            _comps_info.end(),
            [componentId](const std::pair<std::type_index, UDPServer::ComponentInfo> &info) {
                return info.second.networkId == componentId;
            }
        );

        if (comp_info_it == _comps_info.end()) {
            std::cout << "Error : did not find the Component string ID" << std::endl;
            return;
        }

        const std::type_index &compTypeIndex = comp_info_it->first;

        auto &compInstance = engine.getComponentFromId(compTypeIndex);
        auto &sparseArray = compInstance->any_cast(
            engine.getRegistry().componentManager().getComponents(compTypeIndex)
        );
        std::vector<uint8_t> serializedData = std::vector<uint8_t>(operation.begin() + 7, operation.end());

        try {
            sparseArray[entity]->deserialize(serializedData);
        } catch (std::exception &e) {
            std::cerr << "\033[0;35m";
            std::cerr << "Component " << comp_info_it->second.name << " was not attached for entity n°" << entity << std::endl;
            std::cerr << "\033[0;37m";

            return;
        }
    }
    catch(const std::exception &) {
    }    
}
