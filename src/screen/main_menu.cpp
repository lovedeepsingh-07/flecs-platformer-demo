#include "screen.hpp"
#include "systems/render_2d.hpp"

void Screen::MainMenuScreen::on_enter(flecs::world& registry) {}
void Screen::MainMenuScreen::on_exit(flecs::world& registry) {
    registry.reset();
}
void Screen::MainMenuScreen::on_render(flecs::world& registry) {
    Render2DSystem::update(registry);
}
void Screen::MainMenuScreen::on_update(flecs::world& registry) {}
