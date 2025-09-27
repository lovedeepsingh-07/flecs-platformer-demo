#include "clay_raylib.hpp"
#include "components.hpp"
#include "interface.hpp"
#include "utils.hpp"

void Interface::main_menu_GUI(flecs::world& registry) {
    const auto& controller_engine = registry.get<components::Controller_Engine>();
    auto& per_frame_arena = registry.get_mut<components::PerFrame_MemoryArena>();

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
        CLAY({ .id = CLAY_ID("main_menu_ControllerContainer"),
               .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() },
                           .padding = CLAY_PADDING_ALL(8),
                           .childGap = 12,
                           .childAlignment = { .x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_BOTTOM },
                           .layoutDirection = CLAY_TOP_TO_BOTTOM },
               .floating = { .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                             .attachTo = CLAY_ATTACH_TO_ROOT } }) {

            CLAY({ .id = CLAY_ID("main_menu_ControllerSubContainer"),
                   .layout = { .sizing = { .width = CLAY_SIZING_FIT(), .height = CLAY_SIZING_FIT() },
                               .padding = CLAY_PADDING_ALL(8),
                               .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                               .layoutDirection = CLAY_TOP_TO_BOTTOM },
                   .backgroundColor = game_utils::RaylibColorToClayColor(GRAY) }) {
                Clay_String curr_controller_heading =
                    per_frame_arena.arena.get_string("keybinds");
                CLAY_TEXT(
                    curr_controller_heading,
                    CLAY_TEXT_CONFIG({
                        .textColor = game_utils::RaylibColorToClayColor(WHITE),
                        .fontId = 0,
                        .fontSize = 25,
                    })
                );
                for (const auto& curr_controller : controller_engine.engine.keymaps) {
                    Clay_String curr_controller_heading = per_frame_arena.arena.get_string(
                        std::string("------") + (curr_controller.first == 0 ? "player" : "enemy")
                        + std::string("------")
                    );
                    CLAY_TEXT(
                        curr_controller_heading,
                        CLAY_TEXT_CONFIG({
                            .textColor = game_utils::RaylibColorToClayColor(WHITE),
                            .fontId = 0,
                            .fontSize = 25,
                        })
                    );
                    CLAY_TEXT(
                        per_frame_arena.arena.get_string(
                            std::string("left - ")
                            + std::string(GetKeyName(+curr_controller.second.left))
                        ),
                        CLAY_TEXT_CONFIG({
                            .textColor = game_utils::RaylibColorToClayColor(WHITE),
                            .fontId = 0,
                            .fontSize = 25,
                        })
                    );
                    CLAY_TEXT(
                        per_frame_arena.arena.get_string(
                            std::string("right - ")
                            + std::string(GetKeyName(+curr_controller.second.right))
                        ),
                        CLAY_TEXT_CONFIG({
                            .textColor = game_utils::RaylibColorToClayColor(WHITE),
                            .fontId = 0,
                            .fontSize = 25,
                        })
                    );
                    CLAY_TEXT(
                        per_frame_arena.arena.get_string(
                            std::string("down - ")
                            + std::string(GetKeyName(+curr_controller.second.down))
                        ),
                        CLAY_TEXT_CONFIG({
                            .textColor = game_utils::RaylibColorToClayColor(WHITE),
                            .fontId = 0,
                            .fontSize = 25,
                        })
                    );
                    CLAY_TEXT(
                        per_frame_arena.arena.get_string(
                            std::string("jump - ")
                            + std::string(GetKeyName(+curr_controller.second.jump))
                        ),
                        CLAY_TEXT_CONFIG({
                            .textColor = game_utils::RaylibColorToClayColor(WHITE),
                            .fontId = 0,
                            .fontSize = 25,
                        })
                    );
                    CLAY_TEXT(
                        per_frame_arena.arena.get_string(
                            std::string("light attack - ")
                            + std::string(GetKeyName(+curr_controller.second.light_attack))
                        ),
                        CLAY_TEXT_CONFIG({
                            .textColor = game_utils::RaylibColorToClayColor(WHITE),
                            .fontId = 0,
                            .fontSize = 25,
                        })
                    );
                    CLAY_TEXT(
                        per_frame_arena.arena.get_string(
                            std::string("heavy attack - ")
                            + std::string(GetKeyName(+curr_controller.second.heavy_attack))
                        ),
                        CLAY_TEXT_CONFIG({
                            .textColor = game_utils::RaylibColorToClayColor(WHITE),
                            .fontId = 0,
                            .fontSize = 25,
                        })
                    );
                    CLAY_TEXT(
                        per_frame_arena.arena.get_string(
                            std::string("block - ")
                            + std::string(GetKeyName(+curr_controller.second.block))
                        ),
                        CLAY_TEXT_CONFIG({
                            .textColor = game_utils::RaylibColorToClayColor(WHITE),
                            .fontId = 0,
                            .fontSize = 25,
                        })
                    );
                }
            }
        }
    }
}
