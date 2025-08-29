#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"

void observers::movement(flecs::world& registry) {
    registry
        .observer<components::JumpEvent, components::PhysicalBody, components::Position, components::BaseCollider>(
            "JumpEvent "
            "on_add"
        )
        .event(flecs::OnAdd)
        .each([](flecs::entity curr_entity, const components::JumpEvent& jump_event,

                 const components::PhysicalBody& body, const components::Position& pos,
                 const components::BaseCollider& base_collider) {
            b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
            vel.y = constants::PLAYER_JUMP_VEL;
            b2Body_SetLinearVelocity(body.body_id, vel);

            flecs::entity jumping_particle_emitter =
                curr_entity.lookup("jumping_particle_emitter");

            if (!jumping_particle_emitter.is_valid()) {
                return;
            }
            auto& particle_emitter =
                jumping_particle_emitter.get_mut<components::Particle_Emitter>();
            auto& emitter_pos = jumping_particle_emitter.get_mut<components::Position>();
            emitter_pos.x = pos.x;
            emitter_pos.y = pos.y + base_collider.height / 2;
            particle_emitter.engine.emitting = true;
        });
};
