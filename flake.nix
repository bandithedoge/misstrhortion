{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};
    in rec {
      packages.default = pkgs.stdenv.mkDerivation {
        pname = "Misstortion";
        version = "1.3";
        src = ./.;

        nativeBuildInputs = with pkgs;
          [
            cmake
            pkg-config
          ]
          ++ lib.optionals stdenv.targetPlatform.isLinux [
            alsa-lib
            freetype
            xorg.libX11
            xorg.libXcursor
            xorg.libXext
            xorg.libXinerama
            xorg.libXrandr
          ];

        enableParallelBuilding = true;
      };

      devShells.default = pkgs.mkShell {
        inherit (packages.default) buildInputs;
        nativeBuildInputs = packages.default.nativeBuildInputs ++ (with pkgs; [clang-tools]);
      };
    });
}
