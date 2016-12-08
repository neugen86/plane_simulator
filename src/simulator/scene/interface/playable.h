#ifndef PLAYABLE_H
#define PLAYABLE_H

#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>

#include "physics/types.h"
#include "concurrent/lock.h"
#include "concurrent/event.h"

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

    concurrent::event m_resumeEvent;

    mutable concurrent::spinlock m_lock;
    mutable concurrent::spinlock m_finishLock;

    boost::shared_ptr<boost::thread> m_pThread;

public:
    Playable();
    virtual ~Playable();

    bool start();
    bool pause();
    bool stop();

private:
    bool finished() const;
    void loop();

private:
    virtual void play() = 0;

};
} // namespace interface
} // namespace scene

#endif // PLAYABLE_H
