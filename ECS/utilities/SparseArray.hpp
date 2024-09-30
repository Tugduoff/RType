/*
** EPITECH PROJECT, 2024
** BsRtype
** File description:
** SparseArray.hpp file
*/

#ifndef SPARSE_ARRAY_HPP
    #define SPARSE_ARRAY_HPP

    #include <vector>
    #include <stdexcept>
    #include <algorithm>
    #include <memory>

template <class Component>
class SparseArray {
    public:

        // Types
        // N.B: Value type is generally optional in a sparse array

        SparseArray() = default;
        SparseArray(const SparseArray<Component> &other) : __data() {
            for (const std::unique_ptr<Component> &component : other.__data) {
                if (!component) {
                    __data.push_back(nullptr);
                    continue;
                }
                __data.push_back(std::make_unique<Component>(*component));
            }
        };
        ~SparseArray() = default;

        // Operators

        SparseArray &operator=(const SparseArray &other) = default;
        SparseArray &operator=(SparseArray &&other) = default;
        std::unique_ptr<Component> &operator[](std::size_t index) {
            if (index >= __data.size())
                throw std::out_of_range("Index out of range");
            return __data[index];
        }

        // Iterators

        auto size() const { return __data.size(); };

        auto begin() const { return __data.begin(); };
        auto begin() { return __data.begin(); };
        auto cbegin() const { return __data.cbegin(); };
        auto end() { return __data.end(); };
        auto end() const { return __data.end(); };
        auto cend() const { return __data.cend(); };

        auto back() { return __data.back(); };
        auto front() { return __data.front(); };

        // Methods

        void push_back(std::unique_ptr<Component> &&component) {
            __data.push_back(std::move(component));
        }

        void push_front(std::unique_ptr<Component> &&component) {
            __data.insert(__data.begin(), std::move(component));
        }

        void erase(std::size_t index) {
            if (index >= __data.size())
                throw std::out_of_range("Index out of range");
            __data.at(index).reset();
        }

        void insertAt(std::size_t index, std::unique_ptr<Component> &&component) {
            if (index >= __data.size()) {
                __data.resize(index + 1);
            }
            __data[index] = std::move(component);
        }

        void emplaceAt(std::size_t index, std::unique_ptr<Component> &&component) {
            if (index >= __data.size()) {
                __data.resize(index + 1);
            }
            __data[index] = std::move(component);
        }

        void clear() { __data.clear(); }

    private:

        std::vector<std::unique_ptr<Component>> __data;
};

#endif // SPARSE_ARRAY_HPP
