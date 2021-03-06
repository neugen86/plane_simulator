#ifndef CONCEPTS_H
#define CONCEPTS_H

#include "algebra.h"

namespace physics
{
class Body
{
public:
    virtual ~Body() {}
};

class Object : public Body
{
    types::obj_id m_id;

    algebra::Point m_position;

    types::value_t m_radius;
    types::value_t m_mass;

public:
    explicit Object(types::value_t radius = constants::DefaultRadius,
                    types::value_t mass = constants::DefaultMass);

    explicit Object(const algebra::Point& position,
                    types::value_t radius = constants::DefaultRadius,
                    types::value_t mass = constants::DefaultMass);

    types::obj_id id() const { return m_id; }
    const algebra::Point& position() const { return m_position; }
    types::value_t radius() const { return m_radius; }
    types::value_t mass() const { return m_mass; }

    void setId(types::obj_id id) { m_id = id; }
    void setPosition(const algebra::Point& position) { m_position = position; }
    void setRadius(types::value_t radius) { m_radius = radius; }
    void setMass(types::value_t mass) { m_mass = mass; }

    void move(types::value_t dx, types::value_t dy)
    {
        m_position = algebra::Point(position().x() + dx, position().y() + dy);
    }
};

class Particle : public Object
{
    bool m_fixed;
    algebra::Vector m_gravity;
    algebra::Vector m_velocity;

public:
    explicit Particle()
        : Object(), m_fixed(false) {}

    explicit Particle(const Object& object)
        : Object(object), m_fixed(false) {}

    bool isFixed() const { return m_fixed; }
    const algebra::Vector& gravity() const { return m_gravity; }
    const algebra::Vector& velocity() const { return m_velocity; }

    void setFixed(bool fixed) { m_fixed = fixed; }
    void setGravity(const algebra::Vector& gravity) { m_gravity = gravity; }
    void setVelocity(const algebra::Vector& velocity) { m_velocity = velocity; }

    void addGravity(const algebra::Vector& gravity) { m_gravity += gravity; }
    void addVelocity(const algebra::Vector& velocity) { m_velocity += velocity; }
};

class Gravity
{
public:
    enum class Type
    {
        None, Simple, Newtonian
    };

private:
    Type m_type;

public:
    explicit Gravity(Type type = Type::Simple) : m_type(type) {}
    types::value_t operator()(const Object& lhs, const Object& rhs) const;
    Type type() const { return m_type; }
};

bool crossing(const Object& lhs, const Object& rhs);

} // namespace physics

#endif // CONCEPTS_H
