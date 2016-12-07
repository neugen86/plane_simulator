#ifndef EVENT_H
#define EVENT_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace concurrent
{
typedef boost::mutex mutex;
typedef boost::condition_variable condition;
typedef boost::mutex::scoped_lock scoped_lock;

class event
{
    mutable bool m_state;
    mutable mutex m_mutex;
    mutable condition m_condition;

public:
    explicit event(bool state = false)
        : m_state(state) {}

    bool state() const { return m_state; }

    void set() const
    {
        scoped_lock lock(m_mutex);
        m_state = true;
    }

    void reset() const
    {
        scoped_lock lock(m_mutex);
        m_state = false;
        m_condition.notify_one();
    }

    void wait() const
    {
        scoped_lock lock(m_mutex);
        while(m_state) m_condition.wait(lock);
    }
};
} // namespace concurrent

#endif // EVENT_H
