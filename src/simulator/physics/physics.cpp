#include "physics.h"

#include <cmath>
#include <stdexcept>

namespace physics
{
void checkObjectArguments(types::value_t radius, types::value_t mass)
{
    if (radius < constants::PositiveUnit || mass < constants::PositiveUnit)
        throw std::invalid_argument("Attempt to create object of Object with invalid arguments");
}

Object::Object(types::value_t radius, types::value_t mass)
    : m_radius(radius)
    , m_mass(mass)
{
    checkObjectArguments(m_radius, m_mass);
}

Object::Object(const algebra::Point& position, types::value_t radius, types::value_t mass)
    : m_position(position)
    , m_radius(radius)
    , m_mass(mass)
{
    checkObjectArguments(m_radius, m_mass);
}

types::value_t newtonGravity(const Object& lhs, const Object& rhs)
{
    static const types::value_t GravitationalConstant(6.7);

    const types::value_t d = distance(lhs.position(), rhs.position());

    if (d > constants::Zero)
        return GravitationalConstant * lhs.mass() * lhs.mass() / d / d;

    return constants::Zero;
}

types::value_t simpeGravity(const Object& lhs, const Object& rhs)
{
    const types::value_t d = distance(lhs.position(), rhs.position());

    if (d > constants::PositiveUnit)
        return ((d - constants::PositiveUnit) / d) / d;

    return constants::Zero;
}

types::value_t Gravity::operator()(const Object& lhs, const Object& rhs) const
{
    switch (m_type)
    {
    case Type::Simple:
        return simpeGravity(lhs, rhs);
    case Type::Newtonian:
        return newtonGravity(lhs, rhs);
    case Type::None:
    default:
        return constants::Zero;
    }
}

bool crossing(const Object& lhs, const Object& rhs)
{
    const types::value_t d = distance(lhs.position(), rhs.position());
    return less(d, lhs.radius() + rhs.radius());
}
} // namespace physics
