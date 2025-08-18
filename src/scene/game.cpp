#include "scene.hpp"

void scene::game(flecs::iter& iter, size_t, components::ActiveScene) {
    flecs::world registry = iter.world();

    // reset the scene
    registry.defer_begin();
    registry.delete_with(flecs::ChildOf, registry.entity<components::SceneRoot>());
    registry.defer_end();

    flecs::entity scene_root = registry.component<components::SceneRoot>();
}
