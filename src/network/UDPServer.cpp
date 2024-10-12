/*
** EPITECH PROJECT, 2024
** RType
** File description:
** udp server
*/
#include "UDPServer.hpp"
#include <iostream>

#include "../../plugins/components/IComponent.hpp"
#include "../../plugins/components/position/Position.hpp"
#include "../../plugins/components/velocity/Velocity.hpp"

using boost::asio::ip::udp;

// --- PUBLIC --- //

UDPServer::UDPServer(boost::asio::io_context& io_context, short port,
                     std::unordered_map<std::string,  std::unique_ptr<Components::IComponent>> &components)
    : socket_(io_context, udp::endpoint(udp::v4(), port)), io_context_(io_context) {
    for (auto& component : components)
        __component_names.push_back(component.first);

    size_max = get_size_max();
    std::cout << "--- Server started on port " << port << ", package size_max = " << size_max << std::endl;
    start_receive();
}

// --- PRIVATE --- //

void UDPServer::start_receive() {
    socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                std::string message(recv_buffer_.data(), bytes_recvd);
                std::cout << "Received message from client (" << remote_endpoint_.address().to_string() 
                            << ":" << remote_endpoint_.port() << "): " << message << std::endl;

                if (std::find(client_endpoints.begin(), client_endpoints.end(), remote_endpoint_) == client_endpoints.end()) {
                    client_endpoints.push_back(remote_endpoint_);
                    std::cout << "New client added: " << remote_endpoint_.address().to_string() 
                                << ":" << remote_endpoint_.port() << std::endl;
                    client_responses[remote_endpoint_] = true;
                    is_disconnected[remote_endpoint_] = false;
                    checking_client(remote_endpoint_);
                } else {
                    client_responses[remote_endpoint_] = true;
                }

                if (message == "pong") {
                    std::cout << "Received pong from client: " << remote_endpoint_.address().to_string() 
                                << ":" << remote_endpoint_.port() << std::endl;
                    client_responses[remote_endpoint_] = true;
                }
                send_components_infos();
            }
            start_receive();
        });
}

void UDPServer::send_components_infos() {
    // Total components
    uint16_t total_components = static_cast<uint16_t>(__component_names.size());
    total_components = htons(total_components);

    socket_.async_send_to(
        boost::asio::buffer(&total_components, 2), remote_endpoint_,
        [this](boost::system::error_code ec, std::size_t) {
            if (!ec)
                std::cout << "Total components sent to client." << std::endl;
        }
    );

    // Size max
    std::size_t max_name_length = 0;
    for (const auto& component_name : __component_names) {
        max_name_length = std::max(max_name_length, component_name.size());
    }
    uint8_t max_name_length_byte = static_cast<uint8_t>(max_name_length);

    socket_.async_send_to(
        boost::asio::buffer(&max_name_length_byte, 1), remote_endpoint_,
        [this, max_name_length_byte](boost::system::error_code ec, std::size_t) {
            if (!ec)
                std::cout << "Size max sent to client: " << static_cast<int>(max_name_length_byte) << std::endl;
        }
    );

    // Components
    int i = 0;
    for (const auto& component_name : __component_names) {
        uint8_t index = static_cast<uint8_t>(i);

        std::vector<uint8_t> buffer(1 + component_name.size());
        buffer[0] = index;
        std::memcpy(buffer.data() + 1, component_name.data(), component_name.size());

        socket_.async_send_to(
            boost::asio::buffer(buffer), remote_endpoint_,
            [this, index, component_name](boost::system::error_code ec, std::size_t) {
                if (!ec)
                    std::cout << "Sent index: " << static_cast<int>(index) << ", component name: " << component_name << std::endl;
            }
        );
        i++;
    }

    /* v TESTS THAT MUST NOT BE MERGED v */

    // Test entity creation
    Engine::GameEngine gameEngine;
    ECS::Registry& reg = gameEngine.getRegistry();
    ECS::Entity entity_test = reg.entityManager().spawnEntity();
    ECS::Entity entity_test2 = reg.entityManager().spawnEntity();
    create_entity(entity_test);
    create_entity(entity_test2);
    create_entity(entity_test);

    gameEngine.loadSystems("./plugins/bin/systems/configSystems.cfg");

    std::unique_ptr<Components::Position> position = gameEngine.newComponent<Components::Position>(10, 20, 1);
    std::unique_ptr<Components::Velocity> velocity = gameEngine.newComponent<Components::Velocity>(2, 1);

    reg.componentManager().addComponent<Components::Position>(entity_test, std::move(position));
    reg.componentManager().addComponent<Components::Velocity>(entity_test2, std::move(velocity));

    for (const auto& component : gameEngine.getComponents())
        attach_component(entity_test2, *(component.second));
}

std::size_t UDPServer::get_size_max() {
    auto max_name_length = std::max_element(__component_names.begin(), __component_names.end(),
        [](const auto& a, const auto& b) {
            return a.size() < b.size();
        });

    return max_name_length != __component_names.end() ? max_name_length->size() : 0;
}

void UDPServer::checking_client(const udp::endpoint& client) {
    std::unique_ptr<boost::asio::steady_timer> timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(60)));
    timer->async_wait([this, client](const boost::system::error_code& ec) {
        if (!ec) {
            if (!client_responses[client] && !is_disconnected[client]) {
                std::cout << "Client did not respond to ping, disconnecting: " 
                            << client.address().to_string() << ":" << client.port() << std::endl;
                remove_client(client);
            } else {
                client_responses[client] = false;
                send_ping(client);
            }
            checking_client(client);
        }
    });
    client_timers[client] = std::move(timer);
}

void UDPServer::send_ping(const udp::endpoint& client) {
    std::string ping_message = "ping";
    socket_.async_send_to(boost::asio::buffer(ping_message), client,
        [this, client](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                std::cout << "Sent ping to client: " << client.address().to_string() << ":" << client.port() << std::endl;
                start_pong_timer(client);
            }
        }
    );
}

void UDPServer::start_pong_timer(const udp::endpoint& client) {
    std::unique_ptr<boost::asio::steady_timer> pong_timer(new boost::asio::steady_timer(io_context_, std::chrono::seconds(10)));
    pong_timer->async_wait([this, client](const boost::system::error_code& ec) {
        if (!ec) {
            if (!client_responses[client] && !is_disconnected[client]) {
                std::cout << "Client did not respond to ping (no pong), disconnecting: " 
                            << client.address().to_string() << ":" << client.port() << std::endl;
                remove_client(client);
            }
        }
    });
    pong_timers[client] = std::move(pong_timer);
}

void UDPServer::remove_client(const udp::endpoint& client) {
    if (!is_disconnected[client]) {
        is_disconnected[client] = true;

        client_endpoints.erase(std::remove(client_endpoints.begin(), client_endpoints.end(), client), client_endpoints.end());
        client_timers.erase(client);
        pong_timers.erase(client);
        client_responses.erase(client);

        std::cout << "Client disconnected: " << client.address().to_string() << ":" << client.port() << std::endl;

        std::string deconnection_message = "You have been disconnected :(";
        socket_.async_send_to(boost::asio::buffer(deconnection_message), client,
            [this, client](boost::system::error_code ec, std::size_t) {
                if (!ec)
                    std::cout << "Client " << client.address().to_string() << ":" << client.port() << " is now aware of their disconnection" << std::endl;
            }
        );
    }
}

void UDPServer::create_entity(ECS::Entity &entity) {
    uint8_t opcode = 0x0;
    uint16_t entity_id = static_cast<uint16_t>(entity);
    entity_id = htons(entity_id);

    std::array<uint8_t, 3> message;
    message[0] = opcode;
    std::memcpy(&message[1], &entity_id, sizeof(entity_id));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, entity_id](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t id = ntohs(entity_id);
                std::cout << "Entity " << static_cast<int>(id) << " created." << std::endl;
            }
        }
    );
}

void UDPServer::delete_entity(ECS::Entity &entity) {
    uint8_t opcode = 0x1;
    uint16_t entity_id = static_cast<uint16_t>(entity);
    entity_id = htons(entity_id);

    std::array<uint8_t, 3> message;
    message[0] = opcode;
    std::memcpy(&message[1], &entity_id, sizeof(entity_id));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, entity_id](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t id = ntohs(entity_id);
                std::cout << "Entity " << static_cast<int>(id) << " delete." << std::endl;
            }
        }
    );
}

void UDPServer::attach_component(ECS::Entity &entity, Components::IComponent &component) {
    uint8_t opcode = 0x2;
    uint16_t entity_id = static_cast<uint16_t>(entity);
    entity_id = htons(entity_id);

    const std::string component_name = component.getId();

    auto it = std::find(__component_names.begin(), __component_names.end(), component_name);
    if (it == __component_names.end()) {
        std::cerr << "Error 0x2: Component '" << component_name << "' not found." << std::endl;
        return;
    }

    uint8_t index = static_cast<uint8_t>(std::distance(__component_names.begin(), it));
    uint16_t component_id = index;
    component_id = htons(component_id);

    std::array<uint8_t, 5> message;
    message[0] = opcode;
    std::memcpy(&message[1], &entity_id, sizeof(entity_id));
    std::memcpy(&message[3], &component_id, sizeof(component_id));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, entity_id, component_id](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t e_id = ntohs(entity_id);
                uint16_t c_id = ntohs(component_id);
                std::cout << "Attach component [" << static_cast<int>(c_id) << "] to entity [" << static_cast<int>(e_id) << "]." << std::endl;
            }
        }
    );
}

void UDPServer::update_component(ECS::Entity &entity, Components::IComponent &component) {
    uint8_t opcode = 0x3;
    uint16_t entity_id = static_cast<uint16_t>(entity);
    entity_id = htons(entity_id);

    const std::string component_name = component.getId();

    auto it = std::find(__component_names.begin(), __component_names.end(), component_name);
    if (it == __component_names.end()) {
        std::cerr << "Error 0x2: Component '" << component_name << "' not found." << std::endl;
        return;
    }

    uint8_t index = static_cast<uint8_t>(std::distance(__component_names.begin(), it));
    uint16_t component_id = index;
    component_id = htons(component_id);

    // add component data

    std::array<uint8_t, 5> message;
    message[0] = opcode;
    std::memcpy(&message[1], &entity_id, sizeof(entity_id));
    std::memcpy(&message[3], &component_id, sizeof(component_id));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, entity_id, component_id](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t e_id = ntohs(entity_id);
                uint16_t c_id = ntohs(component_id);
                std::cout << "update component [" << static_cast<int>(c_id) << "] of entity [" << static_cast<int>(e_id) << "]." << std::endl;
            }
        }
    );
}

void UDPServer::detach_component(ECS::Entity &entity, Components::IComponent &component) {
    uint8_t opcode = 0x4;
    uint16_t entity_id = static_cast<uint16_t>(entity);
    entity_id = htons(entity_id);

    const std::string component_name = component.getId();

    auto it = std::find(__component_names.begin(), __component_names.end(), component_name);
    if (it == __component_names.end()) {
        std::cerr << "Error 0x2: Component '" << component_name << "' not found." << std::endl;
        return;
    }

    uint8_t index = static_cast<uint8_t>(std::distance(__component_names.begin(), it));
    uint16_t component_id = index;
    component_id = htons(component_id);

    std::array<uint8_t, 5> message;
    message[0] = opcode;
    std::memcpy(&message[1], &entity_id, sizeof(entity_id));
    std::memcpy(&message[3], &component_id, sizeof(component_id));

    socket_.async_send_to(
        boost::asio::buffer(message), remote_endpoint_,
        [this, entity_id, component_id](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint16_t e_id = ntohs(entity_id);
                uint16_t c_id = ntohs(component_id);
                std::cout << "Detach component [" << static_cast<int>(c_id) << "] from entity [" << static_cast<int>(e_id) << "]." << std::endl;
            }
        }
    );
}
