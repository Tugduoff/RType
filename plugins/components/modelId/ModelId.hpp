/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ModelId.hpp file
*/

#ifndef MODEL_ID_HPP
    #define MODEL_ID_HPP

    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
    #include "utils/ComponentUtils.hpp"
    #include <libconfig.h++>
    #include <iostream>
    #include <memory>
    #include <string>
    #include <vector>

namespace Components {
    /**
     * @class ModelId
     * 
     * @brief The ModelId class is a component that holds the ID of a model.
     */
    class ModelId : public AComponent<ModelId> {
    public:
        static constexpr std::size_t MAX_ID_SIZE = 20; // Fixed size for network

        ModelId(const std::string &id = "")
        :   AComponent("ModelId"),
            id(id)
        {
            std::cerr << "Creating ModelId component with value: " << id << std::endl;
        }

        ModelId(const libconfig::Setting &config) : ModelId()
        {
            if (!config.lookupValue("id", id))
                std::cerr << "Warning: 'id' not found in config. Make sure to fill it.\n";
        }

        std::size_t getSize() const override { return MAX_ID_SIZE; }

        /**
         * @brief Serializes the sprite ID into a fixed-size vector of 20 bytes.
         *        If the id is shorter than 20 bytes, it will be padded with 0s.
         */
        virtual std::vector<uint8_t> serialize() override {
            std::vector<uint8_t> data(MAX_ID_SIZE);

            std::string serializedId;
            serializedId.resize(MAX_ID_SIZE);
            std::copy(id.begin(), id.end(), serializedId.begin());

            std::copy(serializedId.begin(), serializedId.end(), data.begin());

            std::cerr << "Serialized ModelId: " << id << " data: " << std::endl;
            for (auto &byte : data) {
                std::cerr << (unsigned)byte << " ";
            }
            std::cerr << std::endl;

            return data;
        }

        /**
         * @brief Deserializes the sprite ID from a fixed-size vector of 20 bytes.
         *        Trailing 0 bytes will be ignored.
         * 
         * @param data A vector of 20 bytes to deserialize into a string.
         */
        virtual void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != MAX_ID_SIZE)
                throw std::runtime_error("Invalid data size for ModelId deserialization");

            id = std::string(data.begin(), data.end());
            id.erase(std::find(id.begin(), id.end(), '\0'), id.end());

            std::cerr << "Deserialized ModelId: " << id << " data: " << std::endl;
            for (auto &byte : data) {
                std::cerr << (unsigned)byte << " ";
            }
            std::cerr << std::endl;
        }

        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            std::vector<std::any> args
        ) override {
            if (args.size() < 1)
                throw std::runtime_error("Invalid number of arguments for ModelId component");

            std::string modelId = std::any_cast<std::string>(args[0]);

            attachAndUpdateComponent<Components::ModelId>(
                engine, to,
                modelId
            );
        }

        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            libconfig::Setting &config
        ) override {
            std::string modelId = "";

            if (!config.lookupValue("id", modelId)) {
                std::cerr << "Warning: 'id' not found in config. Make sure to fill it.\n";
            }

            std::cerr << "Adding ModelId component to entity: " << to << " with value: " << modelId << std::endl;

            attachAndUpdateComponent<Components::ModelId>(
                engine, to,
                modelId
            );
        }

        std::string id;
    };
}

#endif // MODEL_ID_HPP
