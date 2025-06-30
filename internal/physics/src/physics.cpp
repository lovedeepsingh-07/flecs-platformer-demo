#include "physics/physics.hpp"

namespace Physics {
Body::Body(Vector2 position, Vector2 size)
: m_position(position), m_size(size) {}

auto PhysicalWorld::create_body(Vector2 position, Vector2 size) -> Body & {
    m_bodies.emplace_back(position, size);
    return m_bodies.back();
};
} // namespace Physics
