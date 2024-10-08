/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp_client
*/

#include <unordered_map>
#include "UDPConnection.hpp"

class RTypeClient : public UDPConnection
{
    public:

        RTypeClient(std::string hostname, std::string port) :
            UDPConnection(hostname, port) {}

        std::unordered_map<uint8_t, std::string> initGame()
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

        uint16_t receiveUint16()
        {
            std::vector<uint8_t> data = blockingReceive();

            if (data.size() < 2) {
                throw std::runtime_error("Received data is too short to make an uint16");
            }
            uint16_t result = (static_cast<uint16_t>(data[0]) << 8) | static_cast<uint16_t>(data[1]);
            return result;
        }

        uint8_t receiveUint8()
        {
            std::vector<uint8_t> data = blockingReceive();
            std::cout << "data size : " << data.size() << std::endl;
            if (data.size() < 1) {
                throw std::runtime_error("Received data is too short to make an uint8");
            }
            return data[0];
        }

        // ---- Events ----
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

        void sendEvent(enum EventType event)
        {
            std::vector<uint8_t> data = {(uint8_t)event};
            send(data);
        }
};

int main() {
    try {
        RTypeClient conn("127.0.0.1", "8080");

        std::string message = "Hello from client!";

        conn.send(message);
        std::cout << "Sent message: " << UDPConnection::binaryToStr(UDPConnection::strToBinary(message)) << std::endl;

        std::unordered_map<uint8_t, std::string> compNames = conn.initGame();
        for (const auto &name : compNames) {
            std::cout << "Commponent n°" << name.first << ": " << name.second << std::endl;
        }
        while (true) {
            udp::endpoint server_endpoint;
            std::vector<uint8_t> recv_mesage = conn.blockingReceive();
            std::cout << "Received message from server: " << UDPConnection::binaryToStr(recv_mesage) << std::endl;

            if (UDPConnection::binaryToStr(recv_mesage) == "ping") {
                conn.send("pong");
                std::cout << "Sent message: " << "pong" << std::endl;
            }
        }
        conn.close();

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
