#include "broadcaster.h"

namespace interchange
{
typedef boost::shared_ptr<Subscription> SubscriptionPtr;

Broadcaster::Broadcaster()
    : m_lock() {}

ReaderPtr Broadcaster::subscribe()
{
    concurrent::guard guard(m_lock);

    const SubscriptionPtr pSubscription(new Subscription);
    m_subscriptions.push_back(pSubscription);

    return pSubscription;
}

void Broadcaster::unsubscribe(const ReaderPtr& reader)
{
    concurrent::guard guard(m_lock);

    const SubscriptionPtr pSubscription =
            boost::static_pointer_cast<Subscription>(reader);

    m_subscriptions.remove(pSubscription);
}

bool Broadcaster::haveSubscriptions() const
{
    concurrent::guard guard(m_lock);
    return !m_subscriptions.empty();
}

void Broadcaster::feed(const ObjectList& list)
{
    concurrent::guard guard(m_lock);

    for (const WriterPtr& writer: m_subscriptions)
    {
        writer->set(list);
    }
}
} // namespace interchange
