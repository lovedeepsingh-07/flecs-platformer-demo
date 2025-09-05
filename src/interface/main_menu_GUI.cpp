#include "components.hpp"
#include "interface.hpp"
#include "utils.hpp"

void Interface::main_menu_GUI(flecs::world& registry) {
    CLAY({ .id = CLAY_ID("main_menu_MainContainer"),
           .layout = {
               .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
               .padding = CLAY_PADDING_ALL(16),
               .childGap = 16,
               .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
               .layoutDirection = CLAY_TOP_TO_BOTTOM,
           } }) {
        CLAY_TEXT(
            CLAY_STRING("Main Menu"),
            CLAY_TEXT_CONFIG({
                .textColor = game_utils::RaylibColorToClayColor(WHITE),
                .fontId = 0,
                .fontSize = 25,
            })
        );
        CLAY({ .id = CLAY_ID("main_menu_play_button"),
               .layout = { .sizing = { .width = { 120 }, .height = { 20 } },
                           .padding = CLAY_PADDING_ALL(2),
                           .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER } },
               .backgroundColor = Clay_Hovered()
                   ? game_utils::RaylibColorToClayColor(GRAY)
                   : game_utils::RaylibColorToClayColor(WHITE) }) {
            bool hovering = Clay_Hovered();
            if (hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                registry.add<components::ActiveScene, components::scenes::Game>();
            }
            CLAY_TEXT(
                CLAY_STRING("Play"),
                CLAY_TEXT_CONFIG({
                    .textColor = hovering ? game_utils::RaylibColorToClayColor(WHITE)
                                          : game_utils::RaylibColorToClayColor(BLACK),
                    .fontId = 0,
                    .fontSize = 25,
                })
            );
        };
        CLAY({ .id = CLAY_ID("main_menu_setting_button"),
               .layout = { .sizing = { .width = { 120 }, .height = { 20 } },
                           .padding = CLAY_PADDING_ALL(2),
                           .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER } },
               .backgroundColor = Clay_Hovered()
                   ? game_utils::RaylibColorToClayColor(GRAY)
                   : game_utils::RaylibColorToClayColor(WHITE) }) {
            bool hovering = Clay_Hovered();
            if (hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                registry.add<components::ActiveScene, components::scenes::Setting>();
            }
            CLAY_TEXT(
                CLAY_STRING("Setting"),
                CLAY_TEXT_CONFIG({
                    .textColor = hovering ? game_utils::RaylibColorToClayColor(WHITE)
                                          : game_utils::RaylibColorToClayColor(BLACK),
                    .fontId = 0,
                    .fontSize = 25,
                })
            );
        };
        CLAY({ .id = CLAY_ID("main_menu_quit_button"),
               .layout = { .sizing = { .width = { 120 }, .height = { 20 } },
                           .padding = CLAY_PADDING_ALL(2),
                           .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER } },
               .backgroundColor = Clay_Hovered()
                   ? game_utils::RaylibColorToClayColor(GRAY)
                   : game_utils::RaylibColorToClayColor(WHITE) }) {
            bool hovering = Clay_Hovered();
            if (hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                registry.add<components::events::GameQuitEvent>();
            }
            CLAY_TEXT(
                CLAY_STRING("Quit"),
                CLAY_TEXT_CONFIG({
                    .textColor = hovering ? game_utils::RaylibColorToClayColor(WHITE)
                                          : game_utils::RaylibColorToClayColor(BLACK),
                    .fontId = 0,
                    .fontSize = 25,
                })
            );
        };
    }
}
