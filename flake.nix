{
  description = "game";
  inputs = {
    nixpkgs.url =
      "github:nixos/nixpkgs/f898cbfddfab52593da301a397a17d0af801bbc"; # NOTE: somehow this is the only commit that allows compiling to WASM
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        overlays = [ ];
        pkgs = import nixpkgs { inherit system overlays; };
      in {
        formatter = pkgs.nixfmt-classic;
        devShell = pkgs.mkShell {
          packages = [
            pkgs.just
            pkgs.cmake
            pkgs.pkgsCross.mingwW64.buildPackages.gcc
            pkgs.clang-tools
            pkgs.emscripten
            pkgs.nodejs
          ];
        };
      });
}
