#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"
#include <box2d/box2d.h>

void KnockbackSystem::setup(GameContext::GameContext& ctx) {
    ctx.registry
        .observer<components::AttackHitEventComponent, components::PhysicsComponent>()
        .event(flecs::OnSet)
        .each([](flecs::entity curr_entity, const components::AttackHitEventComponent& attack_hit_event,
                 const components::PhysicsComponent& phy) {
            b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);
            vel.x = 2*constants::MAX_PLAYER_RUN_VEL * (float)attack_hit_event.direction;
            b2Body_SetLinearVelocity(phy.body_id, vel);
        });
}
