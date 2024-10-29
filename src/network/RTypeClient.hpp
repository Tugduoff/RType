/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RTypeClient.hpp
*/

#ifndef RTYPE_CLIENT_HPP_
    #define RTYPE_CLIENT_HPP_

    #include <SFML/Graphics.hpp>
    #include <SFML/Window.hpp>
    #include <unordered_map>
    #include "UDPConnection.hpp"
    #include "GameEngine/GameEngine.hpp"

/**
 * @class RTypeClient
 * 
 * @brief This class implement more RType-oriented communications with the server
 * @brief This class inherit from UDPConnection to facilitate the communications with the server
 */
class RTypeClient : public UDPConnection
{
    public:

        /**
         * @brief Constructor of RTypeClient
         * 
         * @param hostname : a string which represents the hostname of the server
         * @param port : a string which represents the port on which the server is running
         * 
         * @note The parameters are transfered to the UDPConnection class
         */
        RTypeClient(std::string hostname, std::string port);

        /**
         * @brief This function gets all the types that will be used during program execution
         * @brief and stores them in _compNames
         */
        void engineInit();

        /**
         * @brief Check if data was send from the server
         * 
         * @return bool : True if data was sent, False otherwise
         */
        bool dataFromServer();

        /**
         * @brief Read data sent from the server and act accordingly
         * 
         * @param engine : The game engine on which to do modifications
         * 
         * @note This function receive the data, read the opcode and call one those functions accordingly : 
         * @note createEntity, deleteEntity, attachComponent, updateComponent, detachComponent
         */
        void interpretServerData(Engine::GameEngine &engine);

        /**
         * @brief Create a new entity
         * 
         * @param engine : The game engine on which to do the modification
         * @param operation : An std::vector<uint8_t> which contains the data sent from the server
         * 
         * @note This function create a new entity
         * @note This function will be called by `interpretServerData`
         */
        void createEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation);

        /**
         * @brief Delete an entity
         * 
         * @param engine : The game engine on which to do the modification
         * @param operation : An std::vector<uint8_t> which contains the data sent from the server
         * 
         * @note This function interprets the `operation` and delete the corresponding entity
         * @note This function will be called by `interpretServerData`
         */
        void deleteEntity(Engine::GameEngine &engine, std::vector<uint8_t> operation);

        /**
         * @brief Attach the component to an entity
         * 
         * @param engine : The game engine on which to do the modification
         * @param operation : An std::vector<uint8_t> which contains the data sent from the server
         * 
         * @note This function interprets the `operation` and attach a component to an entity
         * @note This function will be called by `interpretServerData`
         */
        void attachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation);

        /**
         * @brief Update the component of an entity
         * 
         * @param engine : The game engine on which to do the modification
         * @param operation : An std::vector<uint8_t> which contains the data sent from the server
         * 
         * @note This function interprets the `operation` and update the component of an entity
         * @note This function will be called by `interpretServerData`
         */
        void updateComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation);

        /**
         * @brief Detach the component of an entity
         * 
         * @param engine : The game engine on which to do the modification
         * @param operation : An std::vector<uint8_t> which contains the data sent from the server
         * 
         * @note This function interprets the `operation` and detach a component of an entity
         * @note This function will be called by `interpretServerData`
         */
        void detachComponent(Engine::GameEngine &engine, std::vector<uint8_t> operation);

        /**
         * @brief Send to the server update of components from client
         * 
         * @param entity : The id of the entity on which the update occurs
         * @param name : The id string of the component to update
         * @param data : The serialized data of the component to send
         */
        void sendUpdateComponent(size_t entity, std::string name, std::vector<uint8_t> data);

        /**
         * @brief Create an uint16 from 2 uint8
         * 
         * @param first : The first uint8
         * @param second : The second uint8
         * 
         * @return uint16_t : The uint16 created from the 2 uint8
         */
        uint16_t uint16From2Uint8(uint8_t first, uint8_t second);

        uint32_t uint32From4Uint8(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);

        /**
         * @brief Receive an uint16 from the server
         * 
         * @return uint16_t : The uint16 received from the server
         * 
         * @note This function calls a `blockingReceive` and interprets the std::vector<uint8_t> received into an uint16
         */
        uint16_t receiveUint16();

        /**
         * @brief Receive an uint8 from the server
         * 
         * @return uint8_t : The uint8 received from the server
         * 
         * @note This function calls a `blockingReceive` and interprets the std::vector<uint8_t> received into an uint8
         */
        uint8_t receiveUint8();

        /**
         * @brief Getter of _compNames
         * 
         * @return std::unordered_map<uint8_t, std::string> & : reference to _compNames
         * 
         * @note The _compNames map is a map of the component id received from the network to the string id of the component
         */
        std::unordered_map<uint8_t, std::string> &getCompNames() { return _compNames; };

        void menu();

    public:
        bool gameEnd;
    
    private:
        std::unordered_map<uint8_t, std::string> _compNames;
        std::unordered_map<uint32_t, size_t> _entitiesNetworkId;
};

#endif /* !RTYPE_CLIENT_HPP_ */
