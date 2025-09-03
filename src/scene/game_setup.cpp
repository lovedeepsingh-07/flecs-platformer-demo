#include "scene.hpp"

void scene::game_setup::on_enter(flecs::iter& iter, size_t, components::ActiveScene) {
    flecs::world registry = iter.world();

    // reset the scene
    registry.defer_begin();
    registry.delete_with(flecs::ChildOf, registry.lookup("scene_root"));
    registry.defer_end();

    flecs::entity scene_root = registry.lookup("scene_root");
}

void scene::game_setup::on_exit(flecs::iter& iter, size_t, components::ActiveScene) {}
