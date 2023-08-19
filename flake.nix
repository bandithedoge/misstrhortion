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
        ...
      }: rec {
        packages.default = pkgs.stdenv.mkDerivation {
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
            ]
            ++ (lib.optionals stdenv.hostPlatform.isMusl [libexecinfo]);
        };
        devShells.default = pkgs.stdenv.mkDerivation {
          name = packages.default.pname;
          inherit (packages.default) buildInputs;
          nativeBuildInputs = packages.default.nativeBuildInputs ++ (with pkgs; [clang-tools]);
        };
      };
    };
}
