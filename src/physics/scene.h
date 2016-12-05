#ifndef SCENE_H
#define SCENE_H

#include "concepts.h"
#include "utils/iterative.h"

#include <list>
#include <boost/shared_ptr.hpp>

namespace physics
{
class Scene : public utils::Iterative
{
    typedef boost::shared_ptr<Particle> ParticlePtr;

    duration_t m_iterationDuration;

    std::list<ParticlePtr> m_particles;

public:
    static const duration_t DefaultDuration;

    Scene(duration_t duration = DefaultDuration);

    bool addObject(Object& object);
    bool removeObject(obj_id objectId);

private:
    void iterate();

};
} // namespace physics

#endif // SCENE_H
