#include "scene.h"

namespace scene
{
using namespace physics;

types::obj_id getObjectId()
{
    static types::obj_id counter(0);
    return counter++;
}

const types::duration_t Scene::DefaultDuration(50);

Scene::Scene(types::duration_t duration)
    : utils::Iterative(duration)
{
}

void Scene::addObject(const Object& object)
{
    boost::mutex::scoped_lock lock(m_addGuard);
    m_addList.push_back(object);
}

void Scene::removeObject(types::obj_id id)
{
    boost::mutex::scoped_lock lock(m_removeGuard);
    m_removeList.insert(id);
}

void Scene::iterate()
{
    insert();
    remove();
    attract();
    collide();
    save();
}

void Scene::insert()
{

}

void Scene::remove()
{

}

void Scene::attract()
{

}

void Scene::collide()
{

}

void Scene::save()
{

}

} // namespace scene
