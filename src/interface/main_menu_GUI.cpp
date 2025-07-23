#include "clay/clay.h"
#include "interface.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "scene_manager.hpp"
#include "utils.hpp"

void Interface::main_menu_GUI(GameContext& ctx, SceneManager::SceneManager* scene_manager) {
    CLAY({ .id = CLAY_ID("main_menu_MainContainer"),
           .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                       .padding = CLAY_PADDING_ALL(16),
                       .childGap = 16 } }) {
        CLAY_TEXT(
            CLAY_STRING("Main Menu"),
            CLAY_TEXT_CONFIG({
                .textColor = Utils::RaylibColorToClayColor(RAYWHITE),
                .fontId = 0,
                .fontSize = 25,
            })
        );
        CLAY({.id = CLAY_ID("main_menu_Button_1"),
            .layout =
                {
                    .sizing = {.width = {20}, .height = {20}},
                    .padding = CLAY_PADDING_ALL(2),
                },
            .backgroundColor = Clay_Hovered() ? Utils::RaylibColorToClayColor(RED) : Utils::RaylibColorToClayColor(WHITE)}) {
            if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                scene_manager->switch_to(ctx, SceneLabel::Game);
            }
            CLAY_TEXT(
                CLAY_STRING("Play"),
                CLAY_TEXT_CONFIG({
                    .textColor = Utils::RaylibColorToClayColor(BLACK),
                    .fontId = 0,
                    .fontSize = 25,
                })
            );
        };
        CLAY({.id = CLAY_ID("main_menu_Button_2"),
            .layout =
                {
                    .sizing = {.width = {20}, .height = {20}},
                    .padding = CLAY_PADDING_ALL(2),
                },
            .backgroundColor = Clay_Hovered() ? Utils::RaylibColorToClayColor(RED) : Utils::RaylibColorToClayColor(WHITE)}) {
            if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ctx.should_quit_game = true;
            }
            CLAY_TEXT(
                CLAY_STRING("Quit"),
                CLAY_TEXT_CONFIG({
                    .textColor = Utils::RaylibColorToClayColor(BLACK),
                    .fontId = 0,
                    .fontSize = 25,
                })
            );
        };
    }
}
