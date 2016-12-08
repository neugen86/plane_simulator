#ifndef TYPES_H
#define TYPES_H

#include <boost/shared_ptr.hpp>

namespace types
{
typedef int obj_id;
typedef float value_t;
typedef long int duration_t;
} // namespace types

namespace physics
{
class Object;
typedef boost::shared_ptr<Object> ObjectPtr;
} // namespace physics

#endif // TYPES_H
