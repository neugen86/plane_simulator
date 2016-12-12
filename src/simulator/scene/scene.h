#ifndef SCENE_H
#define SCENE_H

#include "logic.h"
#include "interface/playable.h"
#include "interchange/broadcaster.h"
#include "interface/controllable_container.h"

namespace scene
{
class Scene
        : public interface::Playable
        , public interface::ControllableContainer
        , public interface::WithGravity
        , public interchange::Broadcaster
{
    Logic m_logic;

    std::set<types::obj_id> m_removeList;
    std::list<physics::Object> m_insertList;

    concurrent::spinlock m_grabLock;
    concurrent::spinlock m_insertLock;
    concurrent::spinlock m_removeLock;
    mutable concurrent::spinlock m_gravityLock;

    static const types::value_t DefaultWidth;
    static const types::value_t DefaultHeight;
    static const types::duration_t DefaultDuration;

public:
    explicit Scene(types::value_t width = DefaultWidth,
                   types::value_t height = DefaultHeight,
                   types::duration_t duration = DefaultDuration);

    physics::Gravity::Type gravityType() const;
    void setGravityType(physics::Gravity::Type type);

    void insertObject(const physics::Object& object);
    void removeObject(types::obj_id id);
    void removeAll();

    void grabObject(types::obj_id id, const algebra::Point& position);
    void releaseObject(types::obj_id id);

private:
    void iteration();

    void onStop();

};
} // namespace scene

#endif // SCENE_H
