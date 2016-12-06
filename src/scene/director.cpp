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

const std::list<physics::Object>& Director::snapshot()
{
    return m_snapshot;
}

void Director::grabObject(types::obj_id id, const physics::Vector& velocity)
{
    boost::mutex::scoped_lock lock(m_grabGuard);
    m_grabbed = GrabbedObject(id, velocity);
}

void Director::releaseObject(types::obj_id id)
{
    boost::mutex::scoped_lock lock(m_grabGuard);
    if (m_grabbed.id() == id)
        m_grabbed = GrabbedObject();
}

void Director::remove(const std::set<types::obj_id>& list)
{
    const auto listEndIterator = list.end();

    for (auto it = m_particles.begin(); it != m_particles.end(); ++it)
    {
        if (list.find((*it)->id()) != listEndIterator)
            it = m_particles.erase(it);
    }
}

void Director::insert(const std::list<physics::Object>& list)
{
    for (auto i = list.begin(); i != list.end(); ++i)
    {
        bool canBeBlaced = true;
        const physics::Object& newObject(*i);

        for (auto j = m_particles.begin(); j != m_particles.end(); ++j)
        {
            if (physics::crossing(newObject, (**j)))
            {
                canBeBlaced = false;
                break;
            }
        }

        if (canBeBlaced)
        {
            ParticlePtr particle(new physics::Particle(newObject));
            particle->setId(getNewId());

            m_particles.push_back(particle);
        }
    }
}

void Director::attract()
{

}

void Director::collide()
{

}
} // namespace scene
