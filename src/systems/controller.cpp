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
            flecs::world registry = curr_entity.world();

            bool is_attacking = curr_entity.has<components::events::AttackEvent>();
            bool is_hit = curr_entity.has<components::events::HitEvent>();
            bool is_buffered_to_jump =
                curr_entity.has<components::events::BufferedJumpEvent>();

            if (controller._id == 0) {
                if (IsKeyDown(KEY_A) && !is_attacking) {
                    movement.left_idle_right = -1;
                } else if (IsKeyDown(KEY_D) && !is_attacking) {
                    movement.left_idle_right = 1;
                } else {
                    movement.left_idle_right = 0;
                }
                if (IsKeyPressed(KEY_W) && !is_buffered_to_jump && !is_hit) {
                    if (movement.on_ground) {
                        // if we are on the ground then we just add <JumpEvent,JumpEvent_One>
                        flecs::entity jump_entity =
                            registry.entity()
                                .add<components::events::JumpEvent, components::events::JumpEvent_One>()
                                .child_of(curr_entity);
                        curr_entity.add<components::Jump_Entity>(jump_entity);
                    } else {
                        flecs::entity jump_entity =
                            curr_entity.target<components::Jump_Entity>();
                        if (jump_entity.is_valid()) {
                            // if we have already jumped once, then we just add <JumpEvent,JumpEvent_Two>
                            if (jump_entity.has<components::events::JumpEvent, components::events::JumpEvent_One>(
                                )) {
                                jump_entity.add<components::events::JumpEvent, components::events::JumpEvent_Two>();
                            }
                            // if we have already jumped twice, then we just buffer the jump
                            if (jump_entity.has<components::events::JumpEvent, components::events::JumpEvent_Two>(
                                )) {
                                curr_entity.set<components::events::BufferedJumpEvent>(
                                    { 20 * constants::FRAMES_TO_SEC }
                                );
                            }
                        } else {
                            // if we are in the air and we have jumped yet, then we just add <JumpEvent,JumpEvent_Two>
                            flecs::entity jump_entity =
                                registry.entity()
                                    .add<components::events::JumpEvent, components::events::JumpEvent_Two>()
                                    .child_of(curr_entity);
                            curr_entity.add<components::Jump_Entity>(jump_entity);
                        }
                    }
                }
                if (IsKeyPressed(KEY_E) && !is_attacking && !is_hit) {
                    curr_entity.set<components::events::AttackEvent>({ .hit_some_entity = false });
                    // when we press the attack button, if a jump has been buffered, then that buffered jump will be cancelled
                    if (is_buffered_to_jump) {
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
                if (IsKeyPressed(KEY_K) && !is_buffered_to_jump && !is_hit) {
                    if (movement.on_ground) {
                        // if we are on the ground then we just add <JumpEvent,JumpEvent_One>
                        flecs::entity jump_entity =
                            registry.entity()
                                .add<components::events::JumpEvent, components::events::JumpEvent_One>()
                                .child_of(curr_entity);
                        curr_entity.add<components::Jump_Entity>(jump_entity);
                    } else {
                        flecs::entity jump_entity =
                            curr_entity.target<components::Jump_Entity>();
                        if (jump_entity.is_valid()) {
                            // if we have already jumped once, then we just add <JumpEvent,JumpEvent_Two>
                            if (jump_entity.has<components::events::JumpEvent, components::events::JumpEvent_One>(
                                )) {
                                jump_entity.add<components::events::JumpEvent, components::events::JumpEvent_Two>();
                            }
                            // if we have already jumped twice, then we just buffer the jump
                            if (jump_entity.has<components::events::JumpEvent, components::events::JumpEvent_Two>(
                                )) {
                                curr_entity.set<components::events::BufferedJumpEvent>(
                                    { 20 * constants::FRAMES_TO_SEC }
                                );
                            }
                        } else {
                            // if we are in the air and we have jumped yet, then we just add <JumpEvent,JumpEvent_Two>
                            flecs::entity jump_entity =
                                registry.entity()
                                    .add<components::events::JumpEvent, components::events::JumpEvent_Two>()
                                    .child_of(curr_entity);
                            curr_entity.add<components::Jump_Entity>(jump_entity);
                        }
                    }
                }
                if (IsKeyPressed(KEY_O) && !is_attacking && !is_hit) {
                    curr_entity.set<components::events::AttackEvent>({ .hit_some_entity = false });
                }
            }
        })
        .add<components::system_types::Update>();
};
