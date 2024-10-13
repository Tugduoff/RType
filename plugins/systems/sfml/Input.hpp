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
    #include <libconfig.h++>
    #include "Keys.hpp"
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

            const std::unordered_map<sf::Keyboard::Key, enum Key> __sfmlToKey = {
                {sf::Keyboard::Z, Key::Z}, {sf::Keyboard::Q, Key::Q}, {sf::Keyboard::S, Key::S}, {sf::Keyboard::D, Key::D},
                {sf::Keyboard::Escape, Key::ESCAPE}, {sf::Keyboard::Space, Key::SPACE}, {sf::Keyboard::Enter, Key::ENTER},
                {sf::Keyboard::BackSpace, Key::BACKSPACE}, {sf::Keyboard::Delete, Key::DELETE_KEY},
                {sf::Keyboard::Up, Key::UP_KEY}, {sf::Keyboard::Down, Key::DOWN_KEY},
                {sf::Keyboard::Left, Key::LEFT_KEY}, {sf::Keyboard::Right, Key::RIGHT_KEY},
                {sf::Keyboard::A, Key::A}, {sf::Keyboard::E, Key::E}, {sf::Keyboard::R, Key::R}, {sf::Keyboard::T, Key::T},
                {sf::Keyboard::Y, Key::Y}, {sf::Keyboard::U, Key::U}, {sf::Keyboard::I, Key::I}, {sf::Keyboard::O, Key::O},
                {sf::Keyboard::P, Key::P}, {sf::Keyboard::F, Key::F}, {sf::Keyboard::G, Key::G}, {sf::Keyboard::H, Key::H},
                {sf::Keyboard::J, Key::J}, {sf::Keyboard::K, Key::K}, {sf::Keyboard::L, Key::L}, {sf::Keyboard::M, Key::M},
                {sf::Keyboard::W, Key::W}, {sf::Keyboard::X, Key::X}, {sf::Keyboard::C, Key::C}, {sf::Keyboard::V, Key::V},
                {sf::Keyboard::B, Key::B}, {sf::Keyboard::N, Key::N},
                {sf::Keyboard::Num0, Key::NUM_0}, {sf::Keyboard::Num1, Key::NUM_1}, {sf::Keyboard::Num2, Key::NUM_2},
                {sf::Keyboard::Num3, Key::NUM_3}, {sf::Keyboard::Num4, Key::NUM_4}, {sf::Keyboard::Num5, Key::NUM_5},
                {sf::Keyboard::Num6, Key::NUM_6}, {sf::Keyboard::Num7, Key::NUM_7}, {sf::Keyboard::Num8, Key::NUM_8},
                {sf::Keyboard::Num9, Key::NUM_9},
            };
    };
};

#endif // INPUT_HPP
