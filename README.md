# Misst*rh*ortion

![image](https://github.com/user-attachments/assets/13ea5d3f-8760-4bcd-b77c-e3862b92e19e)

**This project is NOT associated with or supported by [nimble.tools](https://github.com/nimbletools). Please do not bother them with issues related to this port.**

This is a WIP port of the old [Misstortion](https://github.com/nimbletools/misstortion1) to use [DPF](https://github.com/DISTRHO/DPF) instead of JUCE, allowing wider plugin format and platform support. Terrible name, I know.

## Differences from Misstortion

- more natively supported platforms (Windows, macOS, Linux) and plugin formats (CLAP, LV2, VST2, VST3, JACK/Standalone)
  - Misstortion 1 is only built for Windows and macOS as a VST3 plugin.
  - [pdesaulniers](https://github.com/pdesaulniers) maintains a [fork](https://github.com/pdesaulniers/misstortion1) that supports LV2 on Linux
  - AU and AAX are not and probably never will be supported. If you absolutely need to use these formats consider [Misstortion 2](https://nimble.itch.io/misstortion) or a wrapper such as [Blue Cat's PatchWork](https://www.bluecataudio.com/Products/Product_PatchWork/)
- all-new GUI

## Installation

While release builds are not provided yet, you can download the latest CI build [here](https://nightly.link/bandithedoge/misstrhortion/workflows/build/master).

Nix users can install the latest commit into their profile by running `nix profile install github:bandithedoge/misstrhortion?submodules=1`.

To install from source, simply follow the building instructions below.

# Building

Misstrhortion has at least these dependencies:

- CMake
- OpenGL
- X11 (if building for Linux)
- GLEW (if building for Windows)

Clone the repo and build it with CMake:

```sh
git clone https://github.com/bandithedoge/misstrhortion --recursive
cd misstrhortion
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Now you can install the plugin system-wide...
cmake --install build --prefix /usr
# ...or just for the current user
cmake --install build --prefix ~/.local
#
```

Nix users can use the provided flake to get a development shell with all dependencies:

```sh
nix develop .

nix build ".?submodules=1"

# yes, you need that extra parameter for Nix to properly fetch submodules.
# https://discourse.nixos.org/t/how-can-i-make-a-nix-flake-source-recuse-through-submodules/14345
```
