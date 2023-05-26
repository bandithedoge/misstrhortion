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
        name = "Misstortion";
        inherit (packages.default) buildInputs;
        nativeBuildInputs = packages.default.nativeBuildInputs ++ (with pkgs; [clang-tools]);
      };
    });
}
