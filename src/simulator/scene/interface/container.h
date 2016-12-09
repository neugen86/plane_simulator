#ifndef CONTAINER_H
#define CONTAINER_H

#include "physics/physics.h"

namespace scene
{
namespace interface
{
class Container
{
    const types::value_t m_width;
    const types::value_t m_height;

public:
    Container(types::value_t width, types::value_t height)
        : m_width(width), m_height(height) {}
    virtual ~Container() {}

    virtual void insertObject(const physics::Object& object) = 0;
    virtual void removeObject(types::obj_id id) = 0;

    types::value_t width() { return m_width; }
    types::value_t height() { return m_height; }
};
} // namespace interface
} // namespace scene

#endif // CONTAINER_H
