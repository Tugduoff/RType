/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RTypeClient
*/

#include <stdlib.h>
#include "RTypeClient.hpp"

RTypeClient::RTypeClient(std::string hostname, std::string port)
: UDPConnection(hostname, port) 
{
}

void RTypeClient::initGame()
{
    std::cout << "Init Game" << std::endl;
    send("start");

    uint16_t compNb = receiveUint16();
    uint8_t compNameMaxSize = receiveUint8();

    std::cout << "CompNb : " << (int)compNb << "." << std::endl;
    std::cout << "CompNameMaxSize : " << (int)compNameMaxSize << "." << std::endl;

    for (uint16_t i; i < compNb; i++) {
        std::vector<uint8_t> compName = blockingReceive();
        std::string strCompName = std::string(compName.begin() + 1, compName.end());
        // uint16_t CompId = (static_cast<uint16_t>(compName[0]) << 8) | static_cast<uint16_t>(compName[1]);

        _compNames[compName[0]] = std::string(compName.begin() + 2, compName.end());
    }
    // uint16_t endIndicator = receiveUint16();
    // if (endIndicator != 0xffff) {
    //     std::cerr << "End Indicator : " << (int) endIndicator << std::endl;
    //     throw std::runtime_error("Did not find the end indicator for network init");
    // }
}

int RTypeClient::findFirstOf(char c, const std::vector<uint8_t> &array)
{
    for (int i = 0; i < array.size(); i++) {
        if (array[i] == c) {
            return i;
        }
    }
    return -1;
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
