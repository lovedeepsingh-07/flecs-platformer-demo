#include "systems.hpp"

void systems::render::texture(flecs::world& registry) {
    registry
        .system<components::TextureComponent, components::Position>("Render "
                                                                    "Textures")
        .kind(flecs::PreStore)
        .each([](const components::TextureComponent& texture, const components::Position& pos) {
            float source_width = texture.flipped ? -texture.source_rect.width
                                                 : texture.source_rect.width;
            float render_width =
                texture.source_rect.width * texture.render_scale.x;
            float render_height =
                texture.source_rect.height * texture.render_scale.y;
            DrawTexturePro(
                texture.texture,
                (Rectangle){ texture.source_rect.x, texture.source_rect.y,
                             source_width, texture.source_rect.height },
                (Rectangle){ pos.x - (render_width / 2), pos.y - (render_height / 2),
                             render_width, render_height },
                (Vector2){ 0, 0 }, 0.0F, WHITE
            );
        })
        .add<components::system_types::Render>();
}
