/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RTypeClient
*/

#ifndef RTYPE_CLIENT_HPP_
    #define RTYPE_CLIENT_HPP_
    #include <unordered_map>
    #include "UDPConnection.hpp"

class RTypeClient : public UDPConnection
{
    public:

        enum EventType {
            RIGHT_KEY_PRESSED = 0,
            RIGHT_KEY_RELEASED,
            UP_KEY_PRESSED,
            UP_KEY_RELEASED,
            LEFT_KEY_PRESSED,
            LEFT_KEY_RELEASED,
            DOWN_KEY_PRESSED,
            DOWN_KEY_RELEASED,
            LEFT_CLICK_PRESSED,
            LEFT_CLICK_RELEASED,
            RIGHT_CLICK_PRESSED,
            RIGHT_CLICK_RELEASED,

            EVENT_NB,
        };

    public:

        RTypeClient(std::string hostname, std::string port);

        std::unordered_map<uint8_t, std::string> initGame();

        uint16_t receiveUint16();
        uint8_t receiveUint8();

        void sendEvent(enum EventType event);
};

#endif /* !RTYPE_CLIENT_HPP_ */
