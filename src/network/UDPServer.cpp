/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/
#include "UDPServer.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <typeindex>
#include <utility>
#include <vector>
#include <ranges>

using boost::asio::ip::udp;

// --- PUBLIC --- //

UDPServer::UDPServer(
    boost::asio::io_context& io_context,
    short port,
    std::unordered_map<std::string, std::type_index> &idStringToType,
    std::function<const std::vector<ECS::Entity> &()> entityLister,
    std::function<std::vector<std::pair<std::type_index, SparseArray<Components::IComponent> &>>()> componentLister
)
:   socket_(io_context, udp::endpoint(udp::v4(),port)),
    io_context_(io_context),
    _listEntities(std::move(entityLister)),
    _listComponents(std::move(componentLister)),
    _isGameRunning(false)
{
    this->updateIdStringToType(idStringToType);
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
    if (ec || bytes_recvd == 0) {
        std::cerr << "Could not receive properly from " << remote_endpoint_ << ", ec=" << ec << ", bytes_rcvd=" << bytes_recvd << std::endl;
        return;
    }
    switch (recv_buffer_[0]) {
        case 0x0:
            // send component list to client

            // set something to indicate that we should periodically
            // send number of components to client
            break;

        case 0x1:
            // ignore one component type for client
            break;

        case 0x2:
            // Start the game: send all existing entities and
            // components to requesting client
        
            _isGameRunning = true;
            break;

        case 0x3:

            receiveUpdateComponent(engine, recv_buffer_);
            break;
    }
}

// --- Helpers --- //

void UDPServer::__send_message(const std::span<const uint8_t>& message) {
    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [](boost::system::error_code ec, std::size_t bytes_sent) {
            if (!ec) {
                std::cerr << "Sent message of size " << bytes_sent << " bytes." << std::endl;
            } else {
                std::cerr << "Error sending message: " << ec.message() << std::endl;
            }
        }
    );
}

void UDPServer::__init_new_client()
{
    _clients.try_emplace(remote_endpoint_);
    std::cerr << "New client added: " << remote_endpoint_ << std::endl;
}

void UDPServer::__add_new_client()
{
    __send_components_infos();
    for (const auto &e : _listEntities()) {
        __send_entity_created_message(
            _entitiesNetworkId.at(e),
            remote_endpoint_
        );
    }
    auto components = _listComponents();
    for (const auto &[typeIdx, compArr] : components) {
        uint16_t comp_netId = _comps_info.at(typeIdx).networkId;
        for (auto &&[i, _] : IndexedZipper(compArr)) {
            uint32_t entity_netId = _entitiesNetworkId.at(i);
            __send_attach_component_message(entity_netId, comp_netId, remote_endpoint_);
        }
    }
    for (const auto &[typeIdx, compArr] : components) {
        uint16_t comp_netId = _comps_info.at(typeIdx).networkId;
        for (auto &&[i, comp] : IndexedZipper(compArr)) {
            uint32_t entity_netId = _entitiesNetworkId.at(i);

            __send_update_component(
                entity_netId,
                comp_netId,
                comp.serialize(),
                remote_endpoint_
            );
        }
    }
}

// --- Client Init --- //

void UDPServer::__send_components_infos() {
    __send_components();
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

void UDPServer::__remove_client(const udp::endpoint& client) {
    if (!_clients.erase(client)) {
        return;
    }
    std::cerr << "Client disconnected: " << client.address().to_string() << ":" << client.port() << std::endl;

    socket_.async_send_to(boost::asio::buffer("You have been disconnected :("), client,
        [client](boost::system::error_code ec, std::size_t) {
            if (!ec)
                std::cerr << "Client " << client << " is now aware of their disconnection" << std::endl;
        }
    );
}

// --- Entity --- //

void UDPServer::create_entity(const ECS::Entity &entity) {
    uint32_t networkId = _nextNetworkId;
    _nextNetworkId++;
    _entitiesNetworkId[entity] = networkId;

    for (const auto &client_endpoint : std::views::keys(_clients)) {
        __send_entity_created_message(networkId, client_endpoint);
    }
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

    for (const auto &client_endpoint : std::views::keys(_clients)) {
        socket_.async_send_to(
            boost::asio::buffer(message), client_endpoint,
            [networkId](boost::system::error_code ec, std::size_t) {
                std::cerr << "Sending delete entity message to client for entity: " << ntohl(networkId) << std::endl;
                if (!ec) {
                    uint32_t id = ntohl(networkId);
                    std::cerr << "Entity " << static_cast<int>(id) << " delete." << std::endl;
                }
            }
        );
    }
}

// --- Component --- //

void UDPServer::attach_component(size_t entity, std::type_index component) {
    uint32_t networkId = _entitiesNetworkId.at(entity);

    if (!_comps_info.contains(component)) {
        std::cerr << "Error 0x2: Component '" << component.name() << "' not found." << std::endl;
        return;
    }

    uint16_t component_id = _comps_info.at(component).networkId;

    for (const auto &client_endpoint : std::views::keys(_clients)) {
        __send_attach_component_message(networkId, component_id, client_endpoint);
    }
}

void UDPServer::__send_attach_component_message(
    uint32_t entity_netId,
    uint16_t comp_netId,
    const udp::endpoint &client
)
{
    uint8_t opcode = 0x2;

    entity_netId = htonl(entity_netId);
    comp_netId = htons(comp_netId);
    std::array<uint8_t, 7> message;
    message[0] = opcode;
    std::memcpy(&message[1], &entity_netId, sizeof(entity_netId));
    std::memcpy(&message[5], &comp_netId, sizeof(comp_netId));

    socket_.async_send_to(
        boost::asio::buffer(message), client,
        [entity_netId, comp_netId](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t e_id = ntohl(entity_netId);
                uint16_t c_id = ntohs(comp_netId);
                std::cerr << "Attach component [" << static_cast<int>(c_id) << "] to entity [" << static_cast<int>(e_id) << "]." << std::endl;
            }
        }
    );
}

void UDPServer::update_component(size_t entity, std::string name, std::vector<uint8_t> data) {
    uint32_t networkId = _entitiesNetworkId.at(entity);

    if (!__idStringToType.contains(name)) {
        std::cerr << "Error 0x3: Component '" << name << "' not found." << std::endl;
        return;
    }

    const std::type_index &type = __idStringToType.at(name);

    uint16_t component_id = _comps_info.at(type).networkId;

    for (const auto &client_endpoint : std::views::keys(_clients)) {
        __send_update_component(
            networkId,
            component_id,
            data,
            client_endpoint
        );
    }
}

void UDPServer::__send_update_component(
    uint32_t entity_netId,
    uint16_t comp_netId,
    const std::vector<uint8_t> &data,
    const udp::endpoint &client
)
{
    uint8_t opcode = 0x3;

    entity_netId = htonl(entity_netId);
    comp_netId = htons(comp_netId);
    size_t component_size = data.size();

    std::vector<uint8_t> message(7 + component_size);
    message[0] = opcode;
    std::memcpy(&message[1], &entity_netId, sizeof(entity_netId));
    std::memcpy(&message[5], &comp_netId, sizeof(comp_netId));
    std::memcpy(&message[7], data.data(), component_size);

    socket_.async_send_to(
        boost::asio::buffer(message), client,
        [](boost::system::error_code ec, std::size_t) {
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

    for (const auto &client_endpoint : std::views::keys(_clients)) {
        socket_.async_send_to(
            boost::asio::buffer(message), client_endpoint,
            [networkId, component_id](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    uint16_t e_id = ntohl(networkId);
                    uint16_t c_id = ntohs(component_id);
                    std::cerr << "Detach component [" << static_cast<int>(c_id) << "] from entity [" << static_cast<int>(e_id) << "]." << std::endl;
                }
            }
        );
    }
}

uint16_t uint16From2Uint8(uint8_t first, uint8_t second)
{
    return static_cast<uint16_t>((first << 8) | static_cast<uint16_t>(second));
}

uint32_t uint32From4Uint8(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4)
{
    return static_cast<uint32_t>((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4);
}

void UDPServer::receiveUpdateComponent(Engine::GameEngine &engine, std::span<const uint8_t> operation)
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

void UDPServer::sendNextFrame()
{
    std::vector<uint8_t> data = {0xa};
    for (const auto &client_endpoint : std::views::keys(_clients)) {
        socket_.async_send_to(
            boost::asio::buffer(data), client_endpoint,
            [](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                }
            }
        );
    }
}

bool UDPServer::gameRunning() const noexcept
{
    return _isGameRunning;
}
