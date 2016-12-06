#include "algebra.h"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace physics
{
static const types::value_t Epsilon(std::numeric_limits<types::value_t>::epsilon());

Point::Point(types::value_t x, types::value_t y)
    : m_x(x), m_y(y)
{
    if (m_x < constants::Zero || m_y < constants::Zero)
        throw std::invalid_argument("Attempt to create object of Point with invalid arguments");
}

const Vector operator+(const Vector& lhs, const Vector& rhs)
{
    const types::value_t rx = lhs.rx() + rhs.rx();
    const types::value_t ry = lhs.ry() + rhs.ry();
    return Vector(rx, ry);
}

const Vector operator*(const Vector& vector, types::value_t scalar)
{
    return Vector(vector.rx() * scalar, vector.ry() * scalar);
}

const Vector operator*(types::value_t scalar, const Vector& vector)
{
    return vector * scalar;
}

const Vector normalize(const Vector& vector)
{
    return vector * (1. / length(vector));
}

types::value_t distance(const Point& lhs, const Point& rhs)
{
    const types::value_t dx = lhs.x() - rhs.x();
    const types::value_t dy = lhs.y() - rhs.y();
    return sqrt(dx * dx + dy * dy);
}

types::value_t length(const Vector& vector)
{
    return distance(Point(), Point(vector.rx(), vector.ry()));
}

bool less(types::value_t lhs, types::value_t rhs)
{
    return rhs - lhs > Epsilon;
}

bool same(types::value_t lhs, types::value_t rhs)
{
    return std::fabs(lhs - rhs) < Epsilon;
}
} // namespace algebra
