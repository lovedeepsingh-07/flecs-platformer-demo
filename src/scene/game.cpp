#include "scene.hpp"

void scene::game(flecs::iter& iter, size_t, components::ActiveScene) {
    flecs::world registry = iter.world();

    // reset the scene
    registry.defer_begin();
    registry.delete_with(flecs::ChildOf, registry.entity<components::SceneRoot>());
    registry.defer_end();

    flecs::entity scene_root = registry.component<components::SceneRoot>();

    registry.entity("player")
        .set<components::Position>({ 200, 200 })
        .set<components::Rectangle>({ 32, 32, RAYWHITE })
        .set<components::Controller>({ 0 })
        .child_of(scene_root);

    registry.entity("enemy")
        .set<components::Position>({ 400, 400 })
        .set<components::Rectangle>({ 32, 32, RED })
        .set<components::Controller>({ 1 })
        .child_of(scene_root);
}
