#include "subscription.h"

#include <cmath>

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
    m_duration = boost::chrono::milliseconds(std::max(duration, 0l));
}

types::duration_t Subscription::duration() const
{
    concurrent::guard guard(m_durationLock);
    return m_duration.count();
}

void Subscription::set(const SubscriptionData& data)
{
    if (!m_dataLock.try_lock())
        return;

    m_data = data;
    m_dataLock.unlock();

    notify();
}

SubscriptionData Subscription::get() const
{
    wait();

    concurrent::guard guard(m_dataLock);
    return m_data;
}

void Subscription::deactivate()
{
    {
        concurrent::guard guard(m_activeLock);
        m_active = false;
    }
    {
        concurrent::guard guard(m_dataLock);
        m_data = SubscriptionData();
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
