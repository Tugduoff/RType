/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RTypeClient.hpp
*/

#ifndef RTYPE_CLIENT_HPP_
    #define RTYPE_CLIENT_HPP_
    #include <unordered_map>
    #include "UDPConnection.hpp"
    #include "GameEngine/GameEngine.hpp"

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

        void engineInit();
        bool dataFromServer();
        void interpretServerData(Engine::GameEngine &engine);

        void createEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation);      // We also give entityId to the create Entity to ensure same entity id
        void deleteEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation);
        void attachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation);
        void updateComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation);
        void detachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation);

        uint16_t uint16From2Uint8(uint8_t first, uint8_t second);
        uint16_t receiveUint16();
        uint8_t receiveUint8();

        void sendEvent(enum EventType event);

        std::unordered_map<uint8_t, std::string> getCompNames() const { return _compNames; };

    public:
        bool gameEnd;
    
    private:
        std::unordered_map<uint8_t, std::string> _compNames;
};

#endif /* !RTYPE_CLIENT_HPP_ */
