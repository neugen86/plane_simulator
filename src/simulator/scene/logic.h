#ifndef LOGIC_H
#define LOGIC_H

#include <set>
#include <list>

#include "interface/with_gravity.h"
#include "interface/controllable.h"

namespace scene
{
typedef boost::shared_ptr<physics::Object> ObjectPtr;

class Logic
        : public interface::WithGravity
        , public interface::Controllable
{
    typedef boost::shared_ptr<physics::Particle> ParticlePtr;

    std::size_t m_sceneWidth;
    std::size_t m_sceneHeight;

    physics::Gravity m_gravity;

    std::list<ObjectPtr> m_snapshot;
    std::list<ParticlePtr> m_particles;

    class GrabbedObject
    {
        bool m_off;
        types::obj_id m_id;
        physics::Point m_position;

    public:
        GrabbedObject()
            : m_off(false) {}

        GrabbedObject(types::obj_id id,
                      const physics::Point& position)
            : m_off(true) , m_id(id) , m_position(position) {}

        bool off() const { return m_off; }
        types::obj_id id() const { return m_id; }
        const physics::Point& position() const { return m_position; }

    } m_grabbed;

public:
    explicit Logic(std::size_t width, std::size_t height);

    physics::Gravity::Type gravityType() const;
    void setGravityType(physics::Gravity::Type type);

    void grabObject(types::obj_id id, const physics::Point& position);
    void releaseObject(types::obj_id id);

    const std::list<ObjectPtr>& snapshot() const;

    void remove(const std::set<types::obj_id>& list);
    void insert(const std::list<physics::Object>& list);
    void gravitate();
    void move(bool withSnapshot);

private:
    bool grabbed(types::value_t id);
    void processWalls(physics::Particle& particle);
    void collide(physics::Particle& current, physics::Particle& other);
    void update(physics::Particle& particle, bool withSnapshot);

};
} // namespace scene

#endif // LOGIC_H
