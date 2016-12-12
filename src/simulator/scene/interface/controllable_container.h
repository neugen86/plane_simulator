#ifndef CONTROLLABLE_CONTAINER_H
#define CONTROLLABLE_CONTAINER_H

#include "container.h"
#include "controllable.h"

namespace scene
{
namespace interface
{
class ControllableContainer
        : public Controllable
        , public Container
{
public:
    ControllableContainer(types::value_t width, types::value_t height)
        : Controllable() , Container(width, height) {}
};
} // namespace interface
} // namespace scene

#endif // CONTROLLABLE_CONTAINER_H
