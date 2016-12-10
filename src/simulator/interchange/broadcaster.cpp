#include "broadcaster.h"

namespace interchange
{

typedef boost::shared_ptr<SubscriptionProducer> ProducerPtr;

Broadcaster::Broadcaster()
    : m_lock()
{
}

Broadcaster::~Broadcaster()
{
    clear();
}

ConsumerPtr Broadcaster::subscribe(types::duration_t duration)
{
    concurrent::guard guard(m_lock);

    const SubscriptionPtr pSubscription(new Subscription(duration));
    m_subscriptions.push_back(pSubscription);

    return pSubscription;
}

void Broadcaster::unsubscribe(const ConsumerPtr& consumer)
{
    concurrent::guard guard(m_lock);

    const SubscriptionPtr pSubscription =
            boost::static_pointer_cast<Subscription>(consumer);

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

    for (const ProducerPtr& writer : m_subscriptions)
    {
        if (writer->expired())
            writer->set(list);
    }
}

void Broadcaster::clear()
{
    concurrent::guard guard(m_lock);
    m_subscriptions.clear();
}
} // namespace interchange
