#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include "utils/types.h"

namespace interchange
{
template <typename T>
class SubscriptionReader
{
public:
    virtual ~SubscriptionReader() {}

    virtual std::list<T> get() = 0;
    virtual void wait() = 0;
};

template <typename T>
class SubscriptionWriter
{
    const boost::chrono::milliseconds m_duration;
    boost::chrono::high_resolution_clock::time_point m_prevTime;

public:
    explicit SubscriptionWriter(types::duration_t duration)
        : m_duration(duration) {}

    virtual ~SubscriptionWriter() {}

    virtual void set(const std::list<T>& list) = 0;
    virtual void notify() = 0;

    bool expired();
};

template <typename T>
class Subscription
        : public SubscriptionReader<T>
        , public SubscriptionWriter<T>
{
    bool m_notify;

    boost::mutex m_listGuard;
    boost::mutex m_notifyGuard;

    boost::condition_variable m_notifyCondition;

    std::list<T> m_list;

public:
    explicit Subscription(types::duration_t duration)
        : SubscriptionReader<T>()
        , SubscriptionWriter<T>(duration) {}

    void set(const std::list<T>& list);
    std::list<T> get();

    void notify();
    void wait();

};
} // namespace interchange

#endif // SUBSCRIPTION_H
