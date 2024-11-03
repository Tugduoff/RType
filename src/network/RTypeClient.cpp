/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RTypeClient.cpp
*/

#include <cstdint>
#include <netinet/in.h>
#include <stdlib.h>
#include <thread>
#include "RTypeClient.hpp"

RTypeClient::RTypeClient(std::string hostname, std::string port)
: UDPConnection(hostname, port), gameEnd(false), nextFrame(false), finishedInit(false)
{
}

// void RTypeClient::engineInit(const std::unordered_map<std::string, std::type_index> &idStringToType)
// {
//     std::cerr << "Init Game" << std::endl;
//     send(std::vector<uint8_t>({0x0}));
//     bool finishedInit = false;

//     while (!finishedInit)
//     {
//         std::optional<std::vector<uint8_t>> recv_buffer = nonBlockingReceive();
//         if (recv_buffer.has_value()) {
//             // std::cout << "Received something" << std::endl;
//             interpretServerInitData(recv_buffer.value(), finishedInit, idStringToType);
//         }
//     }
//     send(std::vector<uint8_t>({0x2}));
// }

// void RTypeClient::interpretServerInitData(std::vector<uint8_t> &recv_buffer, bool &finishedInit,
//     const std::unordered_map<std::string, std::type_index> &idStringToType)
// {
//     static int receivedFinishNb = 0;

//     if (recv_buffer.size() < 1) {
//         return;
//     }
//     switch (recv_buffer[0]) {
//         case 0x5:
//         {
//             uint16_t compId = *reinterpret_cast<uint16_t *>(recv_buffer.data() + 1);
//             std::string strCompName = std::string(recv_buffer.begin() + 3, recv_buffer.end());
//             if (idStringToType.contains(strCompName)) {
//                 std::cout << "Component " << strCompName <<
//                     " added to client components" << std::endl;
//                 _compNames[compId] = strCompName;
//             } else {
//                 std::vector<uint8_t> message(3);
//                 message[0] = 0x1;
//                 std::memcpy(&message[1], &compId, sizeof(compId));

//                 std::cout << "Component " << strCompName <<
//                     " is not needed by the client so removing it" << std::endl;
//                 send(message);
//             }
//             break;
//         }
//         case 0x6:
//         {
//             uint16_t componentsTypesNb = ntohs(*reinterpret_cast<uint16_t *>(recv_buffer.data() + 1));
//             std::cout << "Received init end from server, ";
//             if (componentsTypesNb == idStringToType.size() && receivedFinishNb >= 2) {
//                 finishedInit = true;
//                 std::cout << "initialization finished" << std::endl;
//             } else {
//                 std::cout << "components not synchronized with client, continuing initialization: "
//                     << componentsTypesNb << " != " << idStringToType.size() << "; recvd " << receivedFinishNb << std::endl;
//             }
//             receivedFinishNb += ((receivedFinishNb < 2) ? 1 : 0);
//             break;
//         }
//         default:
//             break;
//     }
// }

bool RTypeClient::dataFromServer()
{
    if (_socket.available()) {
        return true;
    }
    return false;
}

void RTypeClient::asyncReceive(Engine::GameEngine &engine)
{
    std::unique_ptr<std::vector<uint8_t>> recv_buffer = std::make_unique<std::vector<uint8_t>>(CLIENT_BUFFER_FIXED_SIZE);
    auto buffer_ptr = recv_buffer.get();
    _socket.async_receive_from(
        boost::asio::buffer(*buffer_ptr), _sender_endpoint,
        [this, &engine, _recv_buffer = std::move(recv_buffer)](const boost::system::error_code &ec, std::size_t bytes_recvd) {
            if (!ec && _sender_endpoint == _server_endpoint) {
                _recv_buffer.get()->resize(bytes_recvd);
                this->_packetQueueMutex.lock();
                this->_packetQueue.push(*_recv_buffer.get());
                this->_packetQueueMutex.unlock();
                asyncReceive(engine);
            }
        }
    );
}

void RTypeClient::startInterpret(Engine::GameEngine &engine)
{
    while (true)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1us);
        if (!_packetQueue.empty()) {
            interpretServerData(engine, _packetQueue.front());
            _packetQueueMutex.lock();
            _packetQueue.pop();
            _packetQueueMutex.unlock();
        }
    }
}

void RTypeClient::interpretServerData(Engine::GameEngine &engine, std::vector<uint8_t> &recv_buffer)
{
    static int receivedFinishNb = 0;

    if (recv_buffer.size() < 1) {
        return;
    }
    switch (recv_buffer[0]) {
        case 0x0:
            std::cerr << "Create Entity n°" << *reinterpret_cast<uint16_t *>(&recv_buffer[1]) << std::endl;
            createEntity(engine, recv_buffer);
            std::cerr << std::endl;
            break;
        case 0x1:
            std::cerr << "Recieved delete instruction!" << std::endl;
            std::cerr << "Delete Entity n°" << *reinterpret_cast<uint16_t *>(&recv_buffer[1]) << std::endl;
            deleteEntity(engine, recv_buffer);
            std::cerr << std::endl;
            break;
        case 0x2:
            std::cerr << "Attach Component" << std::endl;
            attachComponent(engine, recv_buffer);
            std::cerr << std::endl;
            break;
        case 0x3:
            std::cerr << "Update Component" << std::endl;
            std::cerr << "Component n°" << *reinterpret_cast<uint16_t *>(&recv_buffer[5]) << " of Entity n°" << *reinterpret_cast<uint32_t *>(&recv_buffer[1]) << std::endl;
            updateComponent(engine, recv_buffer);
            break;
        case 0x4:
            std::cerr << "Detach Component" << std::endl;
            detachComponent(engine, recv_buffer);
            std::cerr << std::endl;
            break;
        case 0xa:
            nextFrame = true;
            std::cerr << "Next frame" << std::endl;
            break;
        case 0x5:
        {
            uint16_t compId = *reinterpret_cast<uint16_t *>(recv_buffer.data() + 1);
            std::string strCompName = std::string(recv_buffer.begin() + 3, recv_buffer.end());
            if (engine.getIdStringToType().contains(strCompName)) {
                std::cout << "Component " << strCompName <<
                    " added to client components" << std::endl;
                _compNames[compId] = strCompName;
            } else {
                std::vector<uint8_t> message(3);
                message[0] = 0x1;
                std::memcpy(&message[1], &compId, sizeof(compId));

                std::cout << "Component " << strCompName <<
                    " is not needed by the client so removing it" << std::endl;
                send(message);
            }
            break;
        }
        case 0x6:
        {
            uint16_t componentsTypesNb = ntohs(*reinterpret_cast<uint16_t *>(recv_buffer.data() + 1));
            std::cout << "Received init end from server, ";
            if (componentsTypesNb == engine.getIdStringToType().size() && receivedFinishNb >= 2) {
                finishedInit = true;
                std::cout << "initialization finished" << std::endl;
            } else {
                std::cout << "components not synchronized with client, continuing initialization: "
                    << componentsTypesNb << " != " << engine.getIdStringToType().size() << "; recvd " << receivedFinishNb << std::endl;
            }
            receivedFinishNb += ((receivedFinishNb < 2) ? 1 : 0);
            break;
        }
        default:
            std::cerr << "Error: Unknown opcode : " << int(recv_buffer[0]) << ". Full command is : " << std::endl;
            std::cerr << binaryToStr(recv_buffer) << std::endl;
            if (binaryToStr(recv_buffer).find("You have been disconnected :(") != std::string::npos) {
                gameEnd = true;
            }
            break;
    }
}

void RTypeClient::createEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    uint32_t networkId = *reinterpret_cast<uint32_t *>(&operation[1]);

    lockMutex();
    ECS::Entity entity = engine.getRegistry().entityManager().spawnEntity();
    unlockMutex();
    _entitiesNetworkId.insert({networkId, entity});
    std::cerr << "Created entity n°" << entity << " in local with network id n°" << networkId << std::endl;
} 

void RTypeClient::deleteEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = *reinterpret_cast<uint32_t *>(&operation[1]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));

        try {
            lockMutex();
            engine.getRegistry().killEntity(entity);
            unlockMutex();
            _entitiesNetworkId.erase(networkId);
            std::cerr << "Deleted entity n°" << entity << " in local" << std::endl;
            std::cerr << "Deleted entity n°" << networkId << " in network" << std::endl;
        } catch (std::exception &e) {
            std::cerr << "\033[0;31m";
            std::cerr << "Could not delete entity n°" << entity << std::endl;
            std::cerr << "\033[0;37m";
        }
    }
    catch(const std::exception &) {
        std::cerr << "Could not delete entity. Couldnt find network id" << std::endl;
    }
}

void RTypeClient::attachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = *reinterpret_cast<uint32_t *>(&operation[1]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));
        uint16_t componentId = *reinterpret_cast<uint16_t *>(&operation[5]);
        std::string &strCompId = _compNames[componentId];
        std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
        
        std::cerr << "Attaching Component n°" << componentId << ": " << strCompId << " to entity n°" << entity << std::endl;
        auto &compInstance = engine.getComponentFromId(strCompId);
        auto &sparseArray = compInstance->any_cast(
            engine.getRegistry().componentManager().getComponents(compTypeIndex)
        );
        lockMutex();
        sparseArray.constructAt(entity);
        unlockMutex();
    }
    catch(const std::exception &) {

    }
}

void RTypeClient::updateComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = *reinterpret_cast<uint32_t *>(&operation[1]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));
        uint16_t componentId = *reinterpret_cast<uint16_t *>(&operation[5]);
        std::string strCompId = _compNames[componentId];
        std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
        
        // std::cerr << "Updating Component n°" << componentId << ": " << strCompId << " of entity n°" << networkId << " network n°" << entity << std::endl;
        auto &compInstance = engine.getComponentFromId(compTypeIndex);
        auto &sparseArray = compInstance->any_cast(
            engine.getRegistry().componentManager().getComponents(compTypeIndex)
        );
        std::vector<uint8_t> serializedData = std::vector<uint8_t>(operation.begin() + 7, operation.end());
        try {
            sparseArray[entity]->getId();
        } catch (std::exception &e) {
            std::cerr << "\033[0;35m";
            std::cerr << "Component " << strCompId << " was not attached for entity n°" << entity << " so created it" << std::endl;
            std::cerr << "\033[0;37m";

            lockMutex();
            sparseArray.constructAt(entity);
            unlockMutex();
        }
        lockMutex();
        sparseArray[entity]->deserialize(serializedData);
        unlockMutex();
    }
    catch(const std::exception &) {

    }    
}

void RTypeClient::detachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = *reinterpret_cast<uint32_t *>(&operation[1]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));
        uint16_t componentId = *reinterpret_cast<uint16_t *>(&operation[5]);
        std::string strCompId = _compNames[componentId];
        std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
        
        std::cerr << "Detaching Component n°" << componentId << ": " << strCompId << " of entity n°" << entity << std::endl;
        auto &compInstance = engine.getComponentFromId(compTypeIndex);
        auto &sparseArray = compInstance->any_cast(
            engine.getRegistry().componentManager().getComponents(compTypeIndex)
        );
        lockMutex();
        sparseArray.erase(entity);
        unlockMutex();
    } catch (std::exception &) {

    }
}

void RTypeClient::sendUpdateComponent(size_t entity, std::string name, std::vector<uint8_t> data)
{
    uint8_t opcode = 0x3;
    uint32_t networkId;

    auto entity_it = std::find_if(_entitiesNetworkId.begin(), _entitiesNetworkId.end(),
        [entity](const auto& pair) {
            return pair.second == entity;
        });
    if (entity_it != _entitiesNetworkId.end()) {
        networkId = entity_it->first;
    } else {
        std::cerr << "The entity n°" << entity << " does not have a network id" << std::endl;
        return;
    }

    auto it = std::find_if(_compNames.begin(), _compNames.end(),
    [&name](const auto& pair) {
        return pair.second == name;
    });
    if (it == _compNames.end()) {
        std::cerr << "Error 0x3: Component '" << name << "' not found." << std::endl;
        return;
    }

    uint16_t component_id = it->first;
    size_t component_size = data.size();

    std::vector<uint8_t> message(7 + component_size);
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));
    std::memcpy(&message[5], &component_id, sizeof(component_id));
    std::memcpy(&message[7], data.data(), component_size);

    std::cerr << "Sending update component n°" << component_id << ": " << name << " of entity n°" << entity << std::endl;

    send(message);
}

uint16_t RTypeClient::uint16From2Uint8(uint8_t first, uint8_t second)
{
    return static_cast<uint16_t>((second << 8) | static_cast<uint16_t>(first));
}

uint32_t RTypeClient::uint32From4Uint8(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4)
{
    return static_cast<uint32_t>((byte4 << 24) | (byte3 << 16) | (byte2 << 8) | byte1);
}

uint16_t RTypeClient::receiveUint16()
{
    std::vector<uint8_t> data = blockingReceive();

    if (data.size() < 2) {
        throw std::runtime_error("Received data is too short to make an uint16");
    }
    return *reinterpret_cast<uint16_t *>(&data[0]);
}

uint8_t RTypeClient::receiveUint8()
{
    std::vector<uint8_t> data = blockingReceive();
    std::cerr << "data size : " << data.size() << std::endl;
    if (data.size() < 1) {
        throw std::runtime_error("Received data is too short to make an uint8");
    }
    return data[0];
}

void RTypeClient::lockMutex() {
    _engineMutex.lock();
}

void RTypeClient::unlockMutex() {
    _engineMutex.unlock();
}
