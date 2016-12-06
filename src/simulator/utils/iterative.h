#ifndef ITERATIVE_H
#define ITERATIVE_H

#include "utils/types.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

namespace utils
{
class Iterative : private boost::noncopyable
{
    bool m_paused;
    bool m_stopped;

    boost::mutex m_guard;
    boost::mutex m_stopGuard;
    boost::mutex m_pauseGuard;

    boost::condition_variable m_resumeCondition;

    boost::shared_ptr<boost::thread> m_pThread;

    const boost::chrono::milliseconds m_timeout;

public:
    explicit Iterative(types::duration_t timeout);
    virtual ~Iterative();

    bool start();
    bool pause();
    bool stop();

private:
    void loop();
    void handleTimeout(boost::chrono::milliseconds timeout);
    void handleResume();
    bool handleStop();

private:
    virtual void iterate() = 0;

};
} // namespace utils

#endif // ITERATIVE_H
