#ifndef BROADCASTER_H
#define BROADCASTER_H

#include "subscription.h"

namespace interchange
{
typedef boost::shared_ptr<SubscriptionConsumer> ConsumerPtr;

class Broadcaster
{
private:
    mutable concurrent::spinlock m_lock;

    std::list<boost::shared_ptr<Subscription>> m_subscriptions;

public:
    Broadcaster();
    virtual ~Broadcaster();

    ConsumerPtr subscribe();
    void unsubscribe(const ConsumerPtr& reader);

    bool haveSubscriptions() const;

protected:
    void feed(const ObjectList& list);

private:
    void clear();

};
} // namespace interchange

#endif // BROADCASTER_H
