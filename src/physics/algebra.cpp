#include "algebra.h"

#include <math.h>
#include <stdexcept>

namespace physics
{
Point::Point(value_t x, value_t y)
    : m_x(x), m_y(y)
{
    if (m_x < constants::Zero || m_y < constants::Zero)
        throw std::invalid_argument("Attempt to create object of Point with invalid arguments");
}

Vector::Vector(value_t rx, value_t ry)
    : m_rx(rx), m_ry(ry)
{
    if (m_rx < constants::Zero || m_ry < constants::Zero)
        throw std::invalid_argument("Attempt to create object of Vector with invalid arguments");
}

const Vector operator+ (const Vector& lhs, const Vector& rhs)
{
    const value_t rx = lhs.rx() + rhs.rx();
    const value_t ry = lhs.ry() + rhs.ry();
    return Vector(rx, ry);
}

const Vector operator* (const Vector& vector, value_t scalar)
{
    return Vector(vector.rx() * scalar, vector.ry() * scalar);
}

const Vector operator* (value_t scalar, const Vector& vector)
{
    return vector * scalar;
}

const Vector normalize(const Vector& vector)
{
    value_t length = distance(Point(), Point(vector.rx(), vector.ry()));
    return vector * (1 / length);
}

value_t distance(const Point& lhs, const Point& rhs)
{
    const value_t dx = lhs.x() - rhs.x();
    const value_t dy = lhs.y() - rhs.y();
    return sqrt(dx * dx + dy * dy);
}
} // namespace algebra
