/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Visible.cpp file
*/

#include <memory>
#include "Visible.hpp"

namespace Components {

    /**
     * @brief Factory function to create an instance of Visible with an initial state.
     * 
     * @param isVisible Initial visibility state for the component.
     * @return A unique pointer to the created Visible component.
     */
    extern "C" std::unique_ptr<IComponent> entryPoint(bool isVisible)
    {
        return std::make_unique<Visible>(isVisible);
    }

    /**
     * @brief Factory function to create an instance of Visible using configuration settings.
     * 
     * @param config A libconfig::Setting object containing the visibility state.
     * @return A unique pointer to the created Visible component.
     */
    extern "C" std::unique_ptr<IComponent> entryConfig(libconfig::Setting &config)
    {
        bool visibleState;
        if (!config.lookupValue("isVisible", visibleState))
            throw std::runtime_error("Missing 'isVisible' setting for Visible component");

        return std::make_unique<Visible>(visibleState);
    }

}
