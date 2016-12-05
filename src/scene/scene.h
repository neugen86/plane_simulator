#ifndef SCENE_H
#define SCENE_H

#include <set>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include "physics/concepts.h"
#include "utils/iterative.h"

namespace scene
{
class Scene : public utils::Iterative
{
    static const types::duration_t DefaultDuration;
    typedef boost::shared_ptr<physics::Particle> ParticlePtr;

    boost::mutex m_addGuard;
    boost::mutex m_removeGuard;

    std::list<physics::Object> m_addList;
    std::set<types::obj_id> m_removeList;

    std::list<ParticlePtr> m_particles;

public:
    Scene(types::duration_t duration = DefaultDuration);

    void addObject(const physics::Object& object);
    void removeObject(types::obj_id id);

private:
    void iterate();

    void insert();
    void remove();
    void attract();
    void collide();
    void save();
};
} // namespace scene

#endif // SCENE_H
