#include "concepts.h"

#include <math.h>
#include <stdexcept>

namespace physics
{
void checkObjectArguments(value_t radius, value_t mass)
{
    if (radius < constants::Unitary || mass < constants::Unitary)
        throw std::invalid_argument("Attempt to create object of Object with invalid arguments");
}

Object::Object(value_t radius, value_t mass)
    : m_radius(radius)
    , m_mass(mass)
{
    checkObjectArguments(m_radius, m_mass);
}

Object::Object(const Point& position, value_t radius, value_t mass)
    : m_position(position)
    , m_radius(radius)
    , m_mass(mass)
{
    checkObjectArguments(m_radius, m_mass);
}

value_t simpeGravity(const Object& lhs, const Object& rhs)
{
    const value_t d = distance(lhs.position(), rhs.position());

    if (d > constants::Unitary)
        return ((d - constants::Unitary) / d) / d;

    return constants::Zero;
}

value_t newtonGravity(const Object& lhs, const Object& rhs)
{
    static const value_t GravitationalConstant(constants::Unitary);

    const value_t d = distance(lhs.position(), rhs.position());

    if (d > constants::Zero)
        return GravitationalConstant * lhs.mass() * lhs.mass() / d / d;

    return constants::Zero;
}
} // namespace physics
