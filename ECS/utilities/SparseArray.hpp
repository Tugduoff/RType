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
    #include <optional>
    #include <algorithm>
    #include <memory>
    #include <typeindex>
    #include <typeinfo>

template <typename Component>
class SparseArray {
    public:

        // Types
        // N.B: Value type is generally optional in a sparse array

        using valueType = std::optional<Component>;
        using referenceType = valueType &;
        using constReferenceType = const valueType &;
        using container = std::vector<valueType>;

        using sizeType = typename container::sizeType;
        using iterator = typename container::iterator;
        using constIterator = typename container::constIterator;

        // Ctors & Dtors

        SparseArray() = default;
        SparseArray(const SparseArray &) = default;
        SparseArray(SparseArray &&) noexcept = default;
        ~SparseArray() = default;

        // operators

        SparseArray &operator=(const SparseArray &) = default;
        SparseArray &operator=(SparseArray &&) noexcept = default;

        referenceType operator[](sizeType index)
        {
            if (index >= __data.size())
                throw std::out_of_range("Index out of bounds");
            return __data[index];
        };

        constReferenceType operator[](sizeType index) const
        {
            if (index >= __data.size())
                throw std::out_of_range("Index out of bounds");
            return __data[index];
        };

        // Methods

        iterator begin() { return __data.begin(); };
        constIterator begin() const { return __data.begin(); };
        constIterator cbegin() const { return __data.cbegin(); };

        iterator end() { return __data.end(); };
        constIterator end() const { return __data.end(); };
        constIterator cend() const { return __data.cend(); };

        sizeType size() const { return __data.size(); };

        referenceType insertAt(sizeType pos, Component const &)
        {
            if (pos >= __data.size())
                __data.resize(pos + 1);
            __data[pos] = value;
            return __data[pos];
        };

        referenceType insertAt(sizeType pos, Component &&)
        {
            if (pos >= __data.size())
                __data.resize(pos + 1);
            __data[pos] = std::move(value);
            return __data[pos];
        };

        template <class... Params>
        referenceType emplaceAt(sizeType pos, Params &&...)
        {
            if (pos > __data.size())
                __data.resize(pos + 1);
            if (__data[pos].has_value()) {
                using allocatorType = typename container::allocatorType;
                allocatorType alloc = __data.get_allocator();
                std::allocator_traits<allocatorType>::destroy(alloc, std::addressof(__data[pos].value()));
            }
            __data[pos].emplace(std::forward<Params>(params)...);
            return __data[pos];
        };

        void erase(sizeType pos)
        {
            if (pos >= __data.size())
                throw std::out_of_range("Index out of bounds");
            if (__data[pos].has_value()) {
                using allocatorType = decltype(__data.get_allocator());
                allocatorType alloc = __data.get_allocator();
                std::allocator_traits<allocatorType>::destroy(alloc, std::addressof(__data[pos].value()));
                __data[pos].reset();
            }
        };

        sizeType getIndex(valueType const &) const
        {
            auto it = std::find_if(__data.begin(), __data.end(), [&](const valueType &elem) {
                return std::addressof(elem) == std::addressof(value);
            });
            if (it == __data.end())
                throw std::out_of_range("Value not found");
            return std::distance(__data.begin(), it);
        };

    private:

        // Data

        container __data;
};

#endif // SPARSE_ARRAY_HPP
