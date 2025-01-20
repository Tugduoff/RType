/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/
#include "UDPServer.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "GameEngine/ComponentsGetter.hpp"
#include "boost/asio/detail/chrono.hpp"
#include "boost/asio/steady_timer.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <typeindex>
#include <utility>
#include <vector>
#include <ranges>
#include "components/position/Position.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/collider/Collider.hpp"
#include "components/damage/Damage.hpp"
#include "components/type/Type.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/acceleration/Acceleration.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "components/controllable/Controllable.hpp"
#include "components/gun/Gun.hpp"
#include "components/scale/Scale.hpp"

using boost::asio::ip::udp;

// --- PUBLIC --- //

UDPServer::UDPServer(
    boost::asio::io_context& io_context,
    short port,
    std::unordered_map<std::string, std::type_index> &idStringToType,
    std::function<const std::vector<ECS::Entity> &()> entityLister,
    ComponentsGetter compGetter
)
:   socket_(io_context, udp::endpoint(udp::v4(),port)),
    io_context_(io_context),
    _listEntities(std::move(entityLister)),
    _compGetter(std::move(compGetter)),
    _isGameRunning(false)
{
    this->updateIdStringToType(idStringToType);
}

// --- PRIVATE --- //

// --- Loop --- //

void UDPServer::createNewEntity(Engine::GameEngine &engine)
{
    lockMutex();
    ECS::Entity entity = engine.getRegistry().createEntity();
    std::cerr << "New entity created with ID: " << entity << std::endl;

    srand(time(NULL));
    int posY = rand() % 1080;

    attachAndUpdateComponent<Components::Position>(engine, entity, 50, posY, 2);
    attachAndUpdateComponent<Components::Velocity>(engine, entity, 0, 0, 100);
    attachAndUpdateComponent<Components::Collider>(engine, entity, 30, 30);
    attachAndUpdateComponent<Components::Damage>(engine, entity, 50);
    attachAndUpdateComponent<Components::Type>(engine, entity, Components::TypeID::ALLY);
    attachAndUpdateComponent<Components::SpriteID>(engine, entity, "player");
    attachAndUpdateComponent<Components::Acceleration>(engine, entity, -5, 5, -5, 5);
    attachAndUpdateComponent<Components::Gun>(engine, entity, 50, 500, 8, 0, "shot1");
    attachAndUpdateComponent<Components::Scale>(engine, entity, 300, 300);

    std::map<enum Action, enum Key> keyBindings = {
        {Action::FORWARD, Key::UNKNOWN},
        {Action::BACKWARD, Key::UNKNOWN},
        {Action::LEFT, Key::UNKNOWN},
        {Action::RIGHT, Key::UNKNOWN},
        {Action::ACTION1, Key::UNKNOWN},
        {Action::ACTION2, Key::UNKNOWN},
        {Action::ACTION3, Key::UNKNOWN},
        {Action::ACTION4, Key::UNKNOWN},
        {Action::ACTION5, Key::UNKNOWN},
        {Action::ACTION6, Key::UNKNOWN},
        {Action::ACTION7, Key::UNKNOWN},
        {Action::ACTION8, Key::UNKNOWN},
        {Action::ACTION9, Key::UNKNOWN},
        {Action::ACTION10, Key::UNKNOWN}
    };

    std::unique_ptr<Components::Controllable> component = std::make_unique<Components::Controllable>(keyBindings);
    std::vector<uint8_t> data = component->serialize();

    engine.getRegistry().componentManager().addComponent(entity, std::move(component));
    unlockMutex();

    uint32_t networkId = _entitiesNetworkId.at(entity);

    const std::type_index &type = __idStringToType.at("Controllable");

    uint16_t component_id = _comps_info.at(type).networkId;

    // __send_update_component(
    //     networkId,
    //     component_id,
    //     data,
    //     client
    // );

    // _clients.at(client).setEntity(entity);

    std::cerr << "Sent controllable component to client" << std::endl;
}

void UDPServer::start_receive(Engine::GameEngine &engine)
{
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        [this, &engine](const boost::system::error_code &ec, std::size_t bytes_recvd) {
            if (ec || bytes_recvd == 0) {
                std::cerr << "Could not receive properly from " << remote_endpoint_ << ", ec=" << ec << ", bytes_rcvd=" << bytes_recvd << std::endl;
                return;
            }
            std::cerr << "In the beginning" << std::endl;
            if (!_clients.contains(remote_endpoint_)) {
                __init_new_client();
            }
            std::cerr << "Step 2" << std::endl;
            auto &client_info = _clients.at(remote_endpoint_);
            std::cerr << "Step 3" << std::endl;
            std::cerr << "recv_buffer_[0] : " << (int)recv_buffer_[0] << std::endl;
            switch (recv_buffer_[0]) {
                case 0x0:
                    client_info.state = ClientInfo::State::INIT;
                    __send_components();
                    break;

                case 0x1:
                    std::cerr << "Received component to ignore" << std::endl;
                    if (bytes_recvd < 3) {
                        break;
                    }
                    for (auto type : client_info.used_types) {
                        std::cerr << "Type used by client: " << remote_endpoint_ << " : " << type.name() << std::endl;
                    }
                    __ignoreComponent(client_info);
                    break;

                case 0x2:
                    client_info.state = ClientInfo::State::STARTED;
                    createNewEntity(engine);
                    __add_new_client();

                    _isGameRunning = true;
                    break;

                case 0x3:
                    std::cerr << "Received update component" << std::endl;
                    receiveUpdateComponent(engine, recv_buffer_);
                    break;
            }
            std::cerr << "End of opearations" << std::endl;
            start_receive(engine);
        }
    );
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
    _clients.try_emplace(remote_endpoint_, io_context_, _compGetter);
    std::cerr << "New client added: " << remote_endpoint_ << std::endl;
}

void UDPServer::__ignoreComponent(UDPServer::ClientInfo &clientInfo)
{
    std::cerr << "In function ignoreComponent" << std::endl;
    uint16_t toIgnore = *reinterpret_cast<uint16_t *>(&recv_buffer_[1]);

    auto const &compInfo = std::find_if(
        _comps_info.begin(),
        _comps_info.end(),
        [toIgnore](const std::pair<const std::type_index, UDPServer::ComponentInfo> &a) {
            return a.second.networkId == toIgnore;
        }
    );

    if (compInfo == _comps_info.end()) {
        return;
    }
    clientInfo.used_types.erase(compInfo->first);
    __send_nb_components_message(remote_endpoint_);
    std::cerr << "Component " << compInfo->first.name() << " ignored" << std::endl;
}

void UDPServer::__send_nb_components_message(const udp::endpoint &client)
{
    const uint8_t opcode = 0x6;

    std::array<uint8_t, 3> msg;

    msg[0] = opcode;
    *reinterpret_cast<uint16_t *>(&msg[1]) = htons(_clients.at(client).used_types.size());
    std::cerr << "Sending nb components message: " << _clients.at(client).used_types.size() << std::endl;

    socket_.async_send_to(
        boost::asio::buffer(msg), client,
        [](boost::system::error_code ec, std::size_t bytes_sent) {
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
    for (const auto &e : _listEntities()) {
        __send_entity_created_message(
            _entitiesNetworkId.at(e),
            remote_endpoint_
        );
    }
    auto used_types = _clients.at(remote_endpoint_).used_types;
    used_types.erase(typeid(Components::Controllable));

    for (const auto &typeIdx : used_types) {
        uint16_t comp_netId = _comps_info.at(typeIdx).networkId;
        for (auto &&[i, _] : IndexedZipper(_compGetter[typeIdx])) {
            uint32_t entity_netId = _entitiesNetworkId.at(i);
            __send_attach_component_message(entity_netId, comp_netId, remote_endpoint_);
        }
    }
    for (const auto &typeIdx : used_types) {
        uint16_t comp_netId = _comps_info.at(typeIdx).networkId;
        for (auto &&[i, comp] : IndexedZipper(_compGetter[typeIdx])) {
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

void UDPServer::__send_components() {
    const uint8_t opcode = 0x5;

    for (const auto &[_, info] : _comps_info) {
        const auto &[name, index] = info;
        std::vector<uint8_t> buffer(sizeof(opcode) + sizeof(index) + name.size());

        buffer[0] = opcode;
        *reinterpret_cast<uint16_t *>(&buffer[1]) = index;
        std::memcpy(buffer.data() + 3, name.data(), name.size());

        __send_message(buffer);
    }
    __send_component_nb_loop(remote_endpoint_);
}

void UDPServer::__send_component_nb_loop(const udp::endpoint &ep)
{
    if (!_clients.contains(ep) || _clients.at(ep).state != ClientInfo::State::INIT) {
        return;
    }
    auto &client = _clients.at(ep);

    __send_nb_components_message(ep);

    client.timer.expires_at(client.timer.expiry() + boost::asio::chrono::milliseconds(500));
    client.timer.async_wait([this, ep](auto const &) {
        __send_component_nb_loop(ep);
    });
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

    std::cerr << "Created entity n°" << entity << " in local with network id n°" << networkId << std::endl;

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

    std::array<uint8_t, 5> message;
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));

    socket_.async_send_to(
        boost::asio::buffer(message), client,
        [ networkId](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint32_t id = networkId;
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
                std::cerr << "Sending delete entity message to client for entity: " << networkId << std::endl;
                if (!ec) {
                    uint32_t id = networkId;
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
        return;
    }

    uint16_t component_id = _comps_info.at(component).networkId;

    for (const auto &[endpoint, info] : _clients) {
        if (!info.used_types.contains(component)) {
            continue;
        }
        __send_attach_component_message(networkId, component_id, endpoint);
    }
}

void UDPServer::__send_attach_component_message(
    uint32_t entity_netId,
    uint16_t comp_netId,
    const udp::endpoint &client
)
{
    uint8_t opcode = 0x2;

    std::array<uint8_t, 7> message;
    message[0] = opcode;
    std::memcpy(&message[1], &entity_netId, sizeof(entity_netId));
    std::memcpy(&message[5], &comp_netId, sizeof(comp_netId));

    socket_.async_send_to(
        boost::asio::buffer(message), client,
        [entity_netId, comp_netId](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                std::cerr << "Attach component [" << comp_netId << "] to entity [" << entity_netId << "]." << std::endl;
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

    for (const auto &[endpoint, info] : _clients) {
        if (!info.used_types.contains(type)) {
            continue;
        }
        __send_update_component(
            networkId,
            component_id,
            data,
            endpoint
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
    size_t component_size = data.size();

    std::vector<uint8_t> message(7 + component_size);
    message[0] = opcode;
    std::memcpy(&message[1], &entity_netId, sizeof(entity_netId));
    std::memcpy(&message[5], &comp_netId, sizeof(comp_netId));
    std::memcpy(&message[7], data.data(), component_size);

    socket_.async_send_to(
        boost::asio::buffer(message), client,
        [](boost::system::error_code ec, std::size_t) {
            if (!ec) {}
        }
    );
}

void UDPServer::detach_component(size_t entity, std::type_index component) {
    uint8_t opcode = 0x4;
    uint32_t networkId = _entitiesNetworkId.at(entity);

    if (!_comps_info.contains(component)) {
        std::cerr << "Error 0x4: Component '" << component.name() << "' not found." << std::endl;
        return;
    }

    uint16_t component_id = _comps_info.at(component).networkId;

    std::array<uint8_t, 7> message;
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));
    std::memcpy(&message[5], &component_id, sizeof(component_id));

    for (const auto &[endpoint, info] : _clients) {
        if (!info.used_types.contains(component)) {
            continue;
        }
        socket_.async_send_to(
            boost::asio::buffer(message), endpoint,
            [networkId, component_id](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    std::cerr << "Detach component [" << component_id << "] from entity [" << networkId << "]." << std::endl;
                }
            }
        );
    }
}

void UDPServer::receiveUpdateComponent(Engine::GameEngine &engine, std::span<const uint8_t> operation)
{
    try {
        uint32_t networkId = *reinterpret_cast<const uint32_t *>(&operation[1]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));
        std::cerr << "Entity network id: " << networkId << std::endl;
        uint16_t componentId = *reinterpret_cast<const uint16_t *>(&operation[5]);

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
            std::cerr << "Updating component: " << comp_info_it->second.name << " with ID: " << entity << std::endl;
            for (auto data : serializedData) {
                std::cerr << (int)data << " ";
            }
            std::cerr << std::endl;
            sparseArray[entity]->deserialize(serializedData);
            std::cerr << "Updated component: " << comp_info_it->second.name << " with ID: " << entity << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "\033[0;35m";
            std::cerr << "Component " << comp_info_it->second.name << " was not attached for entity n°" << entity << ": " << e.what() << std::endl;
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

void UDPServer::lockMutex() {
    _engineMutex.lock();
}

void UDPServer::unlockMutex() {
    _engineMutex.unlock();
}
