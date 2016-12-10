#ifndef LOGIC_H
#define LOGIC_H

#include <set>
#include <list>

#include "interface/with_gravity.h"
#include "interface/controllable.h"

namespace scene
{
typedef physics::Gravity::Type GravityType;
typedef boost::shared_ptr<physics::Object> ObjectPtr;

class Logic
        : public interface::WithGravity
        , public interface::Controllable
{
public:

    static const GravityType DefaultGravityType;

    typedef boost::shared_ptr<physics::Particle> ParticlePtr;

    types::value_t m_sceneWidth;
    types::value_t m_sceneHeight;

    GravityType m_gravityType;

    std::list<ObjectPtr> m_snapshot;
    std::list<ParticlePtr> m_particles;

    class GrabbedObject
    {
        bool m_off;
        types::obj_id m_id;
        algebra::Point m_position;

    public:
        GrabbedObject()
            : m_off(false) {}

        GrabbedObject(types::obj_id id,
                      const algebra::Point& position)
            : m_off(true) , m_id(id) , m_position(position) {}

        bool off() const { return m_off; }
        types::obj_id id() const { return m_id; }
        const algebra::Point& position() const { return m_position; }

    } m_grabbed;

public:
    Logic(types::value_t width, types::value_t height,
          GravityType gravityType = DefaultGravityType);

    GravityType gravityType() const;
    void setGravityType(GravityType type);

    void grabObject(types::obj_id id, const algebra::Point& position);
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
