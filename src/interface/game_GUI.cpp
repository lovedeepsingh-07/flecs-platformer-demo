#include "components.hpp"
#include "interface.hpp"
#include "utils.hpp"

void Interface::game_GUI(flecs::world& registry) {
    CLAY({ .id = CLAY_ID("game_MainContainer"),
           .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                       .padding = CLAY_PADDING_ALL(16),
                       .childGap = 16 } }) {
        CLAY_TEXT(
            CLAY_STRING("Game"),
            CLAY_TEXT_CONFIG({
                .textColor = game_utils::RaylibColorToClayColor(RAYWHITE),
                .fontId = 0,
                .fontSize = 25,
            })
        );
        CLAY({.id = CLAY_ID("game_Button"),
            .layout =
                {
                    .sizing = {.width = {20}, .height = {20}},
                    .padding = CLAY_PADDING_ALL(2),
                },
            .backgroundColor = Clay_Hovered() ? game_utils::RaylibColorToClayColor(RED) : game_utils::RaylibColorToClayColor(WHITE)}) {
            if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                registry.add<components::ActiveScene, components::MainMenu_Scene>();
            }
            CLAY_TEXT(
                CLAY_STRING("Exit"),
                CLAY_TEXT_CONFIG({
                    .textColor = game_utils::RaylibColorToClayColor(BLACK),
                    .fontId = 0,
                    .fontSize = 25,
                })
            );
        };
    }
}
