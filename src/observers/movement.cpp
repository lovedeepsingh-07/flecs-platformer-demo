#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"

void observers::movement(flecs::world& registry) {
    registry.observer<components::events::JumpEvent>("JumpEvent on_add")
        .event(flecs::OnAdd)
        .second(flecs::Wildcard)
        .each([](flecs::entity curr_entity, const components::events::JumpEvent&) {
            flecs::entity owner_entity = curr_entity.parent();
            const auto& body = owner_entity.get<components::PhysicalBody>();
            const auto& pos = owner_entity.get<components::Position>();
            const auto& base_collider = owner_entity.get<components::BaseCollider>();
            b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
            vel.y = constants::PLAYER_JUMP_VEL;
            b2Body_SetLinearVelocity(body.body_id, vel);

            flecs::entity jumping_particle_emitter =
                owner_entity.target<components::emitter_types::JumpEmitter>();

            if (!jumping_particle_emitter.is_valid()) {
                return;
            }
            auto& particle_emitter =
                jumping_particle_emitter.get_mut<components::Particle_Emitter>();
            auto& emitter_pos = jumping_particle_emitter.get_mut<components::Position>();
            emitter_pos.x = pos.x;
            emitter_pos.y = pos.y + base_collider.height / 2;
            particle_emitter.engine.emitting = true;

            if (owner_entity.has<components::events::AttackEvent>()) {
                owner_entity.remove<components::events::AttackEvent>();
            }
        });
};
