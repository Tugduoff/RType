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
            std::cout << "Create Entity n°" << uint16From2Uint8(operation[1], operation[2]) << std::endl;
            createEntity(engine, operation);
            std::cout << std::endl;
            break;
        case 0x1:
            std::cout << "Delete Entity n°" << uint16From2Uint8(operation[1], operation[2]) << std::endl;
            deleteEntity(engine, operation);
            std::cout << std::endl;
            break;
        case 0x2:
            std::cout << "Attach Component" << std::endl;
            attachComponent(engine, operation);
            std::cout << std::endl;
            break;
        case 0x3:
            // std::cout << "Update Component" << std::endl;
            // std::cout << "Component n°" << uint16From2Uint8(operation[3], operation[4]) << std::endl;
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
    uint16_t entityId = ntohs(uint16From2Uint8(operation[1], operation[2]));

    std::cout << "Creating entity n°" << entityId << " from network" << std::endl;
    ECS::Entity entity = engine.getRegistry().entityManager().spawnEntity();
    // ECS::Entity entity = engine.getRegistry().entityManager().spawnEntityWithId(entityId);
    std::cout << "Created entity n°" << entity << " in local" << std::endl;
    if ((size_t)entityId != entity) {
        std::cout << "\033[0;33m";
        std::cout << "Error: Entity created does not have the same id as received in the network" << std::endl;
        std::cout << "\033[0;37m";
    }
} 

void RTypeClient::deleteEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    uint16_t entityId = uint16From2Uint8(operation[1], operation[2]);

    try {
        ECS::Entity entity = engine.getRegistry().entityManager().entityFromIndex(entityId);
        
        std::cout << "Deleting entity n°" << entityId << " from network" << std::endl;
        engine.getRegistry().entityManager().killEntity(entity);
        std::cout << "Deleted entity n°" << entity << " in local" << std::endl;
    } catch (std::exception &e) {
        std::cout << "\033[0;31m";
        std::cout << "Could not delete entity n°" << entityId << std::endl;
        std::cout << "\033[0;37m";
    }
}

void RTypeClient::attachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    uint16_t entityId = uint16From2Uint8(operation[1], operation[2]);
    uint16_t componentId = uint16From2Uint8(operation[3], operation[4]);
    std::string &strCompId = _compNames[componentId];
    std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
    
    std::cout << "Attaching Component n°" << componentId << ": " << strCompId << " to entity n°" << entityId << std::endl;
    auto &compInstance = engine.getComponentFromId(strCompId);
    auto &sparseArray = compInstance->any_cast(
        engine.getRegistry().componentManager().getComponents(compTypeIndex)
    );
    sparseArray.constructAt(entityId);
}

void RTypeClient::updateComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    uint16_t entityId = uint16From2Uint8(operation[1], operation[2]);
    uint16_t componentId = uint16From2Uint8(operation[3], operation[4]);
    std::string strCompId = _compNames[componentId];
    std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
    
    // std::cout << "Updating Component n°" << componentId << ": " << strCompId << " of entity n°" << entityId << std::endl;
    auto &compInstance = engine.getComponentFromId(compTypeIndex);
    auto &sparseArray = compInstance->any_cast(
        engine.getRegistry().componentManager().getComponents(compTypeIndex)
    );
    std::vector<uint8_t> serializedData = std::vector<uint8_t>(operation.begin() + 5, operation.end());
    try {
        sparseArray[entityId]->getId();
    } catch (std::exception &e) {
        std::cout << "\033[0;35m";
        std::cout << "Component " << strCompId << " was not attached for entity n°" << entityId << " so created it" << std::endl;
        std::cout << "\033[0;37m";

        sparseArray.constructAt(entityId);
    }
    sparseArray[entityId]->deserialize(serializedData);
}

void RTypeClient::detachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation)
{
    uint16_t entityId = uint16From2Uint8(operation[1], operation[2]);
    uint16_t componentId = uint16From2Uint8(operation[3], operation[4]);
    std::string strCompId = _compNames[componentId];
    std::type_index compTypeIndex = engine.getTypeIndexFromString(strCompId);
    
    std::cout << "Detaching Component n°" << componentId << ": " << strCompId << " of entity n°" << entityId << std::endl;
    auto &compInstance = engine.getComponentFromId(compTypeIndex);
    auto &sparseArray = compInstance->any_cast(
        engine.getRegistry().componentManager().getComponents(compTypeIndex)
    );
    sparseArray.erase(entityId);
}

uint16_t RTypeClient::uint16From2Uint8(uint8_t first, uint8_t second)
{
    return static_cast<uint16_t>((first << 8) | static_cast<uint16_t>(second));
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
