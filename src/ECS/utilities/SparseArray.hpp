#ifndef SPARSE_ARRAY_HPP
    #define SPARSE_ARRAY_HPP

    #include <cstddef>
    #include <functional>
    #include <typeindex>
    #include <stdexcept>
    #include <algorithm>
    #include <memory>
    #include <vector>
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

        using value_type = std::unique_ptr<Component>;
        using vector_type = std::vector<value_type>;
        using iterator = typename vector_type::iterator;
        using const_iterator = typename vector_type::const_iterator;
        using value_iterator = ValueIterator<iterator>;
        using const_value_iterator = ValueIterator<const_iterator>;

        using CreateCallback = std::function<void(std::type_index type, size_t index)>;
        using RemoveCallback = std::function<void(std::type_index type, size_t index)>;

        SparseArray(comp_ctor constructor) : __ctor(std::move(constructor)) {}
        SparseArray(const SparseArray<Component> &other)
        : __data(other.__data.size()), __ctor(other.__ctor) {
            for (std::size_t idx = 0; idx < other.__data.size(); ++idx) {
                if (other.__data[idx]) {
                    __data[idx] = std::make_unique<Component>(*other.__data[idx]);
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
            if (index >= __data.size() || !__data[index])
                throw std::out_of_range("SparseArray index out of range: " + std::to_string(index));
            return __data.at(index);
        }

        // Iterators

        auto size() const { return __data.size(); }

        auto begin() const { return const_value_iterator(__data.begin()); }
        auto begin() { return value_iterator(__data.begin()); }
        auto cbegin() const { return const_value_iterator(__data.cbegin()); }

        auto end() { return value_iterator(__data.end()); }
        auto end() const { return const_value_iterator(__data.end()); }
        auto cend() const { return const_value_iterator(__data.cend()); }

        // Methods

        /**
         * @brief Calls the erase method on the internal vector at the specified index
         *
         * @param index The index at which to erase the component
         */
        void erase(std::size_t index) {
            if (index < __data.size() && __data[index]) {
                __data[index].reset();
                __callAllRemove(index);
            }
        }

        /**
         * @brief Inserts a component at the specified index
         *
         * @param index The index at which to insert the component
         * @param component The component to insert
         */
        void insertAt(std::size_t index, std::unique_ptr<Component> &&component) {
            if (index >= __data.size())
                __data.resize(index + 1);
            __data[index] = std::move(component);
            __callAllCreate(index);
        }

        /**
         * @brief Constructs a new instance of a component at the specified index
         *        using the registered constructor function
         *
         * @param index The index at which to construct a new component
         */
        void constructAt(std::size_t index) {
            this->insertAt(index, std::unique_ptr<Component>(__ctor()));
        }

        /**
         * @brief Clears the SparseArray's internal vector of all its components
         */
        void clear() {
            for (std::size_t index = 0; index < __data.size(); ++index) {
                if (__data[index]) {
                    __callAllRemove(index);
                }
            }
            __data.clear();
        }

        /**
         * @brief Deletes from the SparseArray any nullptrs that would have
         *        been left in the internal vector
         */
        void clearNulls() {
            __data.erase(std::remove(__data.begin(), __data.end(), nullptr), __data.end());
        }

        /**
         * @brief Register a function that will be called
         *        each time a component is created
         */
        void registerCreateCallback(CreateCallback cb) {
            __createCallbacks.push_back(std::move(cb));
        }

        /**
         * @brief Register a function that will be called
         *        each time a component is removed
         */
        void registerRemoveCallback(RemoveCallback cb) {
            __removeCallbacks.push_back(std::move(cb));
        }

    private:

        /**
         * @brief Calls all registered create callbacks
         */
        void __callAllCreate(std::size_t index) {
            for (auto const &callback : __createCallbacks) {
                callback(typeid(Component), index);
            }
        }

        /**
         * @brief Calls all registered remove callbacks
         */
        void __callAllRemove(std::size_t index) {
            for (auto const &callback : __removeCallbacks) {
                callback(typeid(Component), index);
            }
        }

        std::vector<std::unique_ptr<Component>> __data;
        comp_ctor __ctor;

        std::vector<CreateCallback> __createCallbacks;
        std::vector<RemoveCallback> __removeCallbacks;
};

#endif // SPARSE_ARRAY_HPP
