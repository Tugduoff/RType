/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/

#ifndef UDP_SERVER_HPP
    #define UDP_SERVER_HPP

    #include <cstdint>
    #include <functional>
    #include <span>
    #include <string>
    #include <typeindex>
    #include <unordered_map>
    #include <unordered_set>
    #include <vector>
    #include "ECS/entity/Entity.hpp"
    #include "../GameEngine/GameEngine.hpp"
    #include "GameEngine/ComponentsGetter.hpp"
    #include "boost/asio/detail/chrono.hpp"
    #include "boost/asio/io_context.hpp"
    #include "boost/asio/ip/udp.hpp"
    #include "boost/asio/steady_timer.hpp"

using boost::asio::ip::udp;

class UDPServer {
    public:
        /**
        * @brief Constructs a new UDPServer object.
        * 
        * Initializes the UDP socket, starts receiving data, and configures server components.
        * 
        * @param io_context The Boost.Asio context for managing asynchronous operations.
        * @param port The port number on which the server will listen for incoming connections.
        * @param idStringToType A map associating component names to their type index.
        */
        UDPServer(
            boost::asio::io_context& io_context,
            short port,
            std::unordered_map<std::string, std::type_index> &idStringToType,
            std::function<const std::vector<ECS::Entity> &()> entityLister,
            ComponentsGetter compGetter
        );

        /**
        * @brief Starts receiving data from clients asynchronously.
        * 
        * Continuously waits for incoming messages from any client. When a message is received, processes it and recursively calls itself.
        */
        void start_receive(Engine::GameEngine &engine);

        void sendNextFrame();

        /**
         * @brief Check if any clients requested to start the game
         */
        bool gameRunning() const noexcept;

        /**
         * @brief Lock _engineMutex
         * 
         * @note This function has to be called whenever there are modifications made to the engine
         * 
         * @note This function is a blocking call until the mutex is unlocked
         */
        void lockMutex();

        /**
         * @brief Unlock _engineMutex
         * 
         * @note Be sure that the mutex is locked beforehand
         */
        void unlockMutex();

        /**
         * @brief Create a new player entity when a new client connects
         * 
         * @param engine The game engine
         */
        void createNewEntity(Engine::GameEngine &engine);

    private:
        struct ComponentInfo {
            std::string name;
            uint16_t networkId;
        };

        struct ClientInfo {
            enum class State {
                UNKNOWN,
                INIT,
                STARTED,
            };

            ClientInfo(boost::asio::io_context &ctx) : state(State::UNKNOWN), entity(0), timer(ctx, boost::asio::chrono::milliseconds(500)) {}
            ClientInfo(boost::asio::io_context &ctx, State s) : state(s), entity(0), timer(ctx, boost::asio::chrono::milliseconds(500)) {}

            ClientInfo(boost::asio::io_context &ctx, const ComponentsGetter &getter) : state(State::UNKNOWN), entity(0), timer(ctx, boost::asio::chrono::milliseconds(500))
            {
                for (auto const &[type, _] : getter) {
                    used_types.insert(type);
                }
            }
            void setEntity(ECS::Entity entity) { this->entity = entity; }

            std::atomic<State> state;
            std::unordered_set<std::type_index> used_types;
            ECS::Entity entity;
            boost::asio::steady_timer timer;
        };

        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        boost::asio::io_context& io_context_;
        std::array<uint8_t, 1024> recv_buffer_;
        std::unordered_map<std::type_index, ComponentInfo> _comps_info;
        std::unordered_map<std::string, std::type_index> __idStringToType;
        std::unordered_map<udp::endpoint, ClientInfo> _clients;

        std::unordered_map<size_t, uint32_t> _entitiesNetworkId;
        uint32_t _nextNetworkId = 0;

        std::function<const std::vector<ECS::Entity> &()> _listEntities;
        ComponentsGetter _compGetter;

        bool _isGameRunning;

        std::mutex _engineMutex;


        // --- Helpers --- //

        /**
        * @brief Retrieves the maximum size of component names.
        * 
        * @return The size of the longest component name.
        */
        std::size_t __get_size_max();

        /**
        * @brief Sends a message to the remote client.
        * 
        * @param message A vector of bytes representing the message to be sent.
        */
        void __send_message(const std::span<const uint8_t>& message);


        // --- Client Init --- //

        /**
        * @brief Sends information about all components to the connected client.
        */
        void __send_components_infos();

        /**
         * @brief Call asynchronously __send_nb_components_message periodically
         */
        void __send_component_nb_loop(const udp::endpoint &ep);

        /**
        * @brief Sends the total number of components to the client.
        */
        void __send_total_components();

        /**
        * @brief Sends the size of the longest component name to the client.
        */
        void __send_size_max();

        /**
        * @brief Sends the list of component names to the client.
        */
        void __send_components();


        // --- Client Activity --- //

        /**
        * @brief Checks the connection status of a specific client.
        * 
        * @param client The UDP endpoint of the client.
        */
        void __checking_client(const udp::endpoint& client);

        /**
        * @brief Sends a ping to the client to test connectivity.
        * 
        * @param client The UDP endpoint of the client.
        */
        void __send_ping(const udp::endpoint& client);

        /**
        * @brief Starts a timer to wait for the client's pong response.
        * 
        * @param client The UDP endpoint of the client.
        */
        void __start_pong_timer(const udp::endpoint& client);

        /**
        * @brief Removes a disconnected client from the server and informs them.
        * 
        * @param client The UDP endpoint of the client to be removed.
        */
        void __remove_client(const udp::endpoint& client);

        /**
         * @brief Store a new client in the internal storage of this class
         */
        void __init_new_client();

        /**
         * @brief Send to the client the number of used components
         * @brief in the game.
         *
         * @note Also indicates that the server is ready to start a game
         */
        void __send_nb_components_message(const udp::endpoint &client);

        /**
         * @brief Suppress all notifications about a certain
         * @brief component type for the client
         */
        void __ignoreComponent(ClientInfo &info);

        /**
         * @brief Does everything required to set up a new client
         */
        void __add_new_client();

        /**
         * @brief Send to one client the entity created instruction
         */
        void __send_entity_created_message(
            uint32_t networkId,
            const udp::endpoint &client
        );

        void __send_attach_component_message(
            uint32_t entity_netId,
            uint16_t comp_netId,
            const udp::endpoint &client
        );

        void __send_update_component(
            uint32_t entity_netId,
            uint16_t comp_netId,
            const std::vector<uint8_t> &data,
            const udp::endpoint &client
        );


    public:

        // --- Entity --- //

        /**
        * @brief Creates a new entity on the server.
        * 
        * @param entity A reference to the entity being created.
        */
        void create_entity(const ECS::Entity &entity);
    
        /**
        * @brief Deletes an existing entity on the server.
        * 
        * @param entity A reference to the entity being deleted.
        */
        void delete_entity(const ECS::Entity &entity);


        // --- Component --- //

        /**
        * @brief Attaches a component to an entity.
        * 
        * @param entity A reference to the entity receiving the component.
        * @param component A reference to the component being attached.
        */
        void attach_component(size_t entity, std::type_index component);
    
        /**
        * @brief Updates the component of an entity.
        * 
        * @param entity A reference to the entity whose component is being updated.
        * @param component A reference to the updated component.
        */
        void update_component(size_t entity, std::string name, std::vector<uint8_t> data);
    
        /**
        * @brief Detaches a component from an entity.
        * 
        * @param entity A reference to the entity losing the component.
        * @param component A reference to the component being detached.
        */
        void detach_component(size_t entity, std::type_index component);

        void receiveUpdateComponent(Engine::GameEngine &engine, std::span<const uint8_t> operation);

        void updateIdStringToType(std::unordered_map<std::string, std::type_index> &idStringToType) {
            __idStringToType = idStringToType;

            _comps_info.clear();
            uint16_t networkId = 0;
            for (const auto &[name, idx] : idStringToType) {
                _comps_info.emplace(idx, ComponentInfo {name, networkId});
                networkId++;
            }
        }
};

#endif /* !UDP_SERVER_HPP_ */
