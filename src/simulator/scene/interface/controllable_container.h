#ifndef CONTROLLABLE_CONTAINER_H
#define CONTROLLABLE_CONTAINER_H

#include "container.h"
#include "controllable.h"

namespace scene
{
namespace interface
{
class ControllableContainer
        : public Container
        , public Controllable
{
};
} // namespace interface
} // namespace scene

#endif // CONTROLLABLE_CONTAINER_H
