/*
** EPITECH PROJECT, 2024
** RType
** File description:
** sparseArray.hpp file
*/

#ifndef SPARSE_ARRAY_HPP
    #define SPARSE_ARRAY_HPP

#include <vector>
#include <stdexcept>
#include <optional>
#include <algorithm>
#include <memory>
#include <typeindex>
#include <typeinfo>

template <typename Component>
class sparseArray {
    public:

        // Types
        // N.B: Value type is generally optional in a sparse array

        using value_type = std::optional<Component>;
        using reference_type = value_type &;
        using const_reference_type = const value_type &;
        using container_t = std::vector<value_type>;

        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

        // Ctors & Dtors

        sparseArray() = default;
        sparseArray(const sparseArray &) = default;
        sparseArray(sparseArray &&) noexcept = default;
        ~sparseArray() = default;

        // operators

        sparseArray &operator=(const sparseArray &) = default;
        sparseArray &operator=(sparseArray &&) noexcept = default;

        reference_type operator[](size_type index);
        const_reference_type operator[](size_type index) const;

        // Methods

        iterator begin() { return __data.begin(); }
        const_iterator begin() const { return __data.begin(); }
        const_iterator cbegin() const { return __data.cbegin(); }

        iterator end() { return __data.end(); }
        const_iterator end() const { return __data.end(); }
        const_iterator cend() const { return __data.cend(); }

        size_type size() const { return __data.size(); }

        reference_type insert_at(size_type pos, Component const &);
        reference_type insert_at(size_type pos, Component &&);

        template <class... Params>
        reference_type emplace_at(size_type pos, Params &&...);

        void erase(size_type pos);

        size_type get_index(value_type const &) const;

    private:

        // Data

        container_t __data;
};

// Implementation of the methods

template <typename Component>
typename sparseArray<Component>::reference_type
sparseArray<Component>::operator[](size_type index) {
    if (index >= __data.size())
        throw std::out_of_range("Index out of bounds");
    return __data[index];
}

template <typename Component>
typename sparseArray<Component>::const_reference_type
sparseArray<Component>::operator[](size_type index) const {
    if (index >= __data.size())
        throw std::out_of_range("Index out of bounds");
    return __data[index];
}

template <typename Component>
typename sparseArray<Component>::reference_type
sparseArray<Component>::insert_at(size_type pos, Component const &value) {
    if (pos >= __data.size())
        __data.resize(pos + 1);
    __data[pos] = value;
    return __data[pos];
}

template <typename Component>
typename sparseArray<Component>::reference_type
sparseArray<Component>::insert_at(size_type pos, Component &&value) {
    if (pos >= __data.size())
        __data.resize(pos + 1);
    __data[pos] = std::move(value);
    return __data[pos];
}

template <typename Component>
template <class... Params>
typename sparseArray<Component>::reference_type
sparseArray<Component>::emplace_at(size_type pos, Params &&...params) {
    if (pos > __data.size())
        __data.resize(pos + 1);
    if (__data[pos].has_value()) {
        using allocator_type = typename container_t::allocator_type;
        allocator_type alloc = __data.get_allocator();
        std::allocator_traits<allocator_type>::destroy(alloc, std::addressof(__data[pos].value()));
    }
    __data[pos].emplace(std::forward<Params>(params)...);
    return __data[pos];
}

template <typename Component>
void sparseArray<Component>::erase(size_type pos) {
    if (pos >= __data.size())
        throw std::out_of_range("Index out of bounds");
    if (__data[pos].has_value()) {
        using allocator_type = decltype(__data.get_allocator());
        allocator_type alloc = __data.get_allocator();
        std::allocator_traits<allocator_type>::destroy(alloc, std::addressof(__data[pos].value()));
        __data[pos].reset();
    }
}

template <typename Component>
typename sparseArray<Component>::size_type
sparseArray<Component>::get_index(value_type const &value) const {
    auto it = std::find_if(__data.begin(), __data.end(), [&](const value_type &elem) {
        return std::addressof(elem) == std::addressof(value);
    });
    if (it == __data.end())
        throw std::out_of_range("Value not found");
    return std::distance(__data.begin(), it);
}

#endif // SPARSE_ARRAY_HPP
