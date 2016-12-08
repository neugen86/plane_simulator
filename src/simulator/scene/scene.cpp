#include "scene.h"

namespace scene
{
const std::size_t Scene::DefaultWidth(100);
const std::size_t Scene::DefaultHeight(30);

Scene::Scene(std::size_t width, std::size_t height)
    : interface::Periodic()
    , interface::Playable()
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

void Scene::grabObject(types::obj_id id, const physics::Point& position)
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

bool Scene::needSnapshot() const
{
    const bool isOver = expired();
    return isOver ? haveSubscriptions() : false;
}

void Scene::play()
{
    remove();
    insert();

    const bool withSnaphot = needSnapshot();

    m_logic.gravitate();
    m_logic.move(withSnaphot);

    if (withSnaphot)
        feed(m_logic.snapshot());
}
} // namespace scene
