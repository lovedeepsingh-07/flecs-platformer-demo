#pragma once

#include "raymath.h"
#include <vector>

namespace Physics {


struct Object {
    Vector2 position;
    Vector2 dimensions;
    Vector2 velocity;
    Vector2 force;
    float mass;
};

class World {
  private:
    std::vector<Object *> m_objects;

  public:
    Vector2 world_gravity;
    auto add_object(Object *object) -> void;
    auto remove_object(Object *object) -> void;
    auto step(float delta_time) -> void;
};

} // namespace Physics
