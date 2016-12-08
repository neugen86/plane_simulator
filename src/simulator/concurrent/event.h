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
    mutable condition m_condition;

    mutable mutex m_stateMutex;
    mutable mutex m_conditionMutex;

public:
    explicit event(bool state = false)
        : m_state(state) {}

    bool state() const
    {
        scoped_lock lock(m_stateMutex);
        return m_state;
    }

    void set() const
    {
        {
            scoped_lock lock(m_stateMutex);
            m_state = true;
        }

        m_condition.notify_one();
    }

    void reset() const
    {
        scoped_lock lock(m_stateMutex);
        m_state = false;
    }

    void wait() const
    {
        scoped_lock lock(m_conditionMutex);
        while(!m_state) m_condition.wait(lock);
    }
};
} // namespace concurrent

#endif // EVENT_H
