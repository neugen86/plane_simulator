#include "subscription.h"

namespace interchange
{
typedef boost::chrono::high_resolution_clock clock_t;

const types::duration_t Subscription::DefaultDuration(0);

Subscription::Subscription(types::duration_t duration)
    : SubscriptionConsumer()
    , SubscriptionProducer()
    , m_active(true)
    , m_duration(duration)
    , m_dataLock()
    , m_activeLock()
    , m_durationLock()
    , m_then(clock_t::now())
{
}

Subscription::~Subscription()
{
    m_event.reset();
}

void Subscription::setDuration(types::duration_t duration)
{
    concurrent::guard guard(m_durationLock);
    m_duration = boost::chrono::milliseconds(duration);
}

types::duration_t Subscription::duration() const
{
    concurrent::guard guard(m_durationLock);
    return m_duration.count();
}

void Subscription::set(const ObjectList& list)
{
    if (!m_dataLock.try_lock())
        return;

    m_list = list;
    m_dataLock.unlock();

    notify();
}

ObjectList Subscription::get() const
{
    wait();

    concurrent::guard guard(m_dataLock);
    return m_list;
}

void Subscription::deactivate()
{
    {
        concurrent::guard guard(m_activeLock);
        m_active = false;
    }
    {
        concurrent::guard guard(m_dataLock);
        m_list.clear();
    }

    notify();
}

bool Subscription::active() const
{
    concurrent::guard guard(m_activeLock);
    return m_active;
}

bool Subscription::expired() const
{
    using namespace boost::chrono;

    {
        concurrent::guard guard(m_durationLock);
        if (m_duration.count() <= 0)
            return true;
    }

    const clock_t::time_point now = clock_t::now();

    const milliseconds spent =
            duration_cast<milliseconds>(now - m_then);
    {
        concurrent::guard guard(m_durationLock);
        if (m_duration > spent)
            return false;
    }

    m_then = now;

    return true;
}

void Subscription::wait() const
{
    m_event.wait();
    m_event.reset();
}

void Subscription::notify() const
{
    m_event.set();
}
} // namespace interchange
