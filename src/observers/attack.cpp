#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"
#include <iostream>

void observers::attack(flecs::world& registry) {
    registry.observer<components::events::AttackEvent>("AttackEvent on_remove")
        .event(flecs::OnRemove)
        .each([](flecs::entity attack_entity, const components::events::AttackEvent& attack_event) {
            flecs::entity hitbox_entity =
                attack_entity.target<components::Hitbox_Entity>();

            if (hitbox_entity.is_valid()) {
                hitbox_entity.destruct();
            }
        });
    registry
        .observer<components::events::HitEvent, components::PhysicalBody, components::TextureComponent, components::Position>(
            "HitEvent "
            "on_set"
        )
        .event(flecs::OnSet)
        .each([](flecs::entity curr_entity, const components::events::HitEvent& hit_event,
                 const components::PhysicalBody& body,
                 components::TextureComponent& texture, const components::Position& pos) {
            flecs::world registry = curr_entity.world();
            flecs::entity block_entity = curr_entity.target<components::Block_Entity>();

            if (block_entity.is_valid()) {
                if (curr_entity.has<components::events::ParryWindowEvent>()) {
                    // emit parry particles
                    flecs::entity parry_emitter_entity =
                        curr_entity.target<components::emitter_types::Parry>();
                    if (!parry_emitter_entity.is_valid()) {
                        return;
                    }
                    auto& parry_emitter =
                        parry_emitter_entity.get_mut<components::Particle_Emitter>();
                    auto& emitter_pos =
                        parry_emitter_entity.get_mut<components::Position>();
                    emitter_pos = pos;
                    parry_emitter.emitter.emitting = true;

                    // freeze for 15 frames
                    if (!registry.has<components::global_options::Freezed>()) {
                        registry.set<components::global_options::Freezed>(
                            { 15.0F * constants::FRAMES_TO_SEC }
                        );
                    }
                } else {
                    std::cout << "blocked" << '\n';
                }
            } else {
                // apply knockback
                b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
                vel.x = 2 * constants::MAX_PLAYER_RUN_VEL * (float)hit_event.direction;
                b2Body_SetLinearVelocity(body.body_id, vel);
                texture.flipped = (hit_event.direction != -1);

                // add HurtEvent
                curr_entity.set<components::events::HurtEvent>({ .damage =
                                                                     hit_event.damage });

                // emit hurt particles
                flecs::entity hurt_emitter_entity =
                    curr_entity.target<components::emitter_types::Hurt>();
                if (!hurt_emitter_entity.is_valid()) {
                    return;
                }
                auto& hurt_emitter =
                    hurt_emitter_entity.get_mut<components::Particle_Emitter>();
                auto& emitter_pos = hurt_emitter_entity.get_mut<components::Position>();
                emitter_pos = pos;
                hurt_emitter.emitter.emitting = true;
            }
            curr_entity.remove<components::events::HitEvent>();
        });
    registry
        .observer<components::events::HurtEvent, components::Health>("HurtEvent"
                                                                     " on_set")
        .event(flecs::OnSet)
        .each([](flecs::entity curr_entity, const components::events::HurtEvent& hurt_event,
                 components::Health& health) {
            // apply damage
            if (!(health.health <= 0)) {
                health.health -= (float)hurt_event.damage;
            } else {
                health.health = health.max_health;
            }

            flecs::entity attack_entity =
                curr_entity.target<components::Attack_Entity>();
            if (attack_entity.is_valid()) {
                attack_entity.destruct();
            }

            flecs::entity block_entity = curr_entity.target<components::Block_Entity>();
            if (block_entity.is_valid()) {
                block_entity.destruct();
            }
        });
};
