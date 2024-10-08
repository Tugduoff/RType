/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RTypeClient
*/

#include "RTypeClient.hpp"

RTypeClient::RTypeClient(std::string hostname, std::string port)
: UDPConnection(hostname, port) 
{
}

std::unordered_map<uint8_t, std::string> RTypeClient::initGame()
{
    std::cout << "Init Game" << std::endl;
    send("start");

    uint16_t compNb = receiveUint16();
    uint8_t compNameMaxSize = receiveUint8();
    std::unordered_map<uint8_t, std::string> compNames;

    std::cout << "compNb : " << (int)compNb << "." << std::endl;
    std::cout << "compNameMaxSize : " << (int)compNameMaxSize << "." << std::endl;

    for (uint16_t i; i < compNb; i++) {
        std::vector<uint8_t> compName = blockingReceive();
        std::cout << "Comp Name : " << binaryToStr(compName) <<std::endl;
        // if (compName.size() < compNameMaxSize) {
        //     throw std::runtime_error("ERROR: Component name is not of the given size");
        // }
        // compNames.push_back(std::string(compName.begin(), compName.end()));
        compNames[compName[0]] = std::string(compName.begin() + 2, compName.end());
    }
    return compNames;
}

uint16_t RTypeClient::receiveUint16()
{
    std::vector<uint8_t> data = blockingReceive();

    if (data.size() < 2) {
        throw std::runtime_error("Received data is too short to make an uint16");
    }
    uint16_t result = (static_cast<uint16_t>(data[0]) << 8) | static_cast<uint16_t>(data[1]);
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

void RTypeClient::sendEvent(enum EventType event)
{
    std::vector<uint8_t> data = {(uint8_t)event};
    send(data);
}
