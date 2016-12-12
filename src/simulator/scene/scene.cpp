#include "scene.h"

namespace scene
{
const types::value_t Scene::DefaultWidth(1024.);
const types::value_t Scene::DefaultHeight(768.);
const types::duration_t Scene::DefaultDuration(10);

Scene::Scene(types::value_t width, types::value_t height, types::duration_t duration)
    : interface::Playable(duration)
    , interface::ControllableContainer(width, height)
    , interface::WithGravity()
    , interchange::Broadcaster()
    , m_logic(width, height)
    , m_grabLock()
    , m_insertLock()
    , m_removeLock()
    , m_gravityLock()
{
}

physics::Gravity::Type Scene::gravityType() const
{
    concurrent::guard guard(m_gravityLock);
    return m_logic.gravityType();
}

void Scene::setGravityType(physics::Gravity::Type type)
{
    concurrent::guard guard(m_gravityLock);
    m_logic.setGravityType(type);
}

void Scene::insertObject(const physics::Object& object)
{
    concurrent::guard guard(m_insertLock);
    m_insertList.push_back(object);
}

void Scene::removeObject(types::obj_id id)
{
    concurrent::guard guard(m_removeLock);
    m_removeList.insert(id);
}

void Scene::removeAll()
{
    m_logic.removeAll();
}

void Scene::grabObject(types::obj_id id, const algebra::Point& position)
{
    concurrent::guard guard(m_grabLock);
    m_logic.grabObject(id, position);
}

void Scene::releaseObject(types::obj_id id)
{
    concurrent::guard guard(m_grabLock);
    m_logic.releaseObject(id);
}

void Scene::iteration()
{
    SubscriptionData data;

    bool haveChanges = false;

    {
        concurrent::guard guard(m_removeLock);

        if (!m_removeList.empty())
        {
            data.removedIds = m_removeList;
            haveChanges = true;

            m_logic.remove(m_removeList);
            m_removeList.clear();
        }
    }

    {
        concurrent::guard guard(m_insertLock);
        if (!m_insertList.empty())
        {
            haveChanges = true;

            m_logic.insert(m_insertList);
            m_insertList.clear();
        }
    }

    const bool withSnaphot = haveSubscriptions();

    m_logic.gravitate();
    m_logic.move(withSnaphot);

    if (withSnaphot)
    {
        data.objectList = m_logic.snapshot();
        data.maxDuration = realDuration();

        feed(data, haveChanges);
    }
}

void Scene::onStop()
{
    m_logic.reset();
}

} // namespace scene
