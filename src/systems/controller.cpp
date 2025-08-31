#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"

void systems::controller(flecs::world& registry) {
    registry
        .system<components::Controller, components::Movement>("Handle "
                                                              "Controller Input")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, const components::Controller& controller,
                 components::Movement& movement) {
            bool is_attacking = curr_entity.has<components::events::AttackEvent>();

            if (controller._id == 0) {
                if (IsKeyDown(KEY_A) && !is_attacking) {
                    movement.left_idle_right = -1;
                } else if (IsKeyDown(KEY_D) && !is_attacking) {
                    movement.left_idle_right = 1;
                } else {
                    movement.left_idle_right = 0;
                }
                if (IsKeyPressed(KEY_W)
                    && !curr_entity.has<components::events::BufferedJumpEvent>()
                    && !curr_entity.has<components::events::HitEvent>()) {
                    if (movement.on_ground) {
                        curr_entity.add<components::events::JumpEvent>();
                    } else {
                        curr_entity.set<components::events::BufferedJumpEvent>(
                            { 20 * constants::FRAMES_TO_SEC }
                        );
                    }
                }
                if (IsKeyPressed(KEY_E)
                    && !curr_entity.has<components::events::AttackEvent>()
                    && !curr_entity.has<components::events::HitEvent>()) {
                    curr_entity.set<components::events::AttackEvent>({ .hit_some_entity = false });
                    // when we press the attack button, if a jump has been buffered, then that buffered jump will be cancelled
                    if (curr_entity.has<components::events::BufferedJumpEvent>()) {
                        curr_entity.remove<components::events::BufferedJumpEvent>();
                    }
                }
            }
            if (controller._id == 1) {
                if (IsKeyDown(KEY_H) && !is_attacking) {
                    movement.left_idle_right = -1;
                } else if (IsKeyDown(KEY_L) && !is_attacking) {
                    movement.left_idle_right = 1;
                } else {
                    movement.left_idle_right = 0;
                }
                if (IsKeyPressed(KEY_K)
                    && !curr_entity.has<components::events::BufferedJumpEvent>()
                    && !curr_entity.has<components::events::HitEvent>()) {
                    if (movement.on_ground) {
                        curr_entity.add<components::events::JumpEvent>();
                    } else {
                        curr_entity.set<components::events::BufferedJumpEvent>(
                            { 20 * constants::FRAMES_TO_SEC }
                        );
                    }
                }
                if (IsKeyPressed(KEY_O)
                    && !curr_entity.has<components::events::AttackEvent>()
                    && !curr_entity.has<components::events::HitEvent>()) {
                    curr_entity.set<components::events::AttackEvent>({ .hit_some_entity = false });
                }
            }
        })
        .add<components::system_types::Update>();
};
