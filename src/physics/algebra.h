#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "types.h"
#include "constants.h"

namespace physics
{
class Point
{
    value_t m_x;
    value_t m_y;

public:
    explicit Point(value_t x = constants::Zero,
                   value_t y = constants::Zero);

    value_t x() const { return m_x; }
    value_t y() const { return m_y; }

    void setX(value_t x) { m_x = x; }
    void setY(value_t y) { m_y = y; }
};

class Vector
{
    value_t m_rx;
    value_t m_ry;

public:
    explicit Vector(const Point& point)
        : m_rx(point.x()), m_ry(point.y()) {}

    explicit Vector(value_t rx = constants::Zero,
                    value_t ry = constants::Zero);

    const Point operator() () { return Point(m_rx, m_ry); }

    value_t rx() const { return m_rx; }
    value_t ry() const { return m_ry; }

    void setRx(value_t rx) { m_rx = rx; }
    void setRy(value_t ry) { m_ry = ry; }
};

const Vector operator+ (const Vector& lhs, const Vector& rhs);
const Vector operator* (const Vector& vector, value_t scalar);
const Vector operator* (value_t scalar, const Vector& vector);
const Vector normalize(const Vector& vector);

value_t distance(const Point& lhs, const Point& rhs);

} // namespace algebra

#endif // ALGEBRA_H
