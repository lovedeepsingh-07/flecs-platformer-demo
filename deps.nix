{ pkgs, ... }:
let
  raylib = pkgs.fetchFromGitHub {
    owner = "raysan5";
    repo = "raylib";
    rev = "1c3f9fa135cafa494af760c4cd0541c9a23cdf60";
    sha256 = "sha256-bJzuF/5UFIHewQFyCDDPrkh6eyEUv4AYAzmkkV/WjjI=";
  };
  clay = pkgs.fetchFromGitHub {
    owner = "nicbarker";
    repo = "clay";
    rev = "b25a31c1a152915cd7dd6796e6592273e5a10aac";
    sha256 = "sha256-6h1aQXqwzPc4oPuid3RfV7W0WzQFUiddjW7OtkKM0P8=";
  };
  flecs = pkgs.fetchFromGitHub {
    owner = "SanderMertens";
    repo = "flecs";
    rev = "fc3b8999905e05b5ebb6562324c8b198d175804a";
    sha256 = "sha256-YC5SvxNP2gFq/tm9YLmCIdDQ1geA4co/l1tteQGPvhU=";
  };
  imgui = pkgs.fetchFromGitHub {
    owner = "ocornut";
    repo = "imgui";
    rev = "5d4126876bc10396d4c6511853ff10964414c776";
    sha256 = "sha256-m5h/H8k/CZ5Qwr3L9+Elt8XT/Tgwr3s4HuhVtI4wkwE=";
  };
  rlimgui = pkgs.fetchFromGitHub {
    owner = "raylib-extras";
    repo = "rlImGui";
    rev = "0105acf1bfba22e883688e5b9ffb1af0536739e2";
    sha256 = "sha256-7KtzWla0kKurEzfdlYcxep0v8ZpVNRc6ELFabCQBZbU=";
  };
  box2d = pkgs.fetchFromGitHub {
    owner = "erincatto";
    repo = "box2d";
    rev = "8c661469c9507d3ad6fbd2fea3f1aa71669c2fe3";
    sha256 = "sha256-IqQy9A8fWLG9H8ZPmOXeFZDaaks84miRuzXaFlNwm0g=";
  };
  pugixml = pkgs.fetchFromGitHub {
    owner = "zeux";
    repo = "pugixml";
    rev = "5a1892b321a38997e3e359069aea3fa35d49a5c6";
    sha256 = "sha256-/pqvxqpB292Nz1fZG6/CwScmv9fEMZT28braZ6FkNxg=";
  };
  rapidcsv = pkgs.fetchFromGitHub {
    owner = "d99kris";
    repo = "rapidcsv";
    rev = "d76cf1c08e79be1eb04c9d4d8eb7893221127991";
    sha256 = "sha256-vQYZUSb4bVbH8Bw9y3Fk0W5Utbl1mN4q86kkA+QOhNY=";
  };
  expected = pkgs.fetchFromGitHub {
    owner = "TartanLlama";
    repo = "expected";
    rev = "ea916f39627f02f192b5ec532210ab184e266e3e";
    sha256 = "sha256-puVjJJK0ImCfQBcwjkustbYKjvgQVg0by/sc2rNaRQE=";
  };
  lua = pkgs.fetchFromGitHub {
    owner = "lua";
    repo = "lua";
    rev = "9f791535cde1539338f6ba1b9154006595d97fe7";
    sha256 = "sha256-jcu37zun7ibKmK0uquzwYaoJh5miiWvDvAG/LOuexTI=";
  };
  sol2 = pkgs.fetchFromGitHub {
    owner = "ThePhD";
    repo = "sol2";
    rev = "9190880c593dfb018ccf5cc9729ab87739709862";
    sha256 = "sha256-bW6HD9WLOWizli6LnrkFZKxiT8IdN0QESlok+xCFz1w=";
  };
in
{
  setup_script = ''
    mkdir -p deps
    cp -r ${raylib} deps/raylib
    mkdir -p deps/clay
    cp -r ${clay} deps/clay/clay
    cp -r ${flecs} deps/flecs
    cp -r ${imgui} deps/imgui
    cp -r ${rlimgui} deps/rlimgui
    cp -r ${box2d} deps/box2d
    cp -r ${pugixml} deps/pugixml
    cp -r ${rapidcsv} deps/rapidcsv
    cp -r ${expected} deps/expected
    cp -r ${lua} deps/lua
    cp -r ${sol2} deps/sol2
  '';
}
