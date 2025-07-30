#include "components.hpp"
#include "imgui.h"
#include "interface.hpp"
#include "rlImGui.h"

void player_debug(GameContext& ctx) {
    flecs::system player_debug_display_sys =
        ctx.registry
            .system<
                components::PositionComponent, components::SizeComponent, components::PhysicsComponent,
                components::TextureComponent, components::AnimationComponent, components::AnimationStatesComponent,
                components::MovementComponent, components::ControllerComponent, components::CameraComponent>()
            .each([](const components::PositionComponent& pos,
                     const components::SizeComponent& size,
                     const components::PhysicsComponent& phy,
                     const components::TextureComponent& texture,
                     components::AnimationComponent& animation,
                     components::AnimationStatesComponent& animation_states,
                     components::MovementComponent& movement,
                     const components::ControllerComponent& controller,
                     const components::CameraComponent& cam) {
                // player debug information
                ImGui::Text("Position: (%.f, %.f)", pos.x, pos.y);
                ImGui::Text("OnGround: %s", movement.on_ground ? "true" : "false");
                ImGui::Text("Jumping: %s", movement.jumping ? "true" : "false");
                ImGui::Text("Falling: %s", movement.falling ? "true" : "false");
                ImGui::Text("Curr Frame Index: %d", animation.curr_frame_index);
                ImGui::Text("Curr Animation State: %s", animation.curr_state.c_str());
                ImGui::Text(
                    "Loop Animation: %s",
                    animation_states.clips[animation.curr_state].loop ? "true" : "false"
                );
                ImGui::Text("Animation Playing: %s", animation.playing ? "true" : "false");
                ImGui::Text("Animation Finished: %s", animation.finished ? "true" : "false");
                ImGui::Text("Animation Finished: %s", animation.finished ? "true" : "false");
                ImGui::Text("Texture Flipped: %s", texture.flipped ? "true" : "false");

                // player debug actions
                if (ImGui::Button("Jump Button")) {
                    movement.jump_requested = true;
                }
            });
    player_debug_display_sys.run();
}

void Interface::game_debug_GUI(GameContext& ctx) {
    rlImGuiBegin();
    bool open = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav
        | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove;

    const float PAD = 20.0F;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos;
    ImVec2 window_pos_pivot;
    window_pos.x = work_pos.x + work_size.x - PAD;
    window_pos.y = work_pos.y + PAD;
    window_pos_pivot.x = 1.0F;
    window_pos_pivot.y = 0.0F;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

    ImGui::SetNextWindowBgAlpha(0.35F);
    if (ImGui::Begin("Player Information", &open, window_flags)) {
        player_debug(ctx);
    }
    ImGui::End();
    rlImGuiEnd();
}
