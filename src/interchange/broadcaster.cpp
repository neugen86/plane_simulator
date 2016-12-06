#include "broadcaster.h"

namespace interchange
{
template <typename T>
boost::shared_ptr<SubscriptionReader<T>> Broadcaster<T>::subscribe(types::duration_t duration)
{
    boost::shared_ptr<Subscription<T>> subscription(new Subscription<T>(duration));

    {
        boost::mutex::scoped_lock lock(m_guard);
        m_subscriptions.push_back(subscription);
    }

    return subscription;
}

template <typename T>
void Broadcaster<T>::unsubscribe(const ReaderPtr& subscription)
{
    boost::mutex::scoped_lock lock(m_guard);
    m_subscriptions.erase(m_subscriptions.remove(subscription));
}

template <typename T>
void Broadcaster<T>::feed(const std::list<T>& list)
{
    boost::mutex::scoped_lock lock(m_guard);

    for (const WriterPtr& writer: m_subscriptions)
    {
        if (writer->expired())
            writer->set(list);
    }
}
} // namespace interchange
