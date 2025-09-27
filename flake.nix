{
  description = "flecs-platformer-demo";
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
        deps = import ./deps.nix { inherit pkgs; };
        build-deps =
          [ pkgs.emscripten pkgs.nodejs pkgs.cmake pkgs.python3 ];
      in
      {
        formatter = pkgs.nixpkgs-fmt;
        devShell = pkgs.mkShell {
          packages = [
            pkgs.just
            pkgs.pkgsCross.mingwW64.buildPackages.gcc
            pkgs.clang-tools
          ] ++ build-deps;
        };
        packages.default = pkgs.stdenv.mkDerivation {
          name = "flecs-platformer-demo";
          src = ./.;
          nativeBuildInputs = build-deps;
          configurePhase = "true"; # this is to prevent cmake from running automatically
          buildPhase = ''
            		    export EM_CACHE="$(pwd)/.emcache"
            			mkdir -p $EM_CACHE
            		  	${deps.setup_script}
            			mkdir -p $out/bin
            			mkdir -p build/web
            			emcmake cmake -S . -B build/web -DCMAKE_BUILD_TYPE=Release -DEXPECTED_BUILD_TESTS=OFF
            			cmake --build ./build/web
            		  '';
          installPhase = ''
                        			  mkdir -p $out/bin
                        		      cp build/web/flecs-platformer-demo.js $out/bin
                        		      cp build/web/flecs-platformer-demo.wasm $out/bin
                        		      cp build/web/flecs-platformer-demo.data $out/bin
            						  cp public/index.html $out/bin
                        		  '';
        };
        apps.setup = flake-utils.lib.mkApp {
          drv = pkgs.writeShellApplication {
            name = "setup";
            runtimeInputs = [ ];
            text = deps.setup_script;
          };
        };
      });
}
