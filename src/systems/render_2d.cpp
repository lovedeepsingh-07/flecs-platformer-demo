#include "systems/render_2d.hpp"
#include "components.hpp"
#include "raylib.h"

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

    // render rectangles lines
    // auto rect_lines_view{
    //     registry.view<Components::TransformComponent, Components::RectangleLinesComponent>()
    // };
    // rect_lines_view.each([](auto& pos, auto& rect_lines) {
    //     DrawRectangleLines(
    //         pos.x, pos.y, rect_lines.width, rect_lines.height, rect_lines.color
    //     );
    // });

    // render text
    // auto text_view{ registry.view<Components::TransformComponent, Components::TextComponent>() };
    // text_view.each([](auto& pos, auto& text) {
    //     DrawText(text.text.c_str(), pos.x, pos.y, text.font_size, text.color);
    // });
}
