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
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = import nixpkgs {inherit system;};
        drv = hostPkgs:
          hostPkgs.stdenv.mkDerivation {
            pname = "Misstrhortion";
            version = "1.0.0";
            src = ./.;

            nativeBuildInputs = with hostPkgs; [
              cmake
              pkg-config
            ];

            buildInputs = with hostPkgs;
              lib.optionals stdenv.hostPlatform.isLinux [
                alsa-lib
                dbus
                libGL
                xorg.libX11
                xorg.libXScrnSaver
                xorg.libXcursor
                xorg.libXext
                xorg.libXfixes
                xorg.libXi
                xorg.libXrandr
              ]
              ++ (lib.optionals stdenv.hostPlatform.isMusl [libexecinfo]);
          };
      in rec {
        packages = rec {
          inherit (misstrhortion) default;
          misstrhortion = {
            default = drv pkgs;
            linux64 = drv pkgs.pkgsCross.musl64;
            # linux64 = drv (import nixpkgs {
            #   inherit system;
            #   crossSystem = nixpkgs.lib.systems.examples.musl64;
            # });
          };
        };
        devShells.default = pkgs.stdenv.mkDerivation {
          name = packages.default.pname;
          inherit (packages.default) buildInputs;
          nativeBuildInputs = packages.default.nativeBuildInputs ++ (with pkgs; [clang-tools]);
        };
      }
    );
}
