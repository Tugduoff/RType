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


