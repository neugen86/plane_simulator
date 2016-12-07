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

    boost::mutex m_guard;
    std::list<WriterPtr> m_subscriptions;

public:
    ReaderPtr subscribe();
    void unsubscribe(const ReaderPtr& reader);

    bool broadcasting();

protected:
    void feed(const ObjectList& list);

};
} // namespace interchange

#endif // BROADCASTER_H
