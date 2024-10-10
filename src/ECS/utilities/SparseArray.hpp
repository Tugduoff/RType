/*
** EPITECH PROJECT, 2024
** BsRtype
** File description:
** SparseArray.hpp file
*/

#ifndef SPARSE_ARRAY_HPP
    #define SPARSE_ARRAY_HPP

    #include <cstddef>
    #include <functional>
    #include <unordered_map>
    #include <stdexcept>
    #include <algorithm>
    #include <memory>
#include <vector>

template<typename It>
struct ValueIterator : public It {
    ValueIterator(It src) : It(std::move(src)) {}

    auto &operator*() const 
    {
        return It::operator*().second;
    }
};


template <class Component>
class SparseArray {
    public:

        using comp_ctor = std::function<Component *()>;

        using value_type = std::unique_ptr<Component>;
        using map_type = std::unordered_map<std::size_t, value_type>;
        using iterator = map_type::iterator;
        using const_iterator = map_type::const_iterator;
        using value_iterator = ValueIterator<iterator>;
        using const_value_iterator = ValueIterator<const_iterator>;

        SparseArray(comp_ctor constructor) : __ctor(std::move(constructor)) {}
        SparseArray(const SparseArray<Component> &other)
        : __data(), __ctor(other.__ctor) {
            for (auto &[idx, uniq] : other.__data) {
                if (!uniq) {
                    __data.emplace(idx, nullptr);
                } else {
                    __data.emplace(idx, std::make_unique<Component>(*uniq));
                }
            }
        }
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

        auto begin() const { return const_value_iterator(__data.begin()); };
        auto begin() { return value_iterator(__data.begin()); };
        auto cbegin() const { return const_value_iterator(__data.cbegin()); };
        auto end() { return value_iterator(__data.end()); };
        auto end() const { return const_value_iterator(__data.end()); };
        auto cend() const { return const_value_iterator(__data.cend()); };

        // Methods

        void erase(std::size_t index) {
            __data.erase(index);
        }

        void insertAt(std::size_t index, std::unique_ptr<Component> &&component) {
            __data[index] = std::move(component);
        }

        void constructAt(std::size_t index) {
            __data[index] = __ctor();
        }

        void clear() {
            __data.clear();
        }

        void clearNulls() {
            std::erase_if(__data, [](auto const &it) { return !*it; });
        }


    private:

        std::unordered_map<std::size_t, std::unique_ptr<Component>> __data;
        comp_ctor __ctor;
};

#endif // SPARSE_ARRAY_HPP
