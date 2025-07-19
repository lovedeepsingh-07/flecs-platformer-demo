#include "components.hpp"
#include "raylib.h"
#include "systems.hpp"

void Render2DSystem::update(flecs::world& registry) {
    // render rectangles
    flecs::system rect_sys =
        registry
            .system<components::PositionComponent, components::SizeComponent, components::RectangleComponent>()
            .each([](const components::PositionComponent& pos,
                     const components::SizeComponent& size,
                     const components::RectangleComponent& rect) {
                DrawRectangle((int)pos.x, (int)pos.y, (int)size.width, (int)size.height, rect.color);
            });
    rect_sys.run();
}
