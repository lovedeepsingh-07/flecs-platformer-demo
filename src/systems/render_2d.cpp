#include "components.hpp"
#include "systems.hpp"
#include <raylib.h>

void Render2DSystem::render(GameContext::GameContext& ctx) {
    // render texture
    ctx.registry
        .system<components::PositionComponent, components::TextureComponent>()
        .each([](const components::PositionComponent& pos,
                 const components::TextureComponent& texture) {
            float source_width = texture.flipped ? -texture.source_rect.width
                                                 : texture.source_rect.width;
            DrawTexturePro(
                texture.texture,
                (Rectangle){ texture.source_rect.x, texture.source_rect.y,
                             source_width, texture.source_rect.height },
                (Rectangle){ pos.x - (texture.source_rect.width / 2),
                             pos.y - (texture.source_rect.height / 2),
                             texture.source_rect.width, texture.source_rect.height },
                (Vector2){ 0, 0 }, 0.0F, WHITE
            );
        })
        .run();

    // render rectangles
    ctx.registry
        .system<components::PositionComponent, components::RectangleComponent>()
        .each([](const components::PositionComponent& pos,
                 const components::RectangleComponent& rect) {
            DrawRectangle((int)pos.x, (int)pos.y, (int)rect.width, (int)rect.height, rect.color);
        })
        .run();
}

void Render2DSystem::render_raycasts(GameContext::GameContext& ctx) {
    // render raycasts
    ctx.registry
        .system<components::PositionComponent, components::TextureComponent>()
        .each([](flecs::entity curr_entity, const components::PositionComponent& pos,
                 const components::TextureComponent& texture) {
            if (curr_entity.has<components::PermanentRayCastComponent>()) {
                const auto& raycast =
                    curr_entity.get<components::PermanentRayCastComponent>();
                if (texture.flipped) {
                    DrawLine(
                        (int)(pos.x + raycast.start_offset.x),
                        (int)(pos.y + raycast.start_offset.y),
                        (int)(pos.x + raycast.start_offset.x
                              - raycast.translation.x),
                        (int)(pos.y + raycast.start_offset.y
                              - raycast.translation.y),
                        RED
                    );
                } else {
                    DrawLine(
                        (int)(pos.x + raycast.start_offset.x),
                        (int)(pos.y + raycast.start_offset.y),
                        (int)(pos.x + raycast.start_offset.x
                              + raycast.translation.x),
                        (int)(pos.y + raycast.start_offset.y
                              + raycast.translation.y),
                        RED
                    );
                }
            }
            if (curr_entity.has<components::PermanentRayCastListComponent>()) {
                const auto& raycast_list =
                    curr_entity.get<components::PermanentRayCastListComponent>();
                for (const auto& curr_raycast : raycast_list.items) {
                    if (texture.flipped) {
                        DrawLine(
                            (int)(pos.x + curr_raycast.start_offset.x),
                            (int)(pos.y + curr_raycast.start_offset.y),
                            (int)(pos.x + curr_raycast.start_offset.x
                                  - curr_raycast.translation.x),
                            (int)(pos.y + curr_raycast.start_offset.y
                                  - curr_raycast.translation.y),
                            RED
                        );
                    } else {
                        DrawLine(
                            (int)(pos.x + curr_raycast.start_offset.x),
                            (int)(pos.y + curr_raycast.start_offset.y),
                            (int)(pos.x + curr_raycast.start_offset.x
                                  + curr_raycast.translation.x),
                            (int)(pos.y + curr_raycast.start_offset.y
                                  + curr_raycast.translation.y),
                            RED
                        );
                    }
                }
            }
        })
        .run();
}
