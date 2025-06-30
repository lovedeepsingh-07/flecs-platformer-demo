#pragma once

#include <vector>
namespace Physics {

struct Vector2 {
    float x;
    float y;
};

class Body {
  private:
    Vector2 m_position;
    Vector2 m_size;

  public:
    Body(Vector2 position, Vector2 size);
};

class PhysicalWorld {
  private:
    std::vector<Body> m_bodies;

  public:
    auto create_body(Vector2 position, Vector2 size) -> Body &;
    auto step() -> void;
};

} // namespace Physics
