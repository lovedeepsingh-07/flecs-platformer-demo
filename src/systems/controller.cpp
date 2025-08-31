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
            bool is_buffered_to_dash =
                curr_entity.has<components::events::BufferedDashEvent>();

            // TODO: this 'static' here is extremely unsafe as it persists across frames, and you should consider using the iter logic here
            static auto curr_key_pressed = (KeyboardKey)GetKeyPressed();

            if (controller._id == 0) {
                if (IsKeyDown(KEY_A) && !is_attacking) {
                    movement.left_idle_right = -1;
                } else if (IsKeyDown(KEY_D) && !is_attacking) {
                    movement.left_idle_right = 1;
                } else {
                    movement.left_idle_right = 0;
                }

                if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D))
                    && !curr_entity.target<components::Dash_Entity>().is_valid()) {
                    if (is_buffered_to_dash) {
                        if (curr_key_pressed
                            == curr_entity
                                   .get<components::events::BufferedDashEvent>()
                                   .buffer_key) {
                            // if we have already buffered the the dash, and it
                            // was in the same direction as the current keypress, then we add <DashEvent> and remove <BufferedDashEvent>
                            flecs::entity dash_entity =
                                registry.entity()
                                    .set<components::events::DashEvent>(
                                        { .dash_time = 20 * constants::FRAMES_TO_SEC }
                                    )
                                    .child_of(curr_entity);
                            curr_entity.add<components::Dash_Entity>(dash_entity);
                            curr_entity.remove<components::events::BufferedDashEvent>();
                        } else {
                            // if we have already buffered the the dash, and it
                            // was in the opposite direction as the current keypress,
                            // then we remove previous <BufferedDashEvent> and add a new <BufferedDashEvent> in this direction
                            curr_entity.remove<components::events::BufferedDashEvent>();
                            curr_entity.set<components::events::BufferedDashEvent>(
                                { .buffer_key = curr_key_pressed,
                                  .buffer_time = 20 * constants::FRAMES_TO_SEC }
                            );
                        }
                    } else {
                        // if we have not buffered the dash, then we just buffer it
                        curr_entity.set<components::events::BufferedDashEvent>(
                            { .buffer_key = curr_key_pressed,
                              .buffer_time = 20 * constants::FRAMES_TO_SEC }
                        );
                    }
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

                if ((IsKeyPressed(KEY_H) || IsKeyPressed(KEY_L))
                    && !curr_entity.target<components::Dash_Entity>().is_valid()) {
                    if (is_buffered_to_dash) {
                        if (curr_key_pressed
                            == curr_entity
                                   .get<components::events::BufferedDashEvent>()
                                   .buffer_key) {
                            // if we have already buffered the the dash, and it
                            // was in the same direction as the current keypress, then we add <DashEvent> and remove <BufferedDashEvent>
                            flecs::entity dash_entity =
                                registry.entity()
                                    .set<components::events::DashEvent>(
                                        { .dash_time = 20 * constants::FRAMES_TO_SEC }
                                    )
                                    .child_of(curr_entity);
                            curr_entity.add<components::Dash_Entity>(dash_entity);
                            curr_entity.remove<components::events::BufferedDashEvent>();
                        } else {
                            // if we have already buffered the the dash, and it
                            // was in the opposite direction as the current keypress,
                            // then we remove previous <BufferedDashEvent> and add a new <BufferedDashEvent> in this direction
                            curr_entity.remove<components::events::BufferedDashEvent>();
                            curr_entity.set<components::events::BufferedDashEvent>(
                                { .buffer_key = curr_key_pressed,
                                  .buffer_time = 20 * constants::FRAMES_TO_SEC }
                            );
                        }
                    } else {
                        // if we have not buffered the dash, then we just buffer it
                        curr_entity.set<components::events::BufferedDashEvent>(
                            { .buffer_key = curr_key_pressed,
                              .buffer_time = 20 * constants::FRAMES_TO_SEC }
                        );
                    }
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
