{
  description = "game";
  inputs = {
    nixpkgs.url =
      "github:nixos/nixpkgs/c7ab75210cb8cb16ddd8f290755d9558edde7ee1";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        overlays = [ ];
        pkgs = import nixpkgs { inherit system overlays; };

        ctx = {
          package = {
            name = "game";
            version = "0.0.1";
            src = ./.;
          };
          build-deps = [ ];
        };
        package = pkgs.stdenv.mkDerivation {};
        devShell = pkgs.mkShell {
          packages = [
            pkgs.just
            pkgs.cmake
            pkgs.pkgsCross.mingwW64.buildPackages.gcc
            pkgs.clang-tools # includes 'clang-tidy' and 'clang-format'
          ] ++ ctx.build-deps;
        };
      in {
        formatter = pkgs.nixfmt-classic;
        devShell = devShell;
        packages.default = package;
        apps.default = {
          type = "app";
          program = "${package}/bin/${ctx.package.name}";
        };
      });
}
