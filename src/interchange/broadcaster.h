#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <vector>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include "utils/types.h"
#include "subscription.h"

namespace interchange
{
template <typename T>
class Broadcaster
{
public:
    typedef boost::shared_ptr<SubscriptionReader<T>> ReaderPtr;
    typedef boost::shared_ptr<SubscriptionWriter<T>> WriterPtr;

private:
    boost::mutex m_guard;
    std::list<WriterPtr> m_subscriptions;

public:
    ReaderPtr subscribe(types::duration_t duration);
    void unsubscribe(const ReaderPtr& subscription);

protected:
    void feed(const std::list<T>& list);

};
} // namespace interchange

#endif // BROADCASTER_H
