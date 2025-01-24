# Game Engine documentation

Our game engine is an ECS (**E**ntity **C**omponent **S**ystem), so let's start by a quick explanation of what an ECS is.

## What is an ECS

In an ECS, there are three key elements, of course: entities, components, and systems.

### Entities

Entities by themselves are just some sort of unique identifier that can distinguish it from other entities. In our ECS, the unique identifier of an entity is simply the entity number from the start of the engine.

The interesting part about entities is that they can have [components](#components) attached to them

### Components

Components are the way arbitrary data can be stored in an ECS. In our ECS, they can be any C++ class, so you can store any kind of data you want in a component. Components don't exist by themselves, however, for a component instance to exit, it must be attached to an entity, that way it will be accessible when retrieving the components of said entity.

Components are mainly accessed by [systems](#systems), the last key element of an ECS.

### Systems

Systems are where you define code that will be run against entities and components, defining the logic of your game or program. In essence, a system is just a function which has access to entities and components and is able to mutate however you want to apply your logic on the ECS.

## Our game engine

In our game engine, each part the ECS is a plug-in, represented by a shared library in the filesystem. This facilitates the reuse of components and systems.

Components can be loaded individually by using the `Engine::registerComponent` method, which takes as parameter a path to a shared library. It will dynamically open the library using our Dynamic Libray Loader, making this component ready to use. In particular, this allows systems to create instances of components by calling any exposed function in the component shared library.


Systems get access by mutable reference to the entire `GameEngine` class in their `run` method, so they can really do anything on the ECS as a whole. This is by design, to give the most power to developers implementing systems for our engine.

This namely allowed us to create a `Config` system which reads a config file describing a list of systems, and loads these systems in the engine dynamically.
