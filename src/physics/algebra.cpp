#include "algebra.h"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace physics
{
Point::Point(value_t x, value_t y)
    : m_x(x), m_y(y)
{
    if (m_x < constants::Zero || m_y < constants::Zero)
        throw std::invalid_argument("Attempt to create object of Point with invalid arguments");
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
    return vector * (1. / length(vector));
}

value_t distance(const Point& lhs, const Point& rhs)
{
    const value_t dx = lhs.x() - rhs.x();
    const value_t dy = lhs.y() - rhs.y();
    return sqrt(dx * dx + dy * dy);
}

value_t length(const Vector& vector)
{
    return distance(Point(), Point(vector.rx(), vector.ry()));
}

bool same(value_t lhs, value_t rhs)
{
    return std::fabs(lhs - rhs) < std::numeric_limits<value_t>::epsilon();
}
} // namespace algebra
