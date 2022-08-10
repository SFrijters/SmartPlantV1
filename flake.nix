{
  description = "Flake for the SmartPlant Arduino project";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem [ "x86_64-linux" ] (system:
      let
        name = "arduino-SmartPlant-dev";

        vendor = "arduino";
        arch = "avr";
        boardId = "nano";
        fqbn = "${vendor}:${arch}:${boardId}";

        pkgs = import nixpkgs {
          inherit system;
        };

        arduinoShellHookPaths = ''
          export _ARDUINO_ROOT=$HOME/.${name}
          export ARDUINO_DIRECTORIES_USER=$_ARDUINO_ROOT
          export ARDUINO_DIRECTORIES_DATA=$_ARDUINO_ROOT
          export ARDUINO_DIRECTORIES_DOWNLOADS=$_ARDUINO_ROOT/staging
          export _ARDUINO_VENDOR=${vendor}
          export _ARDUINO_ARCH=${arch}
          export _ARDUINO_BOARDID=${boardId}
          export _ARDUINO_FQBN=${fqbn}
          export _ARDUINO_PROJECT_NAME=${name}
        '';

        devShellArduinoCLI =
          pkgs.mkShell ({
            inherit name;
            packages = [ pkgs.arduino-cli pkgs.gnumake ];
            shellHook = ''
              ${arduinoShellHookPaths}
              echo "==> Storing arduino-cli data in $_ARDUINO_ROOT"
              arduino-cli core install ${vendor}:${arch}
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
