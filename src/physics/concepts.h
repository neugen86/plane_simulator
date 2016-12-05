#ifndef CONCEPTS_H
#define CONCEPTS_H

#include "algebra.h"

namespace physics
{
class Object
{
    types::obj_id m_id;

    Point m_position;

    types::value_t m_radius;
    types::value_t m_mass;

public:
    explicit Object(types::value_t radius = constants::DefaultRadius,
                    types::value_t mass = constants::DefaultMass);

    explicit Object(const Point& position,
                    types::value_t radius = constants::DefaultRadius,
                    types::value_t mass = constants::DefaultMass);

    types::obj_id id() const { return m_id; }
    const Point& position() const { return m_position; }
    types::value_t radius() const { return m_radius; }
    types::value_t mass() const { return m_mass; }

    void setId(types::obj_id id) { m_id = id; }
    void setPosition(const Point& position) { m_position = position; }
    void setRadius(types::value_t radius) { m_radius = radius; }
    void setMass(types::value_t mass) { m_mass = mass; }
};

class Particle : public Object
{
    bool m_fixed;
    Vector m_velocity;

public:
    explicit Particle()
        : Object(), m_fixed(false) {}

    explicit Particle(const Object& object)
        : Object(object), m_fixed(false) {}

    bool isFixed() const { return m_fixed; }
    const Vector& velocity() const { return m_velocity; }

    void setFixed(bool fixed) { m_fixed = fixed; }
    void setVelocity(const Vector& velocity) { m_velocity = velocity; }
};

types::value_t simpeGravity(const Object& lhs, const Object& rhs);
types::value_t newtonGravity(const Object& lhs, const Object& rhs);

} // namespace physics

#endif // CONCEPTS_H
