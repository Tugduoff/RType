{
  description = '':
    R-Type Flake
  '';

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.05";
  };

  outputs = { self , nixpkgs ,... }:
    let
      system = "x86_64-linux";
    in {
      devShells."${system}".default = let
        pkgs = import nixpkgs {
          inherit system;
        };
      in pkgs.mkShell {
        packages = with pkgs; [
          bashInteractiveFHS
          pkg-config

          valgrind
          cmake

          xorg.libXrandr
          xorg.libXcursor
          xorg.libX11
          xorg.libXxf86vm
          xorg.libXinerama
          xorg.xinput
          xorg.libXi
          xorg.libXfixes
          glfw
          libGLU
          libudev-zero
          openal
          flac
          libvorbis
          freetype
          criterion
          gcovr
          sfml
          libconfig
          doxygen
          doxygen_gui
        ];

        shellHook = ''
          export LD_LIBRARY_PATH=${pkgs.wayland}/lib:$LD_LIBRARY_PATH
          echo Custom cpp nix-shell !
        '';
      };
    };
}
