/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Chrono.hpp
*/

#ifndef CHRONO_HPP
    #define CHRONO_HPP

    #include <iostream>
    #include <unistd.h>
    #include <chrono>

class Chrono {
    public:

        Chrono() : _startTime(std::chrono::high_resolution_clock::now()), _paused(false), _pausedTime(std::chrono::high_resolution_clock::now()), _pauseDuration(std::chrono::milliseconds(0)) {};
        ~Chrono() = default;

        size_t getElapsedTime() {
            auto currentTime = std::chrono::high_resolution_clock::now() - _pauseDuration;
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _startTime);
            return duration.count();
        }
        void restart() {
            _startTime = std::chrono::high_resolution_clock::now();
            _pausedTime = std::chrono::high_resolution_clock::now();
            _pauseDuration = (std::chrono::milliseconds)0;
            _paused = false;
        }
        void pause() {
            _pausedTime = std::chrono::high_resolution_clock::now();
            _paused = true;
        }
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