#include "algebra.h"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace physics
{
static const types::value_t Epsilon(std::numeric_limits<types::value_t>::epsilon());

const Vector operator +(const Vector& lhs, const Vector& rhs)
{
    const types::value_t rx = lhs.rx() + rhs.rx();
    const types::value_t ry = lhs.ry() + rhs.ry();
    return Vector(rx, ry);
}

const Vector operator -(const Vector& lhs, const Vector& rhs)
{
    return operator +(lhs, constants::NegativeUnit * rhs);
}

const Vector operator +=(Vector& lhs, const Vector& rhs)
{
    lhs.setRx(lhs.rx() + rhs.rx());
    lhs.setRy(lhs.ry() + rhs.ry());
    return lhs;
}

const Vector operator -=(Vector& lhs, const Vector& rhs)
{
    return operator +=(lhs, constants::NegativeUnit * rhs);
}

const Vector operator *(const Vector& vector, types::value_t scalar)
{
    return Vector(vector.rx() * scalar, vector.ry() * scalar);
}

const Vector operator *(types::value_t scalar, const Vector& vector)
{
    return vector * scalar;
}

const Vector operator *(const Vector& lhs, const Vector& rhs)
{
    return Vector(lhs.rx() * rhs.rx(), lhs.ry() * rhs.ry());
}

const Vector operator /(const Vector& vector, types::value_t scalar)
{
    return operator *(vector, constants::PositiveUnit / scalar);
}

const Vector operator *=(Vector& lhs, types::value_t scalar)
{
    lhs.setRx(lhs.rx() * scalar);
    lhs.setRy(lhs.ry() * scalar);
    return lhs;
}

const Vector operator *=(Vector& lhs, const Vector& rhs)
{
    lhs.setRx(lhs.rx() * rhs.rx());
    lhs.setRy(lhs.ry() * rhs.ry());
    return lhs;
}

const Vector flipHorizontal(const Vector& vector)
{
    return vector * Vector(constants::PositiveUnit, constants::NegativeUnit);
}

const Vector flipVertical(const Vector& vector)
{
    return vector * Vector(constants::NegativeUnit, constants::PositiveUnit);
}

const Vector normalize(const Vector& vector)
{
    return vector * (constants::PositiveUnit / length(vector));
}

const Vector turn(const Vector& vector)
{
    return vector * constants::NegativeUnit;
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

bool same(types::value_t lhs, types::value_t rhs)
{
    return std::fabs(lhs - rhs) < Epsilon;
}

bool less(types::value_t lhs, types::value_t rhs)
{
    return rhs - lhs > Epsilon;
}
} // namespace algebra
