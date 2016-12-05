#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "constants.h"

namespace physics
{
class Point
{
    types::value_t m_x;
    types::value_t m_y;

public:
    explicit Point(types::value_t x = constants::Zero,
                   types::value_t y = constants::Zero);

    types::value_t x() const { return m_x; }
    types::value_t y() const { return m_y; }

    void setX(types::value_t x) { m_x = x; }
    void setY(types::value_t y) { m_y = y; }
};

class Vector
{
    types::value_t m_rx;
    types::value_t m_ry;

public:
    explicit Vector(const Point& point)
        : m_rx(point.x()), m_ry(point.y()) {}

    explicit Vector(types::value_t rx = constants::Zero,
                    types::value_t ry = constants::Zero)
        : m_rx(rx), m_ry(ry) {}

    types::value_t rx() const { return m_rx; }
    types::value_t ry() const { return m_ry; }

    void setRx(types::value_t rx) { m_rx = rx; }
    void setRy(types::value_t ry) { m_ry = ry; }
};

const Vector operator+(const Vector& lhs, const Vector& rhs);
const Vector operator*(const Vector& vector, types::value_t scalar);
const Vector operator*(types::value_t scalar, const Vector& vector);
const Vector normalize(const Vector& vector);

types::value_t distance(const Point& lhs, const Point& rhs);
types::value_t length(const Vector& vector);

bool same(types::value_t lhs, types::value_t rhs);

} // namespace algebra

#endif // ALGEBRA_H
