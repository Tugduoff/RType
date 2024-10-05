# R-Type (Multiplayer Variant)

## Project Description
This is a C++ school project where we recreate the classic **R-Type** game with a multiplayer twist. Players can connect to a server as clients, and the server can handle multiple client connections to enable multiplayer gameplay.

## Features
- **Multiplayer**: Players connect as clients to a central game server.
- **Server-Client Architecture**: Separate binaries for `r-type_server` and `r-type_client`.
- **Cross-Platform**: Compatible with Windows and Linux.

## Installation
To set up the project, follow these steps:

### 1. Download the Source Code
- **Windows**: Download the r-type-windows.zip and unzip it.
- **Linux**: Download the r-type-windows.tar and untar it.

### 2. Access the Binaries
Once extracted, you will have access to the following binaries:
- `r-type_client`
- `r-type_server`

## Running the Game

### 1. Start the Server
To start the multiplayer game, first, run the server:

```bash
./r-type_server
```

then the client:

```bash
./r-type_client
```
