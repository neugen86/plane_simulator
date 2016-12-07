#ifndef ITERATIVE_H
#define ITERATIVE_H

#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>

#include "physics/types.h"
#include "concurrent/lock.h"
#include "concurrent/event.h"

namespace scene
{
namespace utils
{
class Iterative
        : private boost::noncopyable
{
    bool m_paused;
    bool m_stopped;
    bool m_finishFlag;

    concurrent::event m_resumeEvent;

    concurrent::spinlock m_lock;
    concurrent::spinlock m_finishLock;

    boost::shared_ptr<boost::thread> m_pThread;

public:
    Iterative();
    virtual ~Iterative();

    bool start();
    bool pause();
    bool stop();

private:
    void loop();

private:
    virtual void iterate() = 0;

};
} // namespace utils
} // namespace scene

#endif // ITERATIVE_H
