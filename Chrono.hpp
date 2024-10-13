/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Chrono.hpp
*/

#ifndef CHRONO_HPP
    #define CHRONO_HPP

    #include <iostream>
    #include <chrono>

/**
 * @class Chrono
 * 
 * @brief Chrono class that handles time.
 * 
 * This class is responsible for handling time and can be used to measure the time elapsed between two points.
 * It can also be used to pause and resume the time.
 */
class Chrono {
    public:

        /**
         * @brief Construct a new Chrono object
         * 
         * @note This constructor will initialize the start time to the current time.
         * It will also initialize the paused state to false.
         */
        Chrono() :
            _startTime(std::chrono::high_resolution_clock::now()),
            _paused(false),
            _pausedTime(std::chrono::high_resolution_clock::now()),
            _pauseDuration(std::chrono::milliseconds(0)) {};

        /**
         * @brief Default destructor
         */
        ~Chrono() = default;

        /**
         * @brief Get the elapsed time
         * 
         * @return size_t The elapsed time in milliseconds
         * 
         * @note This function will return the elapsed time in milliseconds since the last clock restart.
         */
        size_t getElapsedTime() {
            auto currentTime = std::chrono::high_resolution_clock::now() - _pauseDuration;
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _startTime);
            return duration.count();
        }

        /**
         * @brief Restart the chrono
         * 
         * @note This function will restart the chrono.
         */
        void restart() {
            _startTime = std::chrono::high_resolution_clock::now();
            _pausedTime = std::chrono::high_resolution_clock::now();
            _pauseDuration = (std::chrono::milliseconds)0;
            _paused = false;
        }

        /**
         * @brief Pause the chrono
         */
        void pause() {
            _pausedTime = std::chrono::high_resolution_clock::now();
            _paused = true;
        }

        /**
         * @brief Resume the chrono
         * 
         * @note When the chrono is resumed, getElapsedTime returns the time since
         * the last restart without the pause time.
         */
        void resume() {
            _pauseDuration += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _pausedTime);
            _paused = false;
        }

    private:

        std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
        bool _paused;
        std::chrono::time_point<std::chrono::high_resolution_clock> _pausedTime;
        std::chrono::milliseconds _pauseDuration;

};

#endif /* CHRONO_HPP */
