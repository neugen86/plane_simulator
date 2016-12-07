#ifndef BROADCASTER_H
#define BROADCASTER_H

#include "subscription.h"

namespace interchange
{
typedef boost::shared_ptr<SubscriptionReader> ReaderPtr;

class Broadcaster
{
private:
    typedef boost::shared_ptr<SubscriptionWriter> WriterPtr;

    mutable concurrent::spinlock m_lock;

    std::list<WriterPtr> m_subscriptions;

public:
    Broadcaster();
    virtual ~Broadcaster() {}

    ReaderPtr subscribe();
    void unsubscribe(const ReaderPtr& reader);

    bool haveSubscriptions() const;

protected:
    void feed(const ObjectList& list);

};
} // namespace interchange

#endif // BROADCASTER_H
