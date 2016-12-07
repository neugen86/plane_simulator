#include "subscription.h"

namespace interchange
{
void Subscription::set(const ObjectList& list)
{
    boost::mutex::scoped_lock lock(m_listGuard);
    m_list = list;
}

ObjectList Subscription::get()
{
    boost::mutex::scoped_lock lock(m_listGuard);
    return m_list;
}

void Subscription::notify()
{
    boost::mutex::scoped_lock lock(m_notifyGuard);
    m_notify = true;
}

void Subscription::wait()
{
    boost::mutex::scoped_lock lock(m_notifyGuard);
    while (!m_notify) m_notifyCondition.wait(lock);
    m_notify = false;
}

} // namespace interchange
