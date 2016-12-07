#include "director.h"

namespace scene
{
types::obj_id getNewId()
{
    static types::obj_id counter(0);
    return counter++;
}

Director::Director(std::size_t width, std::size_t height)
    : m_sceneWidth(width)
    , m_sceneHeight(height)
{
}

const std::list<ObjectPtr>& Director::snapshot() const
{
    return m_snapshot;
}

physics::Gravity::Type Director::gravityType() const
{
    return m_gravity.type();
}

void Director::setGravityType(physics::Gravity::Type type)
{
    if (m_gravity.type() != type)
        m_gravity = physics::Gravity(type);
}

void Director::grabObject(types::obj_id id, const physics::Point& position)
{
    m_grabbed = GrabbedObject(id, position);
}

void Director::releaseObject(types::obj_id id)
{
    if (m_grabbed.id() == id)
        m_grabbed = GrabbedObject();
}

void Director::remove(const std::set<types::obj_id>& list)
{
    for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
    {
        if (list.find((*i)->id()) != list.end())
            i = m_particles.erase(i);
    }
}

void Director::insert(const std::list<physics::Object>& list)
{
    for (auto i = list.begin(); i != list.end(); ++i)
    {
        bool fit = true;

        const physics::Object& newObject(*i);

        for (auto j = m_particles.begin(); j != m_particles.end(); ++j)
        {
            if (physics::crossing(newObject, (**j)))
            {
                fit = false;
                break;
            }
        }

        if (fit)
        {
            ParticlePtr particle(new physics::Particle(newObject));
            particle->setId(getNewId());

            m_particles.push_back(particle);
        }
    }
}

void Director::gravitate()
{
    types::value_t gravityValue;

    for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
    {
        physics::Particle& current(**i);

        for (auto j = ++i; j != m_particles.end(); ++j)
        {
            physics::Particle& other(**j);

            gravityValue = m_gravity(current, other);

            const physics::Vector vCurrent(current.position(), other.position());
            const physics::Vector vOther = physics::turn(vCurrent);

            if (!grabbed(current.id()))
                current.addGravity(gravityValue * physics::normalize(vCurrent));

            other.addGravity(gravityValue * physics::normalize(vOther));
        }
    }
}

void Director::move(bool withSnapshot)
{
    if (withSnapshot)
        m_snapshot.clear();

    for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
    {
        physics::Particle& current(**i);

        processWalls(current);

        for (auto j = ++i; j != m_particles.end(); ++j)
        {
            collide(current, **j);
        }

        saveState(current, withSnapshot);
    }
}

bool Director::grabbed(types::value_t id)
{
    if (m_grabbed.off())
        return false;

    return m_grabbed.id() == id;
}

void Director::processWalls(physics::Particle& particle)
{
    const types::value_t radius = particle.radius();
    const physics::Point& position = particle.position();

    const physics::Point leftTop(position.x() - radius, position.y() - radius);
    const physics::Point rightBottom(position.x() + radius, position.y() + radius);

    if (leftTop.x() < physics::constants::Zero || rightBottom.x() > m_sceneWidth)
    {
        particle.setVelocity(physics::flipHorizontal(particle.velocity()));
    }
    else if (leftTop.y() < physics::constants::Zero || rightBottom.y() > m_sceneHeight)
    {
        particle.setVelocity(physics::flipVertical(particle.velocity()));
    }
}

/**
 * Implemented in accordance with the article https://en.wikipedia.org/wiki/Elastic_collision
 * Paragraph "Two-dimensional collision with two moving objects"
 */
void Director::collide(physics::Particle& current, physics::Particle& other)
{
    if (!physics::crossing(current, other))
        return;

    const types::value_t m1 = current.mass();
    const types::value_t m2 = other.mass();
    const types::value_t m1m2 = m1 + m2;

    const physics::Vector& v1 = current.velocity();
    const physics::Vector& v2 = other.velocity();

    const physics::Vector v1_v2 = v1 - v2;
    const physics::Vector v2_v1 = physics::turn(v1_v2);

    const physics::Vector x1_x2(current.position(), other.position());
    const physics::Vector x2_x1 = physics::turn(x1_x2);

    const types::value_t distance = physics::length(x1_x2);
    const types::value_t distance2 = distance * distance;

    const physics::Vector v1_new = v1 - ((2 * m2) / m1m2) *
            ((v1_v2 * x1_x2) / distance2) * x1_x2;

    const physics::Vector v2_new = v2 - ((2 * m1) / m1m2) *
            ((v2_v1 * x2_x1) / distance2) * x2_x1;

    current.setVelocity(v1_new);
    other.setVelocity(v2_new);
}

void Director::saveState(physics::Particle& particle, bool withSnapshot)
{
    if (grabbed(particle.id()))
    {
        particle.setPosition(m_grabbed.position());
    }
    else
    {
        const physics::Vector vector = particle.gravity() + particle.velocity();
        particle.move(vector.rx(), vector.ry());
    }

    particle.setGravity(physics::Vector());

    if (withSnapshot)
    {
        const ObjectPtr pObject(new physics::Object(particle));
        m_snapshot.push_back(pObject);
    }
}
} // namespace scene
