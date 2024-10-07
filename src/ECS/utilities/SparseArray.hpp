/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** SparseArray.hpp file
*/

#ifndef SPARSE_ARRAY_HPP
    #define SPARSE_ARRAY_HPP

    #include <cstddef>
    #include <functional>
    #include <unordered_map>
    #include <functional>
    #include <list>
    #include <typeindex>
    #include <vector>
    #include <stdexcept>
    #include <algorithm>
    #include <memory>
    #include "ValueIterator.hpp"

/** @class SparseArray
 * @brief A sparse array that can hold Components of a specific type
 *
 * @tparam Component The type of component to store in the array
 *
 * This class is used to store components in a sparse array.
 * A sparse Array is a data structure that can hold components of a specific type whilst
 * allowing for gaps in the array. This is useful for ECS systems where entities can have
 * different components and we want to avoid having to store a component for every entity.
 * Gaps in the array represent entities that do not have a component of the specified type.
 */
template <class Component>
class SparseArray {
    public:

        /**
         * @brief Type of a function that can be used to get a Component instance
         */
        using comp_ctor = std::function<Component *()>;
    
        using updateCallback_t =  std::function<void(
            std::size_t index,
            std::type_index type,
            const Component &modified
        )>;

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

        /**
         * @brief Copy and move assignment operators
         *
         * @param other The SparseArray to copy or move
         * @return SparseArray& A reference to the SparseArray
         */
        SparseArray &operator=(const SparseArray &other) = default;
        SparseArray &operator=(SparseArray &&other) = default;

        std::unique_ptr<Component> &operator[](std::size_t index) {
            if (index >= __data.size())
                throw std::out_of_range("Index out of range");
            return __data[index];
        }

        /**
         * @brief Function which notifies all event listeners
         * @brief about a specific component instance's state
         *
         * @param index The index of the updated component
         * @param modif A function which will modify the component
         *
         * @return `modif`'s return value
         */
        template<typename T>
        T updateComponent(
            std::size_t index,
            std::function<T(Component &)> modif
        )
        {
            auto ret = modif((*this)[index]);
            updateComponent(index);
            return ret;
        }

        /**
         * @brief Function which notifies all event listeners
         * @brief about a specific component instance's state
         *
         * @param index The index of the updated component
         */
        void updateComponent(std::size_t index) {
            auto modifiedComponent = (*this)[index];

            for (auto &callback : __updateCallbacks) {
                callback(
                    index,
                    std::type_index(typeid(Component)),
                    modifiedComponent
                );
            }
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

        /**
         * @brief Calls the erase method on the internal map at the specified index
         *
         * @param index The index at which to erase the component
         */
        void erase(std::size_t index) {
            __data.erase(index);
        }

        /**
         * @brief Inserts a component at the specified index
         *
         * @param index The index at which to insert the component
         * @param component The component to insert
         */
        void insertAt(std::size_t index, std::unique_ptr<Component> &&component) {
            __data[index] = std::move(component);
        }

        /**
         * @brief Constructs a new instance of a component at the specified
         * @brief index using the registered constructor function
         *
         * @param index The index at which to construct a new component
         */
        void constructAt(std::size_t index) {
            __data[index] = __ctor();
        }

        /**
         * @brief Clears the SparseArray's internal map of all it's components
         */
        void clear() {
            __data.clear();
        }

        /**
         * @brief Deletes from the SparseArray any nullptrs that would have
         * @brief been left in the internal map
         */
        void clearNulls() {
            std::erase_if(__data, [](auto const &it) { return !*it; });
        }

        /**
         * @brief Register an update callback to the sparseArray
         * 
         * The callback will be called each time the `UpdateComponent` is called
         */
        void registerUpdateCallback(updateCallback_t callback) {
            __updateCallbacks.push_back(std::move(callback));
        }

    private:

        std::unordered_map<std::size_t, std::unique_ptr<Component>> __data;
        comp_ctor __ctor;

        std::list<updateCallback_t> __updateCallbacks;
};

#endif // SPARSE_ARRAY_HPP
