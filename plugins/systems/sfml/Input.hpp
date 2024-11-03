/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Input.hpp file
*/

#ifndef INPUT_HPP
    #define INPUT_HPP

    #include <SFML/Graphics.hpp>
    #include <SFML/Window.hpp>
    #include <SFML/System.hpp>
    #include <libconfig.h++>
    #include "utils/EntityActions.hpp"
    #include "utils/Actions.hpp"
    #include "utils/Keys.hpp"
    #include "GameEngine/GameEngine.hpp"

    #include "components/controllable/Controllable.hpp"

namespace Systems {
    /**
     * @class Input
     * 
     * @brief System class from Systems that handles the input.
     * 
     * This system is responsible for handling input events and updating entities based on the input.
     */
    class Input {
        public:

            Input();
            Input(libconfig::Setting &config);
            ~Input() = default;

            void run(Engine::GameEngine &engine, sf::RenderWindow &window);
            void init(Engine::GameEngine &engine);

        private:

            /**
             * @brief Handle input events
             * 
             * @param engine The game engine
             * @param event The input event
             * 
             * @note This function will handle input events and update the entities based on the input.
             */
            void handleInput(Engine::GameEngine &engine, sf::Event &event);

            /**
             * @brief Handle input pressed
             * 
             * @param inputs The inputs array
             * @param actions The actions array
             * @param index The index of the input
             * 
             * @note This function will check whether the key pressed is an input or an action and update the corresponding array.
             */
            void handleInputPressed(std::array<bool, 4> &inputs, std::array<bool, 10> &actions, int index);

            /**
             * @brief Handle input released
             * 
             * @param inputs The inputs array
             * @param actions The actions array
             * @param index The index of the input
             * 
             * @note This function will check whether the key released is an input or an action and update the corresponding array.
             */
            void handleInputReleased(std::array<bool, 4> &inputs, std::array<bool, 10> &actions, int index);

            EntityAction determinePressedAction(bool forwardInput, bool backwardInput, bool rightInput, bool leftInput, bool shootInput, Action action);
            EntityAction determineReleasedAction(bool forwardInput, bool backwardInput, bool rightInput, bool leftInput, bool shootInput, Action action);
            EntityAction determinePressedShootAction(bool forwardInput, bool backwardInput, bool rightInput, bool leftInput);
            EntityAction determineReleasedShootAction(bool forwardInput, bool backwardInput, bool rightInput, bool leftInput, Action action);

            void updateControllable(Engine::GameEngine &engine);

            const std::unordered_map<sf::Keyboard::Key, enum Key> __sfmlToKey = {
                {sf::Keyboard::Key::Z, Key::Z}, {sf::Keyboard::Key::Q, Key::Q}, {sf::Keyboard::Key::S, Key::S}, {sf::Keyboard::Key::D, Key::D},
                {sf::Keyboard::Key::Escape, Key::ESCAPE}, {sf::Keyboard::Key::Space, Key::SPACE}, {sf::Keyboard::Key::Enter, Key::ENTER},
                {sf::Keyboard::Key::BackSpace, Key::BACKSPACE}, {sf::Keyboard::Key::Delete, Key::DELETE_KEY},
                {sf::Keyboard::Key::Up, Key::UP_KEY}, {sf::Keyboard::Key::Down, Key::DOWN_KEY},
                {sf::Keyboard::Key::Left, Key::LEFT_KEY}, {sf::Keyboard::Key::Right, Key::RIGHT_KEY},
                {sf::Keyboard::Key::A, Key::A}, {sf::Keyboard::Key::E, Key::E}, {sf::Keyboard::Key::R, Key::R}, {sf::Keyboard::Key::T, Key::T},
                {sf::Keyboard::Key::Y, Key::Y}, {sf::Keyboard::Key::U, Key::U}, {sf::Keyboard::Key::I, Key::I}, {sf::Keyboard::Key::O, Key::O},
                {sf::Keyboard::Key::P, Key::P}, {sf::Keyboard::Key::F, Key::F}, {sf::Keyboard::Key::G, Key::G}, {sf::Keyboard::Key::H, Key::H},
                {sf::Keyboard::Key::J, Key::J}, {sf::Keyboard::Key::K, Key::K}, {sf::Keyboard::Key::L, Key::L}, {sf::Keyboard::Key::M, Key::M},
                {sf::Keyboard::Key::W, Key::W}, {sf::Keyboard::Key::X, Key::X}, {sf::Keyboard::Key::C, Key::C}, {sf::Keyboard::Key::V, Key::V},
                {sf::Keyboard::Key::B, Key::B}, {sf::Keyboard::Key::N, Key::N},
                {sf::Keyboard::Key::Num0, Key::NUM_0}, {sf::Keyboard::Key::Num1, Key::NUM_1}, {sf::Keyboard::Key::Num2, Key::NUM_2},
                {sf::Keyboard::Key::Num3, Key::NUM_3}, {sf::Keyboard::Key::Num4, Key::NUM_4}, {sf::Keyboard::Key::Num5, Key::NUM_5},
                {sf::Keyboard::Key::Num6, Key::NUM_6}, {sf::Keyboard::Key::Num7, Key::NUM_7}, {sf::Keyboard::Key::Num8, Key::NUM_8},
                {sf::Keyboard::Key::Num9, Key::NUM_9},
            };
    };
};

#endif // INPUT_HPP
