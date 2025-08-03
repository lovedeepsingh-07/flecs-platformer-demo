#include "components.hpp"
#include "systems.hpp"
#include <raylib.h>

void Render2DSystem::update(GameContext::GameContext& ctx) {
    // render texture
    flecs::system texture_sys =
        ctx.registry
            .system<components::PositionComponent, components::SizeComponent, components::TextureComponent>()
            .each([](const components::PositionComponent& pos,
                     const components::SizeComponent& size,
                     const components::TextureComponent& texture) {
                float source_width = texture.flipped ? -texture.source_rect.width
                                                     : texture.source_rect.width;
                DrawTexturePro(
                    texture.texture,
                    (Rectangle){ texture.source_rect.x, texture.source_rect.y,
                                 source_width, texture.source_rect.height },
                    (Rectangle){ pos.x, pos.y, size.width, size.height },
                    (Vector2){ 0, 0 }, 0.0F, WHITE
                );
            });
    texture_sys.run();

    // render rectangles
    flecs::system rect_sys =
        ctx.registry
            .system<components::PositionComponent, components::SizeComponent, components::RectangleComponent>()
            .each([](const components::PositionComponent& pos,
                     const components::SizeComponent& size,
                     const components::RectangleComponent& rect) {
                DrawRectangle((int)pos.x, (int)pos.y, (int)size.width, (int)size.height, rect.color);
            });
    rect_sys.run();
}
