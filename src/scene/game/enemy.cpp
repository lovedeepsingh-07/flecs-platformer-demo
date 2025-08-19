#include "scene.hpp"

void scene::game::setup_enemy(flecs::world& registry, b2WorldId world_id) {
    flecs::entity scene_root = registry.component<components::SceneRoot>();
    auto& texture_engine = registry.get_mut<components::Texture_Engine>();

    // setup enemy entity
    registry.entity("enemy")
        .set<components::Position>({ 400, 400 })
        .set<components::TextureComponent>({
            .texture = texture_engine.engine.get_texture("player_walk"),
            .source_rect = { 0, 0, 128, 128 },
            .flipped = false,
        })
        .set<components::Controller>({ 1 })
        .child_of(scene_root);
}
