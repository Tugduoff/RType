{
  description = ''
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
          sfml
        ];

        shellHook = ''
          echo Custom cpp nix-shell !
        '';
      };
    };
}
