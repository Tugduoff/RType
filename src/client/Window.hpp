/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Window.hpp file
*/

#ifndef WINDOW_HPP
    #define WINDOW_HPP

    #include <SFML/Graphics.hpp>
    #include <SFML/Window.hpp>
    #include <iostream>

/**
 * @class Window
 * 
 * @brief The Window class is a class that encapsulates the sfml render window class.
 * 
 * @note It holds a sf::RenderWindow object and provides a simple interface to interact with it.
 */
class Window {
    public:

        /**
         * @fn
         * @brief Constructor of the Window class
         * 
         * @param width : the width of the window
         * @param height : the height of the window
         * @param title : the title of the window
         * 
         * @note This function is the constructor of the Window class.
         * 
         * @example
         * Window win(1920, 1080, "test");
         */
        Window(unsigned int width, unsigned int height, std::string title);

        /**
         * @fn
         * @brief Destructor of the Window class
         * 
         * @note This function is the destructor of the Window class.
         * It will be called automatically when the object is destroyed.
         */
        ~Window() = default;

        /**
         * @fn
         * @brief Tells if the window is open or not
         * 
         * @return true : the window is open
         * @return false : the window is closed
         * 
         * @note This function can be called to check if the window is currently open.
         * 
         * @example
         * while (window.isOpen())
         *    std::cout << "Window is open!" << std::endl;
         */
        bool isOpen() { return __window.isOpen(); };

        /**
         * @fn
         * @brief Closes the window
         * 
         * @note This function can be called to close the window.
         * 
         * @example
         * window.close();
         */
        void close() { __window.close(); };

        /**
         * @fn
         * @brief Clears the window
         * 
         * @param color : the color to clear the window with
         * 
         * @note This function can be called to clear the window.
         * 
         * @example
         * window.clear();
         */
        void clear(sf::Color color = sf::Color(0, 0, 0, 255)) { __window.clear(color); };

        /**
         * @fn
         * @brief Displays the window
         * 
         * @note This function can be called to display the window.
         * 
         * @example
         * window.display();
         */
        void display() { __window.display(); };

        /**
         * @fn
         * @brief Polls the event
         * 
         * @return true : an event has been polled
         * @return false : no event has been polled
         * 
         * @note This function can be called to poll the event.
         * 
         * @example
         * while (window.pollEvent())
         *    std::cout << "Event polled!" << std::endl;
         */
        bool pollEvent();

    private:

        /**
         * @var sf::RenderWindow __window
         * 
         * @brief The window object of the Window class.
         * 
         * @note This variable is the window object of the Window class.
         */
        sf::RenderWindow __window;

};

#endif // WINDOW_HPP
