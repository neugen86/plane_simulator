#include "subscription.h"

namespace interchange
{
void Subscription::set(const ObjectList& list)
{
    concurrent::guard guard(m_listLock);
    m_list = list;
}

ObjectList Subscription::get() const
{
    concurrent::guard guard(m_listLock);
    return m_list;
}

void Subscription::notify() const
{
    concurrent::guard guard(m_eventLock);
    m_event.set();
}

void Subscription::wait() const
{
    concurrent::guard guard(m_eventLock);
    m_event.wait();
    m_event.reset();
}
} // namespace interchange
