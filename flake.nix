{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };
  outputs = inputs @ {
    self,
    flake-parts,
    ...
  }:
    flake-parts.lib.mkFlake {inherit inputs;} {
      systems = ["x86_64-linux" "x86_64-darwin" "aarch64-darwin"];
      imports = [inputs.flake-parts.flakeModules.easyOverlay];
      perSystem = {
        config,
        pkgs,
        self',
        ...
      }: {
        packages = {
          default = pkgs.stdenv.mkDerivation {
            pname = "Misstrhortion";
            version = "1.0.0";
            src = ./.;

            nativeBuildInputs = with pkgs; [
              cmake
              pkg-config
            ];

            buildInputs = with pkgs;
              lib.optionals stdenv.hostPlatform.isLinux [
                libGL
                xorg.libX11
              ];
          };
        };
        devShells.default = pkgs.stdenv.mkDerivation {
          name = self'.packages.default.pname;
          inherit (self'.packages.default) buildInputs;
          nativeBuildInputs = self'.packages.default.nativeBuildInputs ++ (with pkgs; [clang-tools]);
        };
      };
    };
}
