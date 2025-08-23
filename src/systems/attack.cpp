#include "components.hpp"
#include "raymath.h"
#include "systems.hpp"

void systems::attack(flecs::world& registry) {
    registry
        .system<components::AttackEvent, components::State, components::Animation, components::Position, components::TextureComponent>(
            "Handle Attacks"
        )
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::AttackEvent& attack,
                 const components::State& state, const components::Animation& animation,
                 const components::Position& pos, const components::TextureComponent& texture) {
            flecs::world registry = curr_entity.world();
            const auto& state_engine = registry.get<components::State_Engine>();

            // get current state registry
            auto state_registry_result =
                state_engine.engine.get_state_registry(state.state_registry_id);
            if (!state_registry_result) {
                throw std::runtime_error(state_registry_result.error().message);
            }
            const StateEngine::StateRegistry& curr_registry =
                state_registry_result->get();

            // get current state
            auto state_result = curr_registry.get_state(state.curr_state_id);
            if (!state_result) {
                throw std::runtime_error(state_result.error().message);
            }
            const StateEngine::State& curr_state = state_result->get();

            // we only want to run this system's code for states which are "offensive"
            if (!curr_state.offensive) {
                return;
            }

            flecs::entity hitbox_entity = curr_entity.lookup("hitbox");

            Rectangle hitbox_rect = curr_state.hitbox;
            Color hitbox_color =
                ((curr_state.animation_data.frames[animation.curr_frame_index]._type == "active")
                     ? RED
                     : WHITE);
            Vector2 hitbox_pos = Vector2Add(
                Vector2{ pos.x, pos.y },
                Vector2{ (float)(texture.flipped ? -1 : 1) * hitbox_rect.x,
                         hitbox_rect.y }
            );

            if (!hitbox_entity.is_valid()) {
                registry.entity("hitbox")
                    .set<components::RectangleComponent>({ hitbox_rect.width,
                                                           hitbox_rect.height, hitbox_color })
                    .add<components::RectOpts_Debug>()
                    .add<components::RectOpts_Lines>()
                    .set<components::Position>({ hitbox_pos.x, hitbox_pos.y })
                    .child_of(curr_entity);
            } else {
                auto& hitbox_rect_comp =
                    hitbox_entity.get_mut<components::RectangleComponent>();
                auto& hitbox_pos_comp = hitbox_entity.get_mut<components::Position>();

                // update hitbox color and position
                hitbox_rect_comp.color = hitbox_color;
                hitbox_pos_comp.x = hitbox_pos.x;
                hitbox_pos_comp.y = hitbox_pos.y;
            }

            // TODO: shape cast here
            if (curr_state.animation_data.frames[animation.curr_frame_index]._type
                == "active") {}
        });
};
