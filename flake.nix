{
  description = "Flake for the SmartPlant Arduino project";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem [ "x86_64-linux" ] (system:
      let
        name = "SmartPlantV1";

        pkgs = import nixpkgs {
          inherit system;
        };

        arduinoShellHookPaths = ''
          if [ -z ''${XDG_CACHE_HOME:-} ]; then
              export _ARDUINO_ROOT=$HOME/.arduino/${name}
          else
              export _ARDUINO_ROOT=$XDG_CACHE_HOME/arduino/${name}
          fi
          export _ARDUINO_PYTHON3=${pkgs.python3}
          export ARDUINO_DIRECTORIES_USER=$_ARDUINO_ROOT
          export ARDUINO_DIRECTORIES_DATA=$_ARDUINO_ROOT
          export ARDUINO_DIRECTORIES_DOWNLOADS=$_ARDUINO_ROOT/staging
        '';

        devShellArduinoCLI =
          pkgs.mkShell ({
            name = "${name}-dev";
            packages = with pkgs; [
              arduino-cli
              git
              gnumake
              picocom
              python3
            ];
            shellHook = ''
              ${arduinoShellHookPaths}
              echo "==> Storing arduino-cli data in $_ARDUINO_ROOT"
            '';
          });

      in
        rec {
          devShells = {
            inherit
              devShellArduinoCLI
            ;
          };

          # Development shell spawned by `nix develop`
          devShells.default = devShellArduinoCLI;
        }
    );
}
