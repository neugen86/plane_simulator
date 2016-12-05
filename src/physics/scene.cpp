#include "scene.h"

namespace physics
{

obj_id getObjectId()
{
    static obj_id counter(0);
    return counter++;
}

const duration_t Scene::DefaultDuration(50);

Scene::Scene(duration_t duration)
    : utils::Iterative(duration)
    , m_iterationDuration(duration)
{
}

bool Scene::addObject(Object& object)
{
    object.setId(getObjectId());

    ParticlePtr particle(new Particle(object));
    m_particles.push_back(particle);

    return true;
}

bool Scene::removeObject(obj_id objectId)
{
    bool wasRemoved = false;

    auto predicate = [&wasRemoved, objectId](const ParticlePtr& ptr)
    {
        bool isEqual = ptr->id() == objectId;
        if (isEqual && !wasRemoved)
            wasRemoved = true;
        return isEqual;
    };

    m_particles.remove_if(predicate);

    return wasRemoved;
}

void Scene::iterate()
{
}
} // namespace physics
