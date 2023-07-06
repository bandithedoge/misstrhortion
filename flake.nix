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
        pkgs = nixpkgs.legacyPackages.${system};
        drv = hostPkgs:
          hostPkgs.stdenv.mkDerivation {
            pname = "Misstrhortion";
            version = "1.0.0";
            src = ./.;

            nativeBuildInputs = with hostPkgs; [
              cmake
              pkg-config
              stdenv.cc.libc
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
              ++ (lib.optionals stdenv.hostPlatform.isMusl [libexecinfo])
              ++ (lib.optionals stdenv.hostPlatform.isCygwin [rhash]);

            passthru.cross = {
              linux = drv pkgs.pkgsCross.musl64;
              windows = drv pkgs.pkgsCross.mingw32;
            };

            # installPhase = ''
            #   mkdir -p \
            #     $out/bin \
            #     $out/lib/vst \
            #     $out/lib/clap \
            #     $out/lib/vst3 \
            #     $out/lib/ladspa \
            #     $out/lib/lv2
            #
            #   cp bin/${pname} $out/lib/bin/
            #   cp bin/${pname}-vst.so $out/lib/vst/${pname}.so
            #   cp bin/${pname}.clap $out/lib/clap/
            #   cp -r bin/${pname}.vst3 $out/lib/vst3/
            #   cp bin/${pname}-ladspa.so $out/lib/ladspa/${pname}.so
            #   cp -r bin/${pname}.lv2 $out/lib/lv2/
            # '';
          };
      in rec {
        packages.default = drv pkgs;
        devShells.default = pkgs.stdenv.mkDerivation {
          name = packages.default.pname;
          inherit (packages.default) buildInputs;
          nativeBuildInputs = packages.default.nativeBuildInputs ++ (with pkgs; [clang-tools]);
        };
      }
    );
}
