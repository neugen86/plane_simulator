#include "subscription.h"

namespace interchange
{
template <typename T>
bool SubscriptionWriter<T>::expired()
{
    using namespace boost::chrono;
    typedef high_resolution_clock clock_t;

    milliseconds spent = duration_cast<milliseconds>(clock_t::now() - m_prevTime);
    bool expired = (spent - m_duration).count() >= 0;

    if (expired)
    {
        m_prevTime = clock_t::now();
    }

    return expired;
}

template <typename T>
void Subscription<T>::set(const std::list<T> &list)
{
    boost::mutex::scoped_lock lock(m_listGuard);
    m_list = list;
}

template <typename T>
std::list<T> Subscription<T>::get()
{
    boost::mutex::scoped_lock lock(m_listGuard);
    return m_list;
}

template <typename T>
void Subscription<T>::notify()
{
    boost::mutex::scoped_lock lock(m_notifyGuard);
    m_notify = true;
}

template <typename T>
void Subscription<T>::wait()
{
    boost::mutex::scoped_lock lock(m_notifyGuard);
    while (!m_notify) m_notifyCondition.wait(lock);
    m_notify = false;
}

} // namespace interchange
