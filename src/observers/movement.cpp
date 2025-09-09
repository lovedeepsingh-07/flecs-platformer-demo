#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"

void observers::movement(flecs::world& registry) {
    registry.observer<components::events::JumpEvent>("JumpEvent on_add")
        .event(flecs::OnAdd)
        .second(flecs::Wildcard)
        .each([](flecs::entity curr_entity, const components::events::JumpEvent&) {
            flecs::entity parent_entity = curr_entity.parent();
            const auto& body = parent_entity.get<components::PhysicalBody>();
            const auto& pos = parent_entity.get<components::Position>();
            const auto& base_collider = parent_entity.get<components::BaseCollider>();
            b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
            vel.y = constants::PLAYER_JUMP_VEL;
            b2Body_SetLinearVelocity(body.body_id, vel);

            flecs::entity jump_emitter_entity =
                parent_entity.target<components::emitter_types::Jump>();

            if (!jump_emitter_entity.is_valid()) {
                return;
            }
            auto& jump_emitter =
                jump_emitter_entity.get_mut<components::Particle_Emitter>();
            auto& emitter_pos = jump_emitter_entity.get_mut<components::Position>();
            emitter_pos.x = pos.x;
            emitter_pos.y = pos.y + base_collider.height / 2;
            jump_emitter.emitter.emitting = true;

            flecs::entity attack_entity =
                parent_entity.target<components::Attack_Entity>();
            if (attack_entity.is_valid()) {
                attack_entity.destruct();
            }
        });

    registry.observer<components::events::DashEvent>("DashEvent on_add")
        .event(flecs::OnAdd)
        .each([](flecs::entity curr_entity, const components::events::DashEvent&) {
            flecs::entity parent_entity = curr_entity.parent();

            const auto& body = parent_entity.get<components::PhysicalBody>();
            const auto& texture = parent_entity.get<components::TextureComponent>();
            const auto& pos = parent_entity.get<components::Position>();
            const auto& base_collider = parent_entity.get<components::BaseCollider>();

            b2Body_SetGravityScale(body.body_id, 0.0F);

            b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
            vel.x = constants::PLAYER_DASH_VEL * (float)(texture.flipped ? -1 : 1);
            vel.y = 0;
            b2Body_SetLinearVelocity(body.body_id, vel);

            flecs::entity dash_emitter_entity =
                parent_entity.target<components::emitter_types::Dash>();

            if (!dash_emitter_entity.is_valid()) {
                return;
            }
            auto& dash_emitter =
                dash_emitter_entity.get_mut<components::Particle_Emitter>();
            auto& emitter_pos = dash_emitter_entity.get_mut<components::Position>();
            emitter_pos.x = pos.x
                + (texture.flipped ? base_collider.width / 2 : -base_collider.width / 2);
            emitter_pos.y = pos.y;
            dash_emitter.emitter.config.direction_bias =
                (float)(texture.flipped ? std::numbers::pi : 0);
            dash_emitter.emitter.emitting = true;

            if (parent_entity.has<components::events::AttackEvent>()) {
                parent_entity.remove<components::events::AttackEvent>();
            }
        });

    registry.observer<components::events::DashEvent>("DashEvent on_remove")
        .event(flecs::OnRemove)
        .each([](flecs::entity curr_entity, const components::events::DashEvent&) {
            flecs::entity parent_entity = curr_entity.parent();
            const auto& body = parent_entity.get<components::PhysicalBody>();
            b2Body_SetGravityScale(body.body_id, 1.0F);
        });
};
