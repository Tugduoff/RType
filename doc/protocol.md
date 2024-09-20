# R-Type communication protocol

This document describes the communication protocol between the R-Type server and the R-Type client.

The protocol is heavily [ECS](https://en.wikipedia.org/wiki/Entity_component_system)-oriented in the sense that all info sent by the server on the client are entity creation/deletion, components being attached to entities, and components' internal state being updated. It is true that on the other hand, client-to-server communication is not as much ECS-oriented as its counterpart, that is because the client doesn't decide anything about the ECS. It does, however, communicate player input to the server.

## General information

No matter the platform, communication is always made through UDP sockets, for faster data transfer than TCP.

The protocol is binary, meaning that -- contrary to text protocols -- the data sent is not human-readable, it some form of bytecode. This specific bytecode's styntax is detailed in this document.

Integers types are sent as in network byte order, i.e. big endian. See [`man 3 endian`](https://linux.die.net/man/3/endian) or [`man 3 byteorder`](https://linux.die.net/man/3/byteorder) if you have no idea what the previous sentence is about.

## Protocol specification

Below are described both the server-to-client and the client-to-server communication protocols

### Connection initialisation

When establishing connection between the client and the server, some initialisation process is required to make sure the client knows about all components used by the server, and this process also allows optimisation of the in-game protocol.

#### Description

When the client connects to the server, the server will send to the client the names of all components that may be used during the game. Before that, the server informs the client of the number of used components on 2 bytes and the maximum length of a component's name on 1 byte. Once all the names are transmitted, a special end indicator will be sent so that the client can make sure it received correct data.

> *Note*: The server may send a maximum length that is actually greater than the length of the longest component name (to align with a power of 2 for example)

After the number and size of names, the names of components are sent one after the other, zero-padded to fit the maximum length of a component's name, without any separator.

The names may only be composed of one or more of the following ascii characters:

- All letters `'a'` to `'z'` and `'A'` to `'Z'`
- All digits `'0'` through `'9'`
- The dash character `'-'`
- The underscore character `'_'`
- The dot character `'.'`

Meaning they all match the regular expression `[a-zA-Z0-9\-_.]+`

The order the names are sent in matters: their position (starting from 0) will be used as the `component_id` in later communications.

#### Example

For example, if the only components used are `"Position"`, `"Velocity"` and `"Health"`, the server could decide on 8 to be the maximum name length, and would then proceed to send the following to any incoming clients, in order:

- The bytes `0x0003`: the number of components used as a big endian 16-bits unsigned integer
- The byte `0x08`: the maximum length of a component name as an 8-bits unsigned integer
- The string `"Position"` (bytes `0x506f736974696f6e`)
- The string `"Velocity"` (bytes `0x56656c6f63697479`)
- The string `"Health"`, followed by 2 padding zero bytes to align with the maximum name length (bytes `0x4865616c74680000`)
- The bytes `0xffff`: the indicator sequence of the end of the initialisation process.

#### Design choices

This initialisation process's main purpose is to allow the client to make sure it knows about all the components that will be used by the server during the game.

This purpose is achievable in many different ways; this specific protocol aims at optimising the communication after the initialisation is done, and allowing the client to make sure it received everything properly.

For example, one way of sending component names would have been to send them concatenated with spaces or line breaks without specifying the count of name sent nor the length of the names. The main advantage of proceeding this way is that it is the most bandwidth-efficient way of communicating names (excluding methods which involve compression algorithms). On the other hand, it provides no ways for the client to check if any network errors occured. Plus, processing the names can be more CPU-expensive since they are not aligned in any but rather separated by a delimiter character.

In contrary, our R-Type protocol gives the client many ways to make sure they received everythin properly: since the number and size of the names are sent first, the client can then read `size * count` from the server, then read the end of initialisation sequence to make sure the correct number of bytes have been received. On top of that, the fact that the names are aligned allows the client to use a more performant algorithm to read names than if they were delimiter-separated.

### Server-to-client protocol

The server informs the client about all ECS updates, which includes:

- Entity creation
- Entity deletion
- Components getting attached to entities
- Components' internal state being updated
- Components getting detached from entities.

This way, the server only notifies of updates instead of dumping all info of all entities each frame.

#### General bytecode syntax

The server-to-client communication consists in a stream of instructions formed by the server that the client must interpret to update its copy of the ECS.

Those instructions are identified by an [opcode](https://en.wikipedia.org/wiki/Opcode), followed by instruction-specific operands.

The protocol is designed in a way that allows the client to know for sure the number of bytes of each instruction either directly from the opcode or from the opcode and following operands, depending on each specific instruction.

#### Instruction set

##### Instruction table

| Instruction       | Opcode | Operands                                | Instruction size |
|-------------------|--------|-----------------------------------------|------------------|
| Create entity     | 0x0    | entity_id                               | 3                |
| Delete entity     | 0x1    | entity_id                               | 3                |
|                   |        |                                         |                  |
| Attach component  | 0x2    | entity_id, component_id                 | 5                |
| Update component  | 0x3    | entity_id, component_id, component_data | 5 + data_size    |
| Detach component  | 0x4    | entity_id, component_id                 | 5                |

##### Operand sizes and format

| Operand        | Size               | Type               |
|----------------|--------------------|--------------------|
| entity_id      | 2                  | unsigned integer   |
| component_id   | 2                  | unsigned integer   |
| component_data | component-specific | component-specific |


##### Detail

###### 0x0 - Create entity

This instruction informs the client that an entity has been created, with no components attached. This entity will later be referred to by its `entity_id` by the server, the operand of this instruction.

###### 0x1 - Delete entity

This instruction informs the client that an entity is deleted from the ECS. The deleted entity can be identified by the instruction's operand, the `entity_id`.

###### 0x2 - Attach component

This instruction informs the client that a specific component has been attached to an entity, with its defaults parameters. The first operand is the id of the entity to which the component is attached. The second operand is the id of the component being attached.

###### 0x3 - Update component

This instruction informs the client about the update of a specific component attached to a specific entity. The entity is identified by the first operand, the component, by the second.

This instruction is specific in that its size depends on the component, which is one of its operands. The size of the third operand is specific to each component, which is part of why the initialisation process is required to make sure the client knows about all the used components.

Since the component serialisation is defined by each component, all the server and the client can do is check that the number of bytes processed is correct; it is the component's implementation's responsibility to transfer its state properly over the network.

###### 0x4 Detach component

This instruction informs the client about a specific component getting detached from an entity. The involved entity is identified by the first operand; the involved component is identified by the second operand.

### Client-to-server protocol

The client-to-server protocol is much less elaborate than its counterpart, since there is no ECS involved.

The client informs the server about all user input, in the form of key up / key down events. Similarly to the server-to-client protocol, the communication is done through an instruction-based stream of bytes, where each instruction has its specific operands.

> *Note:* The current instruction set doesn't use operands with any instruction; this is to come in later updates of the protocol (namely for click position)

#### Instruction set

| Instruction          | Opcode | Operands | Instruction size |
|----------------------|--------|----------|------------------|
| Right key pressed    | 0x0    | *none*   | 1                |
| Right key released   | 0x1    | *none*   | 1                |
| Up key pressed       | 0x2    | *none*   | 1                |
| Up key released      | 0x3    | *none*   | 1                |
| Left key pressed     | 0x4    | *none*   | 1                |
| Left key released    | 0x5    | *none*   | 1                |
| Down key pressed     | 0x6    | *none*   | 1                |
| Down key released    | 0x7    | *none*   | 1                |
| Left click pressed   | 0x8    | *none*   | 1                |
| Left click released  | 0x9    | *none*   | 1                |
| Right click pressed  | 0xa    | *none*   | 1                |
| Right click released | 0xb    | *none*   | 1                |

## Version

R-Typeuh network protocol version 0.0.1, 20/09/2024, written by Florent Charpentier
