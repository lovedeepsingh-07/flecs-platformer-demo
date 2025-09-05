#include "systems.hpp"
#include "utils.hpp"

void systems::attack(flecs::world& registry) {
    registry.system<components::events::AttackEvent>("Handle Attacks")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity attack_entity, components::events::AttackEvent& attack_event) {
            flecs::world registry = attack_entity.world();
            const auto& physical_world = registry.get<components::PhysicalWorld>();
            const auto& state_engine = registry.get<components::State_Engine>();

            flecs::entity parent_entity = attack_entity.parent();
            const auto& state = parent_entity.get<components::State>();
            const auto& pos = parent_entity.get<components::Position>();
            const auto& texture = parent_entity.get<components::TextureComponent>();
            const auto& animation = parent_entity.get<components::Animation>();
            const auto& cast_query_filter =
                parent_entity.get<components::CastQueryFilter>();

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

            // calculate hitbox color and position based on current frame type and texture.flipped respectively
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

            flecs::entity hitbox_entity =
                attack_entity.target<components::Hitbox_Entity>();
            if (hitbox_entity.is_valid()) {
                // if hitbox is not valid, that means we have to create the hitbox entity
                auto& hitbox_rect_comp =
                    hitbox_entity.get_mut<components::RectangleComponent>();
                auto& hitbox_pos_comp = hitbox_entity.get_mut<components::Position>();

                // update hitbox color and position
                hitbox_rect_comp.color = hitbox_color;
                hitbox_pos_comp.x = hitbox_pos.x;
                hitbox_pos_comp.y = hitbox_pos.y;
            } else {
                hitbox_entity =
                    registry.entity()
                        .set<components::RectangleComponent>(
                            { hitbox_rect.width, hitbox_rect.height, hitbox_color }
                        )
                        .add<components::rectangle_options::RectOpts_Debug>()
                        .add<components::rectangle_options::RectOpts_Lines>()
                        .set<components::Position>({ hitbox_pos.x, hitbox_pos.y })
                        .child_of(attack_entity);
                attack_entity.add<components::Hitbox_Entity>(hitbox_entity);
            }

            // shape cast must only be performed when the frame is "active"
            if (curr_state.animation_data.frames[animation.curr_frame_index]._type == "active") {
                game_utils::CastContext cast_context{};
                b2Polygon hitbox_polygon =
                    b2MakeBox(hitbox_rect.width / 2, hitbox_rect.height / 2);
                b2ShapeProxy proxy = b2MakeOffsetProxy(
                    hitbox_polygon.vertices, hitbox_polygon.count,
                    hitbox_polygon.radius, b2Vec2{ hitbox_pos.x, hitbox_pos.y },
                    b2MakeRot(0)
                );
                b2World_CastShape(
                    physical_world.world_id, &proxy, (b2Vec2){ 0, 0 },
                    cast_query_filter.filter, game_utils::cast_result_fcn, &cast_context
                );

                if (cast_context.hit && !attack_event.hit_some_entity
                    && !cast_context.hit_entity.has<components::events::HitEvent>()
                    && !cast_context.hit_entity.has<components::events::HurtEvent>()) {
                    cast_context.hit_entity.set<components::events::HitEvent>(
                        { .direction = (texture.flipped ? -1 : 1) }
                    );
                    attack_event.hit_some_entity = true;
                }
            }
        })
        .add<components::system_types::Update>();
};
