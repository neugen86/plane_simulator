#include "algebra.h"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace constants
{
static const types::value_t PositiveUnit(physics::constants::PositiveUnit);
static const types::value_t NegativeUnit(physics::constants::NegativeUnit);
static const types::value_t Epsilon(std::numeric_limits<types::value_t>::epsilon());
}

const algebra::Vector operator+(const algebra::Vector& lhs, const algebra::Vector& rhs)
{
    const types::value_t rx = lhs.rx() + rhs.rx();
    const types::value_t ry = lhs.ry() + rhs.ry();
    return algebra::Vector(rx, ry);
}

const algebra::Vector operator-(const algebra::Vector& lhs, const algebra::Vector& rhs)
{
    return operator+(lhs, constants:: NegativeUnit * rhs);
}

const algebra::Vector operator+=(algebra::Vector& lhs, const algebra::Vector& rhs)
{
    lhs.setRx(lhs.rx() + rhs.rx());
    lhs.setRy(lhs.ry() + rhs.ry());
    return lhs;
}

const algebra::Vector operator-=(algebra::Vector& lhs, const algebra::Vector& rhs)
{
    return operator+=(lhs, constants::NegativeUnit * rhs);
}

const algebra::Vector operator*(const algebra::Vector& vector, types::value_t scalar)
{
    return algebra::Vector(vector.rx() * scalar, vector.ry() * scalar);
}

const algebra::Vector operator*(types::value_t scalar, const algebra::Vector& vector)
{
    return vector * scalar;
}

const algebra::Vector operator*(const algebra::Vector& lhs, const algebra::Vector& rhs)
{
    return algebra::Vector(lhs.rx() * rhs.rx(), lhs.ry() * rhs.ry());
}

const algebra::Vector operator/(const algebra::Vector& vector, types::value_t scalar)
{
    return operator*(vector, constants::PositiveUnit / scalar);
}

const algebra::Vector operator*=(algebra::Vector& lhs, types::value_t scalar)
{
    lhs.setRx(lhs.rx() * scalar);
    lhs.setRy(lhs.ry() * scalar);
    return lhs;
}

const algebra::Vector operator*=(algebra::Vector& lhs, const algebra::Vector& rhs)
{
    lhs.setRx(lhs.rx() * rhs.rx());
    lhs.setRy(lhs.ry() * rhs.ry());
    return lhs;
}

const algebra::Vector flipHorizontal(const algebra::Vector& vector)
{
    return vector * algebra::Vector(constants::PositiveUnit, constants::NegativeUnit);
}

const algebra::Vector flipVertical(const algebra::Vector& vector)
{
    return vector * algebra::Vector(constants::NegativeUnit, constants::PositiveUnit);
}

const algebra::Vector normalize(const algebra::Vector& vector)
{
    return vector * (constants::PositiveUnit / length(vector));
}

const algebra::Vector turn(const algebra::Vector& vector)
{
    return vector * constants::NegativeUnit;
}

types::value_t distance(const algebra::Point& lhs, const algebra::Point& rhs)
{
    const types::value_t dx = lhs.x() - rhs.x();
    const types::value_t dy = lhs.y() - rhs.y();
    return sqrt(dx * dx + dy * dy);
}

types::value_t length(const algebra::Vector& vector)
{
    return distance(algebra::NullPoint, algebra::Point(vector.rx(), vector.ry()));
}

bool same(types::value_t lhs, types::value_t rhs)
{
    return std::fabs(lhs - rhs) < constants::Epsilon;
}

bool less(types::value_t lhs, types::value_t rhs)
{
    return rhs - lhs > constants::Epsilon;
}
