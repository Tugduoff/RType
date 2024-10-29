/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SoundSystem.cpp file
*/

#include "SoundSystem.hpp"
#include "SfmlSoundComponent.hpp"
#include "components/sound/Sound.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"

Systems::SoundSystem::SoundSystem()
{
}

Systems::SoundSystem::SoundSystem(libconfig::Setting &)
{
}

void Systems::SoundSystem::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();
    auto &sounds = reg.componentManager().getComponents<Components::Sound>();
    auto &sfmlSoundComponents = reg.componentManager().getComponents<Components::SfmlSoundComponent>();

    for (auto &&[i, sound] : IndexedZipper(sounds)) {
        for (auto &soundInstance : sound.sounds) {
            try {
                auto &sfmlSoundComp = sfmlSoundComponents[i];

                if (std::get<5>(soundInstance) == 1) {
                    sfmlSoundComp->play(std::get<0>(soundInstance));
                } else {
                    sfmlSoundComp->reset(std::get<0>(soundInstance));
                }
            } catch (std::exception &) {
                std::unique_ptr<Components::SfmlSoundComponent> soundComp =
                    std::make_unique<Components::SfmlSoundComponent>();
                for (auto &soundInstance2 : sound.sounds) {
                    soundComp->addSound(std::get<0>(soundInstance2), std::get<1>(soundInstance2),
                        std::get<2>(soundInstance2), std::get<3>(soundInstance2), std::get<4>(soundInstance2));
                }
                reg
                    .componentManager()
                    .addComponent<Components::SfmlSoundComponent>((ECS::Entity)i, std::move(soundComp));
            }
        }
    }
}

void Systems::SoundSystem::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Sound>("./plugins/bin/components/", "Sound");

    auto &manager = engine.getRegistry().componentManager();
    auto ctor = []() -> Components::SfmlSoundComponent * { return new Components::SfmlSoundComponent(); };
    manager.registerComponent<Components::SfmlSoundComponent>(ctor);
}
