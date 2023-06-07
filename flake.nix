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
      packages.default = pkgs.stdenv.mkDerivation rec {
        pname = "Misstrhortion";
        version = "1.0.0";
        src = ./.;

        nativeBuildInputs = with pkgs; [
          cmake
          pkg-config
        ];

        buildInputs = with pkgs;
          lib.optionals stdenv.targetPlatform.isLinux [
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
          ];

        installPhase = ''
          mkdir -p \
            $out/bin \
            $out/lib/vst \
            $out/lib/clap \
            $out/lib/vst3 \
            $out/lib/ladspa \
            $out/lib/lv2

          cp bin/${pname} $out/lib/bin/
          cp bin/${pname}-vst.so $out/lib/vst/${pname}.so
          cp bin/${pname}.clap $out/lib/clap/
          cp -r bin/${pname}.vst3 $out/lib/vst3/
          cp bin/${pname}-ladspa.so $out/lib/ladspa/${pname}.so
          cp -r bin/${pname}.lv2 $out/lib/lv2/
        '';
      };

      devShells.default = pkgs.stdenv.mkDerivation {
        name = packages.default.pname;
        inherit (packages.default) buildInputs;
        nativeBuildInputs = packages.default.nativeBuildInputs ++ (with pkgs; [clang-tools]);
      };
    });
}
