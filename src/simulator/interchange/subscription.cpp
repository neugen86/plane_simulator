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

void Subscription::set(const ObjectList& list)
{
    concurrent::guard guard(m_lock);
    m_list = list;
    notify();
}

ObjectList Subscription::get() const
{
    wait();
    concurrent::guard guard(m_lock);
    return m_list;
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
