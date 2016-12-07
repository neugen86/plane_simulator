#ifndef WITH_GRAVITY_H
#define WITH_GRAVITY_H

#include "physics/physics.h"

namespace scene
{
namespace interface
{
class WithGravity
{
public:
    virtual ~WithGravity() {}

    virtual physics::Gravity::Type gravityType() const = 0;
    virtual void setGravityType(physics::Gravity::Type type) = 0;
};
} // namespace interface
} // namespace scene

#endif // WITH_GRAVITY_H
