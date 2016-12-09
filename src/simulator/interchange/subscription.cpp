#include "subscription.h"

namespace interchange
{
Subscription::Subscription()
    : SubscriptionConsumer()
    , SubscriptionProducer()
    , m_lock()
{
}

Subscription::~Subscription()
{
    m_event.reset();
}

ObjectList Subscription::get() const
{
    wait();

    concurrent::guard guard(m_lock);
    return m_list;
}

void Subscription::set(const ObjectList& list)
{
    if (!m_lock.try_lock())
        return;

    m_list = list;
    m_lock.unlock();

    notify();
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
