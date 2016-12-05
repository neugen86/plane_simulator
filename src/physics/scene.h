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
    static const duration_t DefaultDuration;

    typedef boost::shared_ptr<Particle> ParticlePtr;

    std::list<ParticlePtr> m_particles;

public:
    Scene(duration_t duration = DefaultDuration);

    bool addObject(Object& object);
    bool removeObject(obj_id objectId);

private:
    void iterate();

};
} // namespace physics

#endif // SCENE_H
