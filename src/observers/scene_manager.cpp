#include "components.hpp"
#include "observers.hpp"
#include "scene.hpp"

void observers::scene_manager(flecs::world& registry) {
    registry.observer<components::ActiveScene>("OnChange to Main Menu scene")
        .event(flecs::OnAdd)
        .second<components::MainMenu_Scene>()
        .each(scene::main_menu);
    registry.observer<components::ActiveScene>("OnChange to Game scene")
        .event(flecs::OnAdd)
        .second<components::Game_Scene>()
        .each(scene::game);
};
