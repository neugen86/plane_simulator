#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

#include "physics/physics.h"

namespace scene
{
namespace interface
{
class Controllable
{
public:
    virtual ~Controllable() {}

    virtual void grabObject(types::obj_id id, const algebra::Point& position) = 0;
    virtual void releaseObject(types::obj_id id) = 0;
};
} // namespace interface
} // namespace scene

#endif // CONTROLLABLE_H
