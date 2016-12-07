#include "scene.h"

namespace scene
{
const std::size_t Settings::DefaultWidth(100);
const std::size_t Settings::DefaultHeight(30);
const types::duration_t Settings::DefaultTimeout(50);

Scene::Scene(const Settings& settings)
    : utils::Iterative()
    , interchange::Broadcaster()
    , m_director(settings.width(), settings.height())
    , m_settings(settings)
    , m_grabLock()
    , m_insertLock()
    ,m_removeLock()
    , m_gravityLock()
{
}

physics::Gravity::Type Scene::gravityType()
{
    concurrent::guard guard(m_gravityLock);
    return m_director.gravityType();
}

void Scene::setGravityType(physics::Gravity::Type type)
{
    concurrent::guard guard(m_gravityLock);
    m_director.setGravityType(type);
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

void Scene::grabObject(types::obj_id id, const physics::Point& position)
{
    concurrent::guard guard(m_grabLock);
    m_director.grabObject(id, position);
}

void Scene::releaseObject(types::obj_id id)
{
    concurrent::guard guard(m_grabLock);
    m_director.releaseObject(id);
}

void Scene::iterate()
{
    remove();
    insert();

    const bool withSnapshot =
            haveSubscriptions();

    m_director.gravitate();
    m_director.move(withSnapshot);

    if (withSnapshot)
        feed(m_director.snapshot());
}

void Scene::remove()
{
    concurrent::guard guard(m_removeLock);
    m_director.remove(m_removeList);
    m_removeList.clear();
}

void Scene::insert()
{
    concurrent::guard guard(m_insertLock);
    m_director.insert(m_insertList);
    m_insertList.clear();
}
} // namespace scene
