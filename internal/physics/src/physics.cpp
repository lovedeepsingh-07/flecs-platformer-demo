#include "physics/physics.hpp"
#include "raymath.h"
#include <algorithm>

namespace Physics {
auto World::add_object(Object *object) -> void {
    if (object != nullptr) {
        m_objects.push_back(object);
    }
};
auto World::remove_object(Object *object) -> void {
    if (object != nullptr) {
        auto iter = std::ranges::find(m_objects, object);
        if (iter != m_objects.end()) {
            m_objects.erase(iter);
        }
    }
};
auto World::step(float delta_time) -> void {
    for (Object *object : m_objects) {
        object->force =
            Vector2Add(object->force, Vector2Scale(world_gravity, object->mass));

        Vector2 acceleration = Vector2Scale(object->force, 1.0F / object->mass);

        object->velocity =
            Vector2Add(object->velocity, Vector2Scale(acceleration, delta_time));
        object->position =
            Vector2Add(object->position, Vector2Scale(object->velocity, delta_time));

        object->force = Vector2(0, 0);
    }
};
} // namespace Physics
