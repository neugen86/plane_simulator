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

    boost::condition_variable m_condResume;

    boost::shared_ptr<boost::thread> m_pThread;

    const types::duration_t m_duration;

public:
    explicit Iterative(types::duration_t duration);
    virtual ~Iterative();

    types::duration_t getDuration() const { return m_duration; }

    bool start();
    bool pause();
    bool stop();

private:
    void loop();
    void handleTimeout();
    void handleResume();
    bool handleStop();

private:
    virtual void iterate() = 0;

};
} // namespace utils

#endif // ITERATIVE_H
