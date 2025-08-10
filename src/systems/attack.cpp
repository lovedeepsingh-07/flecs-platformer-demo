#include "components.hpp"
#include "systems.hpp"
#include <box2d/box2d.h>

void AttackSystem::update(GameContext::GameContext& ctx, b2WorldId world_id) {
    ctx.registry
        .system<components::AttackComponent, components::AnimationComponent>()
        .each([](flecs::entity curr_entity, components::AttackComponent& attack,
                 components::AnimationComponent& animation) {
            if (curr_entity.has<components::AttackEventComponent>() && !attack.attacking) {
                attack.attacking = true;
            }
            // b2QueryFilter filter = b2DefaultQueryFilter();
            // filter.maskBits = Categories::BOXES;
            // ShapeUserData user_data = ShapeUserData{ .input = "someshit" };
            // -- this will stay here -- b2World_CastRay(world_id, tf.p,
            // (b2Vec2){ 50, 0 }, filter, cast_fcn, &user_data); DrawLine(tf.p.x,
            // tf.p.y, tf.p.x + 50, tf.p.y, RED); b2World_CastRay(
            //     b2WorldId worldId, b2Vec2 origin, b2Vec2 translation,
            //     b2QueryFilter filter, b2CastResultFcn * fcn, void* context
            // );
        })
        .run();
}
