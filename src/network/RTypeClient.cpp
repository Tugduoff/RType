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
    std::cout << "Init Game" << std::endl;
    send("start");

    uint16_t compNb = receiveUint16();
    uint8_t compNameMaxSize = receiveUint8();

    std::cout << "CompNb : " << (int)compNb << "." << std::endl;
    std::cout << "CompNameMaxSize : " << (int)compNameMaxSize << "." << std::endl;

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

void RTypeClient::interpretServerData(Engine::GameEngine &engine)
{
    std::vector<uint8_t> operation = blockingReceive();

    switch (operation[0]) {
        case 0x0:
            std::cout << "Create Entity n°" << uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]) << std::endl;
            createEntity(engine, operation);
            std::cout << std::endl;
            break;
        case 0x1:
            std::cout << "Delete Entity n°" << uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]) << std::endl;
            deleteEntity(engine, operation);
            std::cout << std::endl;
            break;
        case 0x2:
            std::cout << "Attach Component" << std::endl;
            attachComponent(engine, operation);
            std::cout << std::endl;
            break;
        case 0x3:
            std::cout << "Update Component" << std::endl;
            std::cout << "Component n°" << uint16From2Uint8(operation[5], operation[6]) << std::endl;
            updateComponent(engine, operation);
            break;
        case 0x4:
            std::cout << "Detach Component" << std::endl;
            detachComponent(engine, operation);
            std::cout << std::endl;
            break;
        default:
            std::cerr << "Error: Unknown opcode : " << int(operation[0]) << ". Full command is : " << std::endl;
            std::cerr << binaryToStr(operation) << std::endl;
            if (binaryToStr(operation).find("You have been disconnected :(") != std::string::npos) {
                gameEnd = true;
            }
            return;
    }
    // std::cout << std::endl;
}

void RTypeClient::createEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);

    ECS::Entity entity = engine.getRegistry().entityManager().spawnEntity();
    _entitiesNetworkId.insert({networkId, entity});
    std::cout << "Created entity n°" << entity << " in local with network id n°" << networkId << std::endl;
} 

void RTypeClient::deleteEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    try {
        uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);
        ECS::Entity &entity = _entitiesNetworkId.at(networkId);

        try {
            engine.getRegistry().entityManager().killEntity(entity);
            _entitiesNetworkId.erase(networkId);
            std::cout << "Deleted entity n°" << entity << " in local" << std::endl;
        } catch (std::exception &e) {
            std::cout << "\033[0;31m";
            std::cout << "Could not delete entity n°" << entity << std::endl;
            std::cout << "\033[0;37m";
        }
    }
    catch(const std::exception &) {

    }
}

void RTypeClient::attachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{    
    try {
        uint32_t networkId = uint32From4Uint8(operation[1], operation[2], operation[3], operation[4]);
        ECS::Entity &entity = _entitiesNetworkId.at(networkId);
        uint16_t componentId = uint16From2Uint8(operation[5], operation[6]);
        std::string &strCompId = _compNames[componentId];
        std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
        
        std::cout << "Attaching Component n°" << componentId << ": " << strCompId << " to entity n°" << entity << std::endl;
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
        ECS::Entity &entity = _entitiesNetworkId.at(networkId);
        uint16_t componentId = uint16From2Uint8(operation[5], operation[6]);
        std::string strCompId = _compNames[componentId];
        std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
        
        // std::cout << "Updating Component n°" << componentId << ": " << strCompId << " of entity n°" << entityId << std::endl;
        auto &compInstance = engine.getComponentFromId(compTypeIndex);
        auto &sparseArray = compInstance->any_cast(
            engine.getRegistry().componentManager().getComponents(compTypeIndex)
        );
        std::vector<uint8_t> serializedData = std::vector<uint8_t>(operation.begin() + 7, operation.end());
        try {
            sparseArray[entity]->getId();
        } catch (std::exception &e) {
            std::cout << "\033[0;35m";
            std::cout << "Component " << strCompId << " was not attached for entity n°" << entity << " so created it" << std::endl;
            std::cout << "\033[0;37m";

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
        ECS::Entity &entity = _entitiesNetworkId.at(networkId);
        uint16_t componentId = uint16From2Uint8(operation[5], operation[6]);
        std::string strCompId = _compNames[componentId];
        std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
        
        std::cout << "Detaching Component n°" << componentId << ": " << strCompId << " of entity n°" << entity << std::endl;
        auto &compInstance = engine.getComponentFromId(compTypeIndex);
        auto &sparseArray = compInstance->any_cast(
            engine.getRegistry().componentManager().getComponents(compTypeIndex)
        );
        sparseArray.erase(entity);
    } catch (std::exception &) {

    }
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
    std::cout << "data size : " << data.size() << std::endl;
    if (data.size() < 1) {
        throw std::runtime_error("Received data is too short to make an uint8");
    }
    return data[0];
}
