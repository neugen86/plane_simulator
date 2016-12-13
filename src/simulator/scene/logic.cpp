#include "logic.h"

namespace scene
{
types::obj_id getNewId()
{
    static types::obj_id counter(0);
    return counter++;
}

const physics::Gravity::Type Logic::DefaultGravityType(physics::Gravity::Type::Simple);

Logic::Logic(types::value_t width, types::value_t height, GravityType gravityType)
    : interface::WithGravity()
    , interface::Controllable()
    , m_sceneWidth(width)
    , m_sceneHeight(height)
    , m_gravityType(gravityType)
{
}

GravityType Logic::gravityType() const
{
    return m_gravityType;
}

void Logic::setGravityType(GravityType type)
{
    m_gravityType = type;
}

void Logic::grabObject(types::obj_id id, const algebra::Point& position)
{
    m_grabbed = GrabbedObject(id, position);
}

void Logic::releaseObject(types::obj_id id)
{
    if (m_grabbed.id() == id)
    {

        m_grabbed = GrabbedObject();
    }
}

const std::list<ObjectPtr>& Logic::snapshot(bool force)
{
    if (force)
    {
        m_snapshot.clear();

        for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
        {
            const ObjectPtr pObject(new physics::Object(**i));
            m_snapshot.push_back(pObject);
        }
    }


    return m_snapshot;
}

void Logic::remove(const std::set<types::obj_id>& list)
{
    for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
    {
        if (list.find((*i)->id()) != list.end())
            i = m_particles.erase(i);
    }
}

void Logic::insert(const std::list<physics::Object>& list)
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

void Logic::gravitate()
{
    const physics::Gravity gravity(m_gravityType);

    for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
    {
        physics::Particle& current(**i);

        for (auto j = --m_particles.end(); j != i; --j)
        {
            physics::Particle& other(**j);

            const types::value_t gravityValue = gravity(current, other);

            const algebra::Vector vCurrent(current.position(), other.position());
            const algebra::Vector vOther = turn(vCurrent);

            if (!grabbed(current.id()))
                current.addGravity(gravityValue * normalize(vCurrent));

            other.addGravity(gravityValue * normalize(vOther));
        }
    }
}

void Logic::move(bool withSnapshot)
{
    if (withSnapshot)
        m_snapshot.clear();

    for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
    {
        physics::Particle& current(**i);

        processWalls(current);

        for (auto j = --m_particles.end(); j != i; --j)
        {
            collide(current, **j);
        }

        update(current, withSnapshot);
    }
}

void Logic::removeAll()
{
    m_particles.clear();
}

void Logic::reset()
{
    for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
    {
        physics::Particle& current(**i);
        current.setVelocity(algebra::NullVector);
        current.setGravity(algebra::NullVector);
    }
}

bool Logic::grabbed(types::value_t id)
{
    if (m_grabbed.off())
        return false;

    return m_grabbed.id() == id;
}

void Logic::processWalls(physics::Particle& particle)
{
    const types::value_t radius = particle.radius();
    const algebra::Point& position = particle.position();

    const algebra::Point leftTop(position.x() - radius, position.y() - radius);
    const algebra::Point rightBottom(position.x() + radius, position.y() + radius);

    if (leftTop.x() < physics::constants::Zero || rightBottom.x() > m_sceneWidth)
    {
        particle.setVelocity(flipHorizontal(particle.velocity()));
    }
    else if (leftTop.y() < physics::constants::Zero || rightBottom.y() > m_sceneHeight)
    {
        particle.setVelocity(flipVertical(particle.velocity()));
    }
}

/**
 * Implemented in accordance with the article https://en.wikipedia.org/wiki/Elastic_collision
 * Paragraph "Two-dimensional collision with two moving objects"
 */
void Logic::collide(physics::Particle& current, physics::Particle& other)
{
    if (!physics::crossing(current, other))
        return;

    const types::value_t m1 = current.mass();
    const types::value_t m2 = other.mass();
    const types::value_t m1m2 = m1 + m2;

    const algebra::Vector& v1 = current.velocity();
    const algebra::Vector& v2 = other.velocity();

    const algebra::Vector v1_v2 = v1 - v2;
    const algebra::Vector v2_v1 = turn(v1_v2);

    const algebra::Vector x1_x2(current.position(), other.position());
    const algebra::Vector x2_x1 = turn(x1_x2);

    const types::value_t distance = length(x1_x2);
    const types::value_t distance2 = distance * distance;

    const algebra::Vector v1_new = v1 - ((2 * m2) / m1m2) *
            ((v1_v2 * x1_x2) / distance2) * x1_x2;

    const algebra::Vector v2_new = v2 - ((2 * m1) / m1m2) *
            ((v2_v1 * x2_x1) / distance2) * x2_x1;

    current.setVelocity(v1_new);
    other.setVelocity(v2_new);
}

void Logic::update(physics::Particle& particle, bool withSnapshot)
{
    if (grabbed(particle.id()))
    {
        particle.setGravity(algebra::NullVector);
        particle.setVelocity(algebra::NullVector);
        particle.setPosition(m_grabbed.position());
    }
    else
    {
        const algebra::Vector vector = particle.gravity() + particle.velocity();
        particle.move(vector.rx(), vector.ry());
    }

    algebra::Point pos = particle.position();

    if (pos.x() < 0)
        pos.setX(m_sceneWidth);
    if (pos.x() > m_sceneWidth)
        pos.setX(0);
    if (pos.y() < 0)
        pos.setY(m_sceneHeight);
    if (pos.y() > m_sceneHeight)
        pos.setY(0);

    particle.setPosition(pos);

    if (withSnapshot)
    {
        const ObjectPtr pObject(new physics::Object(particle));
        m_snapshot.push_back(pObject);
    }
}
} // namespace scene
