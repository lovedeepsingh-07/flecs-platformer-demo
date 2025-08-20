#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"

void observers::movement(flecs::world& registry) {
    registry
        .observer<components::JumpEvent, components::PhysicalBody>("JumpEvent "
                                                                   "on_add")
        .event(flecs::OnAdd)
        .each([](const components::JumpEvent& jump_event, const components::PhysicalBody& body) {
            b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
            vel.y = constants::PLAYER_JUMP_VEL;
            b2Body_SetLinearVelocity(body.body_id, vel);
        });
};
