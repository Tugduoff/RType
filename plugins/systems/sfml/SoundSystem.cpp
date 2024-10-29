/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SoundSystem.cpp file
*/

#include "SoundSystem.hpp"
#include "SfmlSoundComponent.hpp"
#include "components/sound/Sound.hpp"
// No need for this include for now, might be used later on...
// #include "components/position/Position.hpp"
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
    // For now, this is commented but could be used later on
    // auto &posArr = reg.componentManager().getComponents<Components::Position>();

    for (auto &&[i, sound] : IndexedZipper(sounds)) {
        if (sound.sound.empty()) {
            continue;
        }
        try {
            auto &soundComp = sfmlSoundComponents[i];
            if (soundComp->getSound().getStatus() == sf::Sound::Playing) {
                continue;
            }
            soundComp->play();
        } catch (std::exception &) {
            std::unique_ptr<Components::SfmlSoundComponent> soundComp = std::make_unique<Components::SfmlSoundComponent>();
            soundComp->setSound(sound.sound);
            soundComp->setVolume(sound.volume);
            soundComp->setPitch(sound.pitch);
            soundComp->setLoop(sound.loop);

            // try {
            //     // This could be used to create spatial sound later on using a sf::Listener attached to the player.
            //     // For now, it's not doing anything and wont make the program fail. Ive commented it out to avoid misleading comprehension.
            //     auto &posArray = posArr[i];
            //     soundComp->setPosition(posArray->x, posArray->y);
            // } catch (std::exception &) {
            // }

            reg.componentManager().addComponent<Components::SfmlSoundComponent>((ECS::Entity)i, std::move(soundComp));
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
