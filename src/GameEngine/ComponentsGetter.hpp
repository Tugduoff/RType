/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ComponentsGetter.hpp file
*/

#ifndef COMPONENTS_GETTER_HPP
    #define COMPONENTS_GETTER_HPP

    #include "ECS/component/ComponentManager.hpp"
    #include "ECS/utilities/SparseArray.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include "components/IComponent.hpp"
    #include <memory>
    #include <typeindex>
    #include <unordered_map>
    #include <utility>

class ComponentsGetter {
public:
    ComponentsGetter(const auto &compManager, const auto &components)
    : _compManager(compManager), _components(components)
    {
    }

    ComponentsGetter(Engine::GameEngine &engine)
    :   _compManager(engine.getRegistry().componentManager()),
        _components(engine.getComponents())
    {
    }

    friend struct ComponentsGetterIterator;

protected:
    using underlying_map = std::unordered_map<std::type_index, std::unique_ptr<Components::IComponent>>;

public:

    using key_type = std::type_index;
    using mapped_type = SparseArray<Components::IComponent> &;

    using value_type = std::pair<const key_type, mapped_type>;

    using size_type = underlying_map::size_type;
    using difference_type = underlying_map::difference_type;

    using reference = value_type &;
    using const_reference = const value_type &;

protected:

    /**
     * @brief Reference-captured component manager
     */
    ECS::ComponentManager &_compManager;

    /**
     * @brief Reference-captured GameEngine::__components
     */
    const underlying_map &_components;

    value_type _mapValue(const underlying_map::value_type &pair) const
    {
        const auto &[type_idx, comp] = pair;
        return {
                type_idx,
                comp->any_cast(
                   _compManager.getComponents(type_idx)
                )
            };
    }

    struct ComponentsGetterIterator : public ComponentsGetter::underlying_map::const_iterator {
    protected:
        using iterator = ComponentsGetter::underlying_map::const_iterator;
        using value_type = ComponentsGetter::value_type;

        friend class ComponentsGetter;
        ComponentsGetterIterator(iterator src, const ComponentsGetter &compGetter) : iterator(std::move(src)), _compGetter(compGetter) {}

    public:
        value_type operator*() const
        {
            return _compGetter._mapValue(iterator::operator*());
        }

    protected:
        const ComponentsGetter &_compGetter;
    };

public:    

    using const_iterator = ComponentsGetterIterator;

    const_iterator begin() const { return const_iterator(_components.begin(), *this); }
    const_iterator cbegin() const { return begin(); }

    const_iterator end() const { return const_iterator(_components.end(), *this); }
    const_iterator cend() const { return const_iterator(_components.end(), *this); }

    mapped_type at(const std::type_index &type) const
    {
        return _components.at(type)->any_cast(_compManager.getComponents(type));
    }

    mapped_type operator[](const std::type_index &type) const
    {
        return this->at(type);
    }

    auto count(const key_type &key) const noexcept
    {
        return _components.count(key);
    }

    auto contains(const key_type &key) const noexcept
    {
        return _components.contains(key);
    }

    auto find(const key_type &key) const noexcept
    {
        return _mapValue(*_components.find(key));
    }

};

#endif // COMPONENTS_GETTER_HPP
