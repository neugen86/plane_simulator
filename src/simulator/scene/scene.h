#ifndef SCENE_H
#define SCENE_H

#include "logic.h"
#include "interface/periodic.h"
#include "interface/playable.h"
#include "interface/container.h"
#include "interchange/broadcaster.h"

namespace scene
{
class Scene
        : public interface::Periodic
        , public interface::Playable
        , public interface::Controllable
        , public interface::Container
        , public interface::WithGravity
        , public interchange::Broadcaster
{
    Logic m_logic;

    concurrent::spinlock m_grabLock;
    concurrent::spinlock m_insertLock;
    concurrent::spinlock m_removeLock;
    mutable concurrent::spinlock m_gravityLock;

    std::set<types::obj_id> m_removeList;
    std::list<physics::Object> m_insertList;

    static const std::size_t DefaultWidth;
    static const std::size_t DefaultHeight;

public:
    explicit Scene(std::size_t width = DefaultWidth,
                   std::size_t height = DefaultHeight);

    physics::Gravity::Type gravityType() const;
    void setGravityType(physics::Gravity::Type type);

    void insertObject(const physics::Object& object);
    void removeObject(types::obj_id id);

    void grabObject(types::obj_id id, const physics::Point& position);
    void releaseObject(types::obj_id id);

private:
    void remove();
    void insert();

    bool needSnapshot() const;

    void play();

};
} // namespace scene

#endif // SCENE_H
