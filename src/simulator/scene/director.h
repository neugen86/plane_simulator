#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <set>
#include <list>
#include <boost/shared_ptr.hpp>

#include "physics/physics.h"

namespace scene
{
class Director
{
    typedef boost::shared_ptr<physics::Particle> ParticlePtr;

    std::size_t m_sceneWidth;
    std::size_t m_sceneHeight;

    physics::Gravity m_gravity;

    std::list<ParticlePtr> m_particles;
    std::list<physics::Object> m_snapshot;

    class GrabbedObject
    {
        bool m_released;
        types::obj_id m_id;
        physics::Vector m_velocity;

    public:
        GrabbedObject()
            : m_released(true) {}

        GrabbedObject(types::obj_id id,
                      const physics::Vector& velocity)
            : m_released(false) , m_id(id) , m_velocity(velocity) {}

        bool released() const { return m_released; }
        types::obj_id id() const { return m_id; }
        const physics::Vector& velocity() { return m_velocity; }

    } m_grabbed;

public:
    explicit Director(std::size_t width, std::size_t height);

    const std::list<physics::Object>& snapshot() const;

    physics::Gravity::Type gravityType() const;
    void setGravityType(physics::Gravity::Type type);

    void grabObject(types::obj_id id, const physics::Vector& velocity);
    void releaseObject(types::obj_id id);

    void remove(const std::set<types::obj_id>& list);
    void insert(const std::list<physics::Object>& list);
    void attract();
    void collide();

};
} // namespace scene

#endif // DIRECTOR_H
