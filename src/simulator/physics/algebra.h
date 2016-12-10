#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "constants.h"

namespace algebra
{
class Point
{
    types::value_t m_x;
    types::value_t m_y;

public:
    explicit Point(types::value_t x = physics::constants::Zero,
                   types::value_t y = physics::constants::Zero)
        : m_x(x), m_y(y) {}

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

    explicit Vector(types::value_t rx = physics::constants::Zero,
                    types::value_t ry = physics::constants::Zero)
        : m_rx(rx), m_ry(ry) {}

    Vector(const Point& from, const Point& to)
        : m_rx(to.x() - from.x()), m_ry(to.y() - from.y()) {}

    types::value_t rx() const { return m_rx; }
    types::value_t ry() const { return m_ry; }

    void setRx(types::value_t rx) { m_rx = rx; }
    void setRy(types::value_t ry) { m_ry = ry; }
};
} // namespace algebra

const algebra::Vector operator+(const algebra::Vector& lhs, const algebra::Vector& rhs);
const algebra::Vector operator-(const algebra::Vector& lhs, const algebra::Vector& rhs);
const algebra::Vector operator+=(algebra::Vector& lhs, const algebra::Vector& rhs);
const algebra::Vector operator-=(algebra::Vector& lhs, const algebra::Vector& rhs);
const algebra::Vector operator*(const algebra::Vector& vector, types::value_t scalar);
const algebra::Vector operator*(types::value_t scalar, const algebra::Vector& vector);
const algebra::Vector operator*(const algebra::Vector& lhs, const algebra::Vector& rhs);
const algebra::Vector operator/(const algebra::Vector& vector, types::value_t scalar);
const algebra::Vector operator*=(algebra::Vector& lhs, types::value_t scalar);
const algebra::Vector operator*=(algebra::Vector& lhs, const algebra::Vector& rhs);

const algebra::Vector flipHorizontal(const algebra::Vector& vector);
const algebra::Vector flipVertical(const algebra::Vector& vector);
const algebra::Vector normalize(const algebra::Vector& vector);
const algebra::Vector turn(const algebra::Vector& vector);

types::value_t distance(const algebra::Point& lhs, const algebra::Point& rhs);
types::value_t length(const algebra::Vector& vector);

bool same(types::value_t lhs, types::value_t rhs);
bool less(types::value_t lhs, types::value_t rhs);

#endif // ALGEBRA_H
