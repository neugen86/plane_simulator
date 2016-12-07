#include "broadcaster.h"

namespace interchange
{
typedef boost::shared_ptr<Subscription> SubscriptionPtr;

ReaderPtr Broadcaster::subscribe()
{
    boost::mutex::scoped_lock lock(m_guard);

    SubscriptionPtr pSubscription(new Subscription);
    m_subscriptions.push_back(pSubscription);

    return pSubscription;
}

void Broadcaster::unsubscribe(const ReaderPtr& reader)
{
    boost::mutex::scoped_lock lock(m_guard);

    SubscriptionPtr pSubscription =
            boost::static_pointer_cast<Subscription>(reader);

    m_subscriptions.remove(pSubscription);
}

bool Broadcaster::broadcasting()
{
    boost::mutex::scoped_lock lock(m_guard);
    return !m_subscriptions.empty();
}

void Broadcaster::feed(const ObjectList& list)
{
    boost::mutex::scoped_lock lock(m_guard);

    for (const WriterPtr& writer: m_subscriptions)
    {
        writer->set(list);
    }
}
} // namespace interchange
