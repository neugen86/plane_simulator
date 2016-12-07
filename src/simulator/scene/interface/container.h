#ifndef CONTAINER_H
#define CONTAINER_H

#include "physics/physics.h"

namespace scene
{
namespace interface
{
class Container
{
public:
    virtual ~Container() {}

    virtual void insertObject(const physics::Object& object) = 0;
    virtual void removeObject(types::obj_id id) = 0;
};
} // namespace interface
} // namespace scene

#endif // CONTAINER_H
