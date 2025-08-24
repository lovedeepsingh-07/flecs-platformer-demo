#include "components.hpp"
#include "systems.hpp"

void systems::render::rectangle(flecs::world& registry) {
    registry
        .system<components::RectangleComponent, components::Position>(
            "Render Rectangles"
        )
        .kind(flecs::PreStore)
        .each([](flecs::entity curr_entity, const components::RectangleComponent& rect,
                 const components::Position& pos) {
            flecs::world registry = curr_entity.world();

            auto render_rect = Rectangle{
                pos.x - (rect.width / 2),
                pos.y - (rect.height / 2),
                rect.width,
                rect.height,
            };

            if (curr_entity.has<components::RectOpts_Lines>()
                && !curr_entity.has<components::RectOpts_Debug>()) {
                DrawRectangleLines(
                    (int)render_rect.x, (int)render_rect.y,
                    (int)render_rect.width, (int)render_rect.height, rect.color
                );
            }

            if (!curr_entity.has<components::RectOpts_Lines>()
                && curr_entity.has<components::RectOpts_Debug>()
                && registry.has<components::DebugMode>()) {
                DrawRectangle(
                    (int)render_rect.x, (int)render_rect.y,
                    (int)render_rect.width, (int)render_rect.height, rect.color
                );
            }

            if (curr_entity.has<components::RectOpts_Lines>()
                && curr_entity.has<components::RectOpts_Debug>()
                && registry.has<components::DebugMode>()) {
                DrawRectangleLines(
                    (int)render_rect.x, (int)render_rect.y,
                    (int)render_rect.width, (int)render_rect.height, rect.color
                );
            }

            if (!curr_entity.has<components::RectOpts_Lines>()
                && !curr_entity.has<components::RectOpts_Debug>()) {
                DrawRectangle(
                    (int)render_rect.x, (int)render_rect.y,
                    (int)render_rect.width, (int)render_rect.height, rect.color
                );
            }
        });
}
