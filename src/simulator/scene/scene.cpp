#include "scene.h"

namespace scene
{
const std::size_t Settings::DefaultWidth(100);
const std::size_t Settings::DefaultHeight(30);
const types::duration_t Settings::DefaultTimeout(50);

Scene::Scene(const Settings& settings)
    : utils::Iterative(settings.timeout())
    , m_director(settings.width(), settings.height())
    , m_settings(settings)
{
}

physics::Gravity::Type Scene::gravityType()
{
    boost::mutex::scoped_lock lock(m_gravityGuard);
    return m_director.gravityType();
}

void Scene::setGravityType(physics::Gravity::Type type)
{
    boost::mutex::scoped_lock lock(m_gravityGuard);
    m_director.setGravityType(type);
}

void Scene::addObject(const physics::Object& object)
{
    boost::mutex::scoped_lock lock(m_insertGuard);
    m_addList.push_back(object);
}

void Scene::removeObject(types::obj_id id)
{
    boost::mutex::scoped_lock lock(m_removeGuard);
    m_removeList.insert(id);
}

void Scene::grabObject(types::obj_id id, const physics::Point& position)
{
    boost::mutex::scoped_lock lock(m_grabGuard);
    m_director.grabObject(id, position);
}

void Scene::releaseObject(types::obj_id id)
{
    boost::mutex::scoped_lock lock(m_grabGuard);
    m_director.releaseObject(id);
}

void Scene::iterate()
{
    remove();
    insert();

    const bool haveSubscriptions = broadcasting();

    m_director.gravitate();
    m_director.move(haveSubscriptions);

    if (haveSubscriptions)
        feed(m_director.snapshot());
}

void Scene::remove()
{
    boost::mutex::scoped_lock lock(m_removeGuard);
    m_director.remove(m_removeList);
    m_removeList.clear();
}

void Scene::insert()
{
    boost::mutex::scoped_lock lock(m_insertGuard);
    m_director.insert(m_addList);
    m_addList.clear();
}
} // namespace scene
