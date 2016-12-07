#ifndef SCENE_H
#define SCENE_H

#include "logic.h"
#include "interface/periodic.h"
#include "interface/playable.h"
#include "interface/controllable_container.h"
#include "interchange/broadcaster.h"

namespace scene
{
class Scene
        : public interface::Periodic
        , public interface::Playable
        , public interface::WithGravity
        , public interface::ControllableContainer
        , public interchange::Broadcaster
{
    static const std::size_t DefaultWidth;
    static const std::size_t DefaultHeight;

    Logic m_logic;

    const std::size_t m_width;
    const std::size_t m_height;

    concurrent::spinlock m_grabLock;
    concurrent::spinlock m_insertLock;
    concurrent::spinlock m_removeLock;
    mutable concurrent::spinlock m_gravityLock;

    std::set<types::obj_id> m_removeList;
    std::list<physics::Object> m_insertList;

public:
    explicit Scene(std::size_t width = DefaultWidth,
                   std::size_t height = DefaultHeight);

    physics::Gravity::Type gravityType() const;
    void setGravityType(physics::Gravity::Type type);

    void insertObject(const physics::Object& object);
    void removeObject(types::obj_id id);

    void grabObject(types::obj_id id, const physics::Point& position);
    void releaseObject(types::obj_id id);

    std::size_t width() { return m_width; }
    std::size_t height() { return m_height; }

private:
    void remove();
    void insert();

    bool needSnapshot() const;

    void play();

};
} // namespace scene

#endif // SCENE_H
