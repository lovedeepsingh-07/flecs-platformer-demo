#include "clay/clay.h"
#include "interface.hpp"
#include "raylib.h"
#include "screen.hpp"
#include "utils.hpp"

void Interface::game_GUI(ScreenManager::ScreenManager* screen_manager) {
    CLAY({ .id = CLAY_ID("game_MainContainer"),
           .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                       .padding = CLAY_PADDING_ALL(16),
                       .childGap = 16 } }) {
        CLAY_TEXT(
            CLAY_STRING("Game"),
            CLAY_TEXT_CONFIG({
                .textColor = Utils::RaylibColorToClayColor(RAYWHITE),
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
            .backgroundColor = Clay_Hovered() ? Utils::RaylibColorToClayColor(RED) : Utils::RaylibColorToClayColor(WHITE)}) {
            if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                screen_manager->switch_to(Screen::ScreenLabel::MainMenu);
            }
            CLAY_TEXT(
                CLAY_STRING("Exit"),
                CLAY_TEXT_CONFIG({
                    .textColor = Utils::RaylibColorToClayColor(BLACK),
                    .fontId = 0,
                    .fontSize = 25,
                })
            );
        };
    }
}
