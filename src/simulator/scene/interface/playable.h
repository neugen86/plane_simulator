#ifndef PLAYABLE_H
#define PLAYABLE_H

#include <boost/chrono.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>

#include "physics/types.h"
#include "concurrent/lock.h"
#include "concurrent/event.h"

enum class PlaybackState
{
    STARTED, PAUSED, STOPPED
};

namespace scene
{
namespace interface
{
class Playable
        : private boost::noncopyable
{
    bool m_paused;
    bool m_stopped;
    bool m_finishFlag;

    PlaybackState m_state;

    concurrent::event m_resumeEvent;

    mutable concurrent::spinlock m_lock;
    mutable concurrent::spinlock m_finishLock;

    boost::shared_ptr<boost::thread> m_pThread;

    const boost::chrono::milliseconds m_duration;

public:
    explicit Playable(types::duration_t duration);
    virtual ~Playable();

    PlaybackState state() const { return m_state; }

    bool start();
    bool pause();
    bool stop(bool fromDestructor = false);

private:
    bool finished() const;
    void loop();

private:
    virtual void iteration() = 0;
    virtual void onStart() {}
    virtual void onPause() {}
    virtual void onStop() {}

};
} // namespace interface
} // namespace scene

#endif // PLAYABLE_H
