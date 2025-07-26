#include "components.hpp"
#include "raylib.h"
#include "systems.hpp"

void Render2DSystem::update(GameContext& ctx) {
    // render texture
    flecs::system texture_sys =
        ctx.registry
            .system<components::PositionComponent, components::SizeComponent, components::TextureComponent>()
            .each([](const components::PositionComponent& pos,
                     const components::SizeComponent& size,
                     const components::TextureComponent& texture) {
                DrawTexturePro(
                    texture.texture,
                    (Rectangle){ texture.x, texture.y, texture.width, texture.height },
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
