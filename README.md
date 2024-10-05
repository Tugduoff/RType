# R-Type (Multiplayer Variant)

## Project Description
This is a C++ school project in which we recreate the classic [**R-Type**](https://rtype.fandom.com/wiki/R-Type) game with a multiplayer twist. This project is available on Windows and Linux.

You can now play together on different computers with your friends to the best game ever ! **Go destroy the Bydo together !**

## Installation
To play our game, follow these steps:

### 1. Download the archive
On the [release page](https://github.com/Tugduoff/RType/releases), download the files corresponding to your operating system :
- **Windows** : `r-type_windows_x64.zip`
- **Linux** : 
  - **Debian** distributions : `r-type_debian-based_x64.tar`
  - **RedHat** distributions : `r-type_redhat-based_x64.tar`

### 2. Extract the Binaries
Once you have downloaded the archive, you will need to extract the binaries from it :

- **Windows** :
You can either do it via :
  -  Windows Graphical User Interface (GUI) :
  In the **File Explorer** navigate to the folder where the `r-type_windows_x64.zip` is. Right click on it and select the  option `Extract all`.
  -  Powershell : *(We will consider that the archive is in the Download folder of the user)*
  ```powershell
  cd ~\Downloads
  Expand-Archive -Path ".\r-type_windows_x64.zip" -DestinationPath ".\rtype"
  ```
- **Linux** : 
  *(Once again, we will consider that the archive is in the Download folder of the user)*
  ```bash
  cd ~/Downloads
  mkdir ./rtype
  tar -xf "./r-type_debian-based_x64.tar" --directory "./rtype"
  ```
  (replace `debian-based` by `redhat-based` if you have the redhat archive)

### 3. Check Binaries

You should now have a `rtype` directory, in this folder you will find on : 
- **Windows** : `r-type_server.exe`, `r-type_client.exe` and a bunch of `.dll` and `.pdb` files

- **Linux** : `r-type_server.sh`, `r-type_client.sh` and a `lib` folder with a bunch of `.so` files


## Running the Game

### 1. Start the Server
To start the game, first go into the `rtype` directory, then, run the server:
- **Windows** : 
```powershell
.\r-type_server.exe -p 4242
```
- **Linux** : 
```bash
./r-type_server.sh -p 4242
```

### 2. Start the Client
- **Windows** : 
```powershell
.\r-type_client.exe -h 127.0.0.0 -p 4242
```
- **Linux** : 
```bash
./r-type_server.sh -h 127.0.0.0 -p 4242
```

Here you go, you can now enjoy playing our game !
