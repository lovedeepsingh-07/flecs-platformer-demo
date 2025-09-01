#include "constants.hpp"
#include "systems.hpp"

void systems::camera(flecs::world& registry) {
    registry
        .system<components::Camera_Target, components::Position>("Handle Camera")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, const components::Camera_Target& camera_target,
                 const components::Position& pos) {
            flecs::world registry = curr_entity.world();
            if (!registry.has<components::ActiveScene, components::Game_Scene>()
                || !registry.has<components::GlobalCamera>()) {
                return;
            }

            auto& global_camera = registry.get_mut<components::GlobalCamera>();
            global_camera.camera.offset = (Vector2
            ){ (float)GetScreenWidth() / 2.0F, (float)GetScreenHeight() / 2.0F };

            Vector2 diff =
                Vector2Subtract(Vector2(pos.x, pos.y), global_camera.camera.target);
            float length = Vector2Length(diff);

            if (length > constants::MIN_CAMERA_EFFECT_LENGTH) {
                float speed =
                    fmaxf(constants::CAMERA_FRACTION_SPEED * length, constants::MIN_CAMERA_SPEED);
                global_camera.camera.target = Vector2Add(
                    global_camera.camera.target,
                    Vector2Scale(diff, speed * GetFrameTime() / length)
                );
            }
        })
        .add<components::system_types::Update>();
};
