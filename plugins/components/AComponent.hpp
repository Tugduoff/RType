/*
** EPITECH PROJECT, 2024
** RType
** File description:
** AComponent.hpp file
*/

#ifndef ACOMPONENT_HPP
    #define ACOMPONENT_HPP

    #include "ECS/utilities/SparseArray.hpp"
    #include "IComponent.hpp"
    #include <any>

namespace Components {
    /**
     * @class AComponent
     * 
     * @brief Abstract class for all our Components.
     *
     * @tparam Concrete The derived type inheriting from AComponent
     * 
     * This class is an abstract class for all ECS components. It provides a method to get the component's ID.
     * It stores the component's name on ctor as a stringID.
     */
    template<typename Concrete>
    class AComponent : public IComponent {
        public:
            AComponent(std::string componentName) : __componentName(componentName) {};

            const std::string &getId() override { return __componentName; };


            const SparseArray<IComponent> &any_cast(const std::any &anySparseArray) override
            {
                const SparseArray<Concrete> *sparseArray = std::any_cast<SparseArray<Concrete>>(&anySparseArray);

                if (!sparseArray) {
                    throw BadSparseArrayCast();
                }

                return *reinterpret_cast<const SparseArray<IComponent> *>(sparseArray);
            }

            SparseArray<IComponent> &any_cast(std::any &anySparseArray) override
            {
                SparseArray<Concrete> *sparseArray = std::any_cast<SparseArray<Concrete>>(&anySparseArray);

                if (!sparseArray) {
                    throw BadSparseArrayCast();
                }

                return *reinterpret_cast<SparseArray<IComponent> *>(sparseArray);
            }

            /**
             * @class BadSparseArrayCast
             * 
             * @brief Exception thrown when a SparseArray cast fails.
             * 
             * This exception is thrown when a SparseArray cast fails.
             * It is a subclass of std::bad_any_cast.
             */
            class BadSparseArrayCast : public std::bad_any_cast {
                char const *what() const noexcept override {
                    return std::bad_any_cast::what();
                }
            };

        private:

            const std::string __componentName;
    };
}

#endif // ACOMPONENT_HPP
