#include "components.hpp"
#include "observers.hpp"
#include "scene.hpp"

void observers::scene_manager(flecs::world& registry) {
    registry.observer<components::ActiveScene>("MainMenu_Scene on_enter")
        .event(flecs::OnAdd)
        .second<components::scenes::MainMenu>()
        .each(scene::main_menu::on_enter);
    registry.observer<components::ActiveScene>("MainMenu_Scene on_exit")
        .event(flecs::OnRemove)
        .second<components::scenes::MainMenu>()
        .each(scene::main_menu::on_exit);

    registry.observer<components::ActiveScene>("Setting_Scene on_enter")
        .event(flecs::OnAdd)
        .second<components::scenes::Setting>()
        .each(scene::setting::on_enter);
    registry.observer<components::ActiveScene>("Setting_Scene on_exit")
        .event(flecs::OnRemove)
        .second<components::scenes::Setting>()
        .each(scene::setting::on_exit);

    registry.observer<components::ActiveScene>("Game_Scene on_enter")
        .event(flecs::OnAdd)
        .second<components::scenes::Game>()
        .each(scene::game::on_enter);
    registry.observer<components::ActiveScene>("Game_Scene on_exit")
        .event(flecs::OnRemove)
        .second<components::scenes::Game>()
        .each(scene::game::on_exit);
};
