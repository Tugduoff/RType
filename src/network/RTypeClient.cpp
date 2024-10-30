/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RTypeClient.cpp
*/

#include <stdlib.h>
#include "RTypeClient.hpp"

RTypeClient::RTypeClient(std::string hostname, std::string port)
: UDPConnection(hostname, port), gameEnd(false)
{
}

void RTypeClient::engineInit()
{
    std::cerr << "Init Game" << std::endl;
    send("start");

    uint16_t compNb = receiveUint16();
    uint8_t compNameMaxSize = receiveUint8();

    std::cerr << "CompNb : " << (int)compNb << "." << std::endl;
    std::cerr << "CompNameMaxSize : " << (int)compNameMaxSize << "." << std::endl;

    for (uint16_t i = 0; i < compNb; i++) {
        std::vector<uint8_t> compName = blockingReceive();
        std::string strCompName = std::string(compName.begin() + 1, compName.end());
        // uint16_t CompId = (static_cast<uint16_t>(compName[0]) << 8) | static_cast<uint16_t>(compName[1]);

        _compNames[compName[0]] = std::string(compName.begin() + 1, compName.end());
    }
    // uint16_t endIndicator = receiveUint16();
    // if (endIndicator != 0xffff) {
    //     std::cerr << "End Indicator : " << (int) endIndicator << std::endl;
    //     throw std::runtime_error("Did not find the end indicator for network init");
    // }
}

bool RTypeClient::dataFromServer()
{
    if (_socket.available()) {
        return true;
    }
    return false;
}

void RTypeClient::asyncReceive(Engine::GameEngine &engine)
{
    _recv_buffer.resize(CLIENT_BUFFER_FIXED_SIZE);
    _socket.async_receive_from(
        boost::asio::buffer(_recv_buffer), _sender_endpoint,
        [this, &engine](const boost::system::error_code &ec, std::size_t bytes_recvd) {
            if (!ec && _sender_endpoint == _server_endpoint) {
                this->interpretServerData(engine, bytes_recvd);
            }
        }
    );
}

void RTypeClient::interpretServerData(Engine::GameEngine &engine, std::size_t bytes_recvd)
{
    _recv_buffer.resize(bytes_recvd);
    switch (_recv_buffer[0]) {
        case 0x0:
            std::cerr << "Create Entity n°" << uint32From4Uint8(_recv_buffer[1], _recv_buffer[2], _recv_buffer[3], _recv_buffer[4]) << std::endl;
            createEntity(engine, _recv_buffer);
            std::cerr << std::endl;
            break;
        case 0x1:
            std::cerr << "Recieved delete instruction!" << std::endl;
            std::cerr << "Delete Entity n°" << uint32From4Uint8(_recv_buffer[1], _recv_buffer[2], _recv_buffer[3], _recv_buffer[4]) << std::endl;
            deleteEntity(engine, _recv_buffer);
            std::cerr << std::endl;
            break;
        case 0x2:
            std::cerr << "Attach Component" << std::endl;
            attachComponent(engine, _recv_buffer);
            std::cerr << std::endl;
            break;
        case 0x3:
            // std::cerr << "Update Component" << std::endl;
            // std::cerr << "Component n°" << uint16From2Uint8(_recv_buffer[5], _recv_buffer[6]) << " of Entity n°" << uint32From4Uint8(_recv_buffer[1], _recv_buffer[2], _recv_buffer[3], _recv_buffer[4]) << std::endl;
            updateComponent(engine, _recv_buffer);
            break;
        case 0x4:
            std::cerr << "Detach Component" << std::endl;
            detachComponent(engine, _recv_buffer);
            std::cerr << std::endl;
            break;
        default:
            std::cerr << "Error: Unknown opcode : " << int(_recv_buffer[0]) << ". Full command is : " << std::endl;
            std::cerr << binaryToStr(_recv_buffer) << std::endl;
            if (binaryToStr(_recv_buffer).find("You have been disconnected :(") != std::string::npos) {
                gameEnd = true;
            }
            break;
    }
    asyncReceive(engine);
}

void RTypeClient::createEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);

    ECS::Entity entity = engine.getRegistry().entityManager().spawnEntity();
    _entitiesNetworkId.insert({networkId, entity});
    std::cerr << "Created entity n°" << entity << " in local with network id n°" << networkId << std::endl;
} 

void RTypeClient::deleteEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));

        try {
            engine.getRegistry().killEntity(entity);
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
        uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));
        uint16_t componentId = uint16From2Uint8(operation[5], operation[6]);
        std::string &strCompId = _compNames[componentId];
        std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
        
        std::cerr << "Attaching Component n°" << componentId << ": " << strCompId << " to entity n°" << entity << std::endl;
        auto &compInstance = engine.getComponentFromId(strCompId);
        auto &sparseArray = compInstance->any_cast(
            engine.getRegistry().componentManager().getComponents(compTypeIndex)
        );
        sparseArray.constructAt(entity);
    }
    catch(const std::exception &) {

    }
}

void RTypeClient::updateComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));
        uint16_t componentId = uint16From2Uint8(operation[5], operation[6]);
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

            sparseArray.constructAt(entity);
        }
        sparseArray[entity]->deserialize(serializedData);
    }
    catch(const std::exception &) {

    }    
}

void RTypeClient::detachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);
        ECS::Entity entity = static_cast<ECS::Entity>(_entitiesNetworkId.at(networkId));
        uint16_t componentId = uint16From2Uint8(operation[5], operation[6]);
        std::string strCompId = _compNames[componentId];
        std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
        
        std::cerr << "Detaching Component n°" << componentId << ": " << strCompId << " of entity n°" << entity << std::endl;
        auto &compInstance = engine.getComponentFromId(compTypeIndex);
        auto &sparseArray = compInstance->any_cast(
            engine.getRegistry().componentManager().getComponents(compTypeIndex)
        );
        sparseArray.erase(entity);
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
        networkId = htonl(networkId);
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

    uint8_t index = it->first;
    uint16_t component_id = index;
    component_id = htons(component_id);
    size_t component_size = data.size();

    std::vector<uint8_t> message(7 + component_size);
    message[0] = opcode;
    std::memcpy(&message[1], &networkId, sizeof(networkId));
    std::memcpy(&message[5], &component_id, sizeof(component_id));
    std::memcpy(&message[7], data.data(), component_size);

    send(message);
}

uint16_t RTypeClient::uint16From2Uint8(uint8_t first, uint8_t second)
{
    return static_cast<uint16_t>((first << 8) | static_cast<uint16_t>(second));
}

uint32_t RTypeClient::uint32From4Uint8(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4)
{
    return static_cast<uint32_t>((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4);
}

uint16_t RTypeClient::receiveUint16()
{
    std::vector<uint8_t> data = blockingReceive();

    if (data.size() < 2) {
        throw std::runtime_error("Received data is too short to make an uint16");
    }
    uint16_t result = uint16From2Uint8(data[0], data[1]);
    return result;
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
