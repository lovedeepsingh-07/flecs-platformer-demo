#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"

// NOTE: position component represents the center point of any entity in my game
// but in raylib, the position is represented by the top-left corner for rendering
// so while rendering we have to convert my game's center point position to raylib's
// top-left point position by using the width and height of the entity
void systems::render::setup(flecs::world& registry) {
    registry.system("Prepare Frame").kind(flecs::PostUpdate).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();

        BeginDrawing();
        ClearBackground(constants::BACKGROUND_COLOR);

        // only for "Game_Scene" when registry has a "GlobalCamera"
        if (!registry.has<components::ActiveScene, components::Game_Scene>()
            || !registry.has<components::GlobalCamera>()) {
            return;
        }
        auto global_camera = registry.get<components::GlobalCamera>();
        BeginMode2D(global_camera.camera);
    });

    systems::render::texture(registry);

    registry.system<components::Particle_Emitter>("Render Particles")
        .kind(flecs::PreStore)
        .each([](flecs::entity curr_entity, components::Particle_Emitter& particle_emitter
              ) { particle_emitter.engine.render(); });

    systems::render::rectangle(registry);
    systems::render::physics(registry);

    registry
        .system<components::Health, components::BaseCollider, components::Position>("Render Healthbars")
        .kind(flecs::PreStore)
        .each([](const components::Health& health, const components::BaseCollider& base_collider,
                 const components::Position& pos) {
            int pos_x = (int)pos.x - (int)(constants::HEALTHBAR_WIDTH / 2);

            DrawRectangle(
                pos_x, (int)(pos.y - base_collider.height),
                (int)(constants::HEALTHBAR_WIDTH * (health.health / health.max_health)),
                constants::HEALTHBAR_HEIGHT, GREEN
            );
            DrawRectangleLines(
                pos_x, (int)(pos.y - base_collider.height),
                constants::HEALTHBAR_WIDTH, constants::HEALTHBAR_HEIGHT, WHITE
            );
        })
        .run();

    registry
        .system<components::State, components::Position, components::BaseCollider>("Render State")
        .kind(flecs::PreStore)
        .each([](flecs::entity curr_entity, components::State& state,
                 components::Position& pos, components::BaseCollider& base_collider) {
            flecs::world registry = curr_entity.world();
            if (!registry.has<components::ActiveScene, components::Game_Scene>()
                || !registry.has<components::global_options::DebugMode>()) {
                return;
            }
            DrawText(
                state.curr_state_id.c_str(), (int)(pos.x - (base_collider.width / 2.0F)),
                (int)(pos.y - (base_collider.height / 2) - 12), 12, WHITE
            ); // here we do - 12 because that is the font-size so font-height too, and we want the text to be anchored at bottom right of this pos
        })
        .run();

    systems::render::GUI(registry);

    registry.system("Finish Frame").kind(flecs::OnStore).run([](flecs::iter& iter) {
        EndDrawing();
    });
};
