#ifndef BROADCASTER_H
#define BROADCASTER_H

#include "subscription.h"

namespace interchange
{
typedef boost::shared_ptr<SubscriptionConsumer> ConsumerPtr;

class Broadcaster
{
private:
    typedef boost::shared_ptr<Subscription> SubscriptionPtr;

    mutable concurrent::spinlock m_lock;

    std::list<SubscriptionPtr> m_subscriptions;

public:
    Broadcaster();
    virtual ~Broadcaster();

    ConsumerPtr subscribe(types::duration_t duration = Subscription::DefaultDuration);
    void unsubscribe(const ConsumerPtr& reader);

    bool haveSubscriptions() const;

protected:
    void feed(const SubscriptionData& data, bool force);

private:
    void clear();

};
} // namespace interchange

#endif // BROADCASTER_H
