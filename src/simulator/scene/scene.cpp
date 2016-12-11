#include "scene.h"

namespace scene
{
const types::value_t Scene::DefaultWidth(1024.);
const types::value_t Scene::DefaultHeight(768.);
const types::duration_t Scene::DefaultDuration(10);

Scene::Scene(types::value_t width, types::value_t height, types::duration_t duration)
    : interface::Playable(duration)
    , interface::Controllable()
    , interface::Container(width, height)
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

void Scene::remove()
{
    concurrent::guard guard(m_removeLock);
    m_logic.remove(m_removeList);
    m_removeList.clear();
}

void Scene::insert()
{
    concurrent::guard guard(m_insertLock);
    m_logic.insert(m_insertList);
    m_insertList.clear();
}

void Scene::iteration()
{
    remove();
    insert();

    const bool withSnaphot = haveSubscriptions();

    m_logic.gravitate();
    m_logic.move(withSnaphot);

    if (withSnaphot)
        feed(m_logic.snapshot());
}

void Scene::onStop()
{
    m_logic.reset();
}

} // namespace scene
