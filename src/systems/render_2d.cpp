#include "components.hpp"
#include "systems.hpp"
#include <raylib.h>

void Render2DSystem::update(GameContext::GameContext& ctx) {
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
