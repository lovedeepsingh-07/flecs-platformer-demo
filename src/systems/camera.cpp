#include "systems/camera.hpp"
#include "components.hpp"
#include "constants.hpp"
#include "raylib.h"
#include "raymath.h"

void CameraSystem::update(flecs::world& registry, Camera2D& camera) {
    flecs::system camera_sys =
        registry
            .system<components::PositionComponent, components::CameraComponent>()
            .each([&camera](const components::PositionComponent& pos, const components::CameraComponent& cam) {
                camera.offset = (Vector2){ (float)GetScreenWidth() / 2.0F,
                                           (float)GetScreenHeight() / 2.0F };

                Vector2 diff = Vector2Subtract(Vector2(pos.x, pos.y), camera.target);
                float length = Vector2Length(diff);

                if (length > constants::MIN_CAMERA_EFFECT_LENGTH) {
                    float speed =
                        fmaxf(constants::CAMERA_FRACTION_SPEED * length, constants::MIN_CAMERA_SPEED);
                    camera.target =
                        Vector2Add(camera.target, Vector2Scale(diff, speed * GetFrameTime() / length));
                }
            });
    camera_sys.run();
}

// void UpdateCameraCenter(Camera2D *camera, Player *player, float delta,
//                         int width, int height) {
//   camera->offset = (Vector2){width / 2.0f, height / 2.0f};
//   camera->target = player->position;
// }
//
// void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player,
//                                     float delta, int width, int height) {
//   static float minSpeed = 30;
//   static float minEffectLength = 10;
//   static float fractionSpeed = 0.8f;
//
//   camera->offset = (Vector2){width / 2.0f, height / 2.0f};
//   Vector2 diff = Vector2Subtract(player->position, camera->target);
//   float length = Vector2Length(diff);
//
//   if (length > minEffectLength) {
//     float speed = fmaxf(fractionSpeed * length, minSpeed);
//     camera->target =
//         Vector2Add(camera->target, Vector2Scale(diff, speed * delta /
//         length));
//   }
// }
//
// void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, float
// delta,
//                                   int width, int height) {
//   static Vector2 bbox = {0.2f, 0.2f};
//
//   Vector2 bboxWorldMin = GetScreenToWorld2D(
//       (Vector2){(1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height},
//       *camera);
//   Vector2 bboxWorldMax = GetScreenToWorld2D(
//       (Vector2){(1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height},
//       *camera);
//   camera->offset =
//       (Vector2){(1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height};
//
//   if (player->position.x < bboxWorldMin.x)
//     camera->target.x = player->position.x;
//   if (player->position.y < bboxWorldMin.y)
//     camera->target.y = player->position.y;
//   if (player->position.x > bboxWorldMax.x)
//     camera->target.x = bboxWorldMin.x + (player->position.x -
//     bboxWorldMax.x);
//   if (player->position.y > bboxWorldMax.y)
//     camera->target.y = bboxWorldMin.y + (player->position.y -
//     bboxWorldMax.y);
// }
