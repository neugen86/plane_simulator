#ifndef SCENE_H
#define SCENE_H

#include "director.h"
#include "utils/iterative.h"
#include "interchange/broadcaster.h"

namespace scene
{
class Settings
{
    static const std::size_t DefaultWidth;
    static const std::size_t DefaultHeight;
    static const types::duration_t DefaultTimeout;

    std::size_t m_width;
    std::size_t m_height;
    types::duration_t m_timeout;

public:
    explicit Settings(types::value_t width = DefaultWidth,
                      types::value_t height = DefaultHeight,
                      types::duration_t timeout = DefaultTimeout)
        : m_width(width), m_height(height), m_timeout(timeout) {}

    std::size_t width() const { return m_width; }
    std::size_t height() const { return m_height; }
    types::duration_t timeout() const { return m_timeout; }
};

class Scene
        : public utils::Iterative
        , public interchange::Broadcaster<physics::Object>
{
    Director m_director;

    const Settings m_settings;

    boost::mutex m_grabGuard;
    boost::mutex m_insertGuard;
    boost::mutex m_removeGuard;
    boost::mutex m_gravityGuard;

    std::list<physics::Object> m_addList;
    std::set<types::obj_id> m_removeList;

public:
    Scene(const Settings& settings = Settings());

    physics::Gravity::Type gravityType();
    void setGravityType(physics::Gravity::Type type);

    void addObject(const physics::Object& object);
    void removeObject(types::obj_id id);

    void grabObject(types::obj_id id, const physics::Vector& velocity);
    void releaseObject(types::obj_id id);

    const Settings& settings() const { return m_settings; }

private:
    void iterate();

    void remove();
    void insert();

};
} // namespace scene

#endif // SCENE_H
